//
// Created by alena on 06/07/2021.
//

#ifndef WEBSERV_CONFIG_HPP
#define WEBSERV_CONFIG_HPP

#include "parser/export.hpp"
#include "parsing/Tokens.hpp"

#include "Module/Cgi.hpp"
#include "Module/ClientMaxBodySize.hpp"
#include "Module/ErrorPage.hpp"
#include "Module/Listen.hpp"
#include "Module/ServerName.hpp"
#include "Module/Root.hpp"
#include "Module/Index.hpp"
#include "Module/Location.hpp"

#include "Structure/Bloc.hpp"
#include "Structure/Line.hpp"

#include "parsing/HTTP/Request.hpp"

#include <map>

namespace config
{
/*
 * Result of server block
 */
	class ServerConfig
	{
	private:
		int 			_port;
		std::string		_address;
		std::string		_name;
		std::string		_root;
		std::string		_index;
		size_t			_body_size;		// in bytes
		std::vector<LocationConfig>			_locations;
		std::map<int, std::string>			_error_pages;
		std::map<std::string, std::string>	_cgis;

		static ServerConfig		*active;

	public:

		ServerConfig();

		static	void	use(ServerConfig *ptr);

		/*
		 * SETTER
		 */
		static ServerConfig		*with_addr(tuple<std::string, int> addr);
		static	ServerConfig	*dump(slice unused);
		static	ServerConfig	*name(slice name);
		static	ServerConfig	*max_size(size_t max_size);
		static	ServerConfig	*root(slice root);
		static	ServerConfig	*cgi(tuple<slice, slice> cgi);
		static	ServerConfig	*error_codes(std::vector<std::pair<int, std::string> > errors);
		static	ServerConfig	*index(slice index);
		static	ServerConfig	*with_location(LocationConfig config);

		/*
		 * GETTER
		 */
		unsigned short 				get_port();
		std::string					get_addr();
		std::string					get_name();
		std::string					get_root();
		std::string					get_index();
		size_t						get_max_body_size();

		LocationConfig				get_location(const Request &req);
		Result<std::string>			get_error_page(int status_code);

		//TODO
		Result<std::string>		cgi_binary(std::string ext);

		friend std::ostream &operator<<(std::ostream& stream, const ServerConfig& cfg);
	};
/*
 * ServerContent is a line for block server
 * listen | server_name ...
 */
	class ServerContent: public Parser<ServerConfig>
	{
	public:
		ServerContent() { }

		result_type 	operator()(const slice &input)
		{
			ServerConfig	cfg;
			ServerConfig::use(&cfg);

			ParserResult<slice>	res = as_slice(many(alt(
					as_slice(map(Location(), ServerConfig::with_location)),
					as_slice(map(directive(Listen()), ServerConfig::with_addr)),
					as_slice(map(directive(ServerName()), ServerConfig::name)),
					as_slice(map(directive(Root()), ServerConfig::root)),
					as_slice(map(directive(Cgi()), ServerConfig::cgi)),
					as_slice(map(directive(ClientMaxBodySize()), ServerConfig::max_size)),
					as_slice(map(directive(Index()), ServerConfig::index)),
					alt(
						as_slice(map(directive(ErrorPage()), ServerConfig::error_codes)),
						as_slice(map(comment, ServerConfig::dump)),
						as_slice(map(empty_line, ServerConfig::dump))
					)
					)))(input);
			return res.map(cfg);
		}
	};

	/*
	 * A create the server block
	 */
	class Server : public Parser<ServerConfig>
	{
	public:
		Server() { }

		result_type operator()(const slice &input)
		{
			ParserResult<tuple<slice, ServerConfig> >	res = Bloc<Tag, ServerContent>(Tag("server"), ServerContent())(input);
			if (res.is_err())
				return res.convert<ServerConfig>();
			return res.map(res.unwrap().second);
		}
	};
}

class ConfigParser : public Parser<std::vector<config::ServerConfig> >
{
public :
	ConfigParser() { }

	result_type operator()(const slice &input)
	{
		return terminated(many(config::Server()), take_with(newline, true))(input);
	}
};

#endif //WEBSERV_CONFIG_HPP
