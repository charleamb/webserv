#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <queue>
# include "Request.hpp"
# include "../config/ConfigServer.hpp"
# include "../config/Location.hpp"
# include "Response.hpp"
# include "Server.hpp"
// # include "../config/ConfigServer.hpp"
class Response;

class Server;
class Request;
class Client {
	private:
		int							_fd;
		Server						*_server;

	public:
		Request						*request;
		std::queue<Response>		responses;
		bool						connected;

		Client();
		Client(const Client &ref);
		~Client();
		Client(int fd, Server *server);


		const Location				*find_location(const std::vector<Location> &locations, const std::string &requested_route) const ;
		ConfigServer				*find_config(void) const;

		const Response				*get_response(void) const;
		const std::queue<Response>	&get_responses(void) const;
		Request						*get_request(void) const;
		const Server				*get_server(void) const ;
		void						set_server(Server *server);
};
#endif
