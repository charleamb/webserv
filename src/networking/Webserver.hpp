#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include <map>
# include <sys/select.h>
# include <csignal>
# include <cstdlib>
# include <cstring>
# include <sys/types.h>
# include <sys/stat.h>
# include <exception>

# include "Server.hpp"
# include "Client.hpp"
# include "CGI.hpp"
# include "../config/config.hpp"
# include "../utils/utils.hpp"
# include "RequestInterpreter.hpp"
# include "Response.hpp"
# include "Request.hpp"
# define STDIN 0
# define QUEUE_SIZE 50
# define DEFAULT_CONFIG_PATH "../src/config/config.conf"


class Response;
class Request;
class Webserver {

	private:
		fd_set						_masterfds;
		int							_fdmax;
		std::map<int, Server>		_servers;
		std::map<int, Client>		_clients;
		std::map<int, Response *>	_cgi_output;
		std::map<int, Response *>	_cgi_input;

		std::vector<Server>			parseConfig(const char *filename);


		bool						setup_config(const char *filename);
		int							start_listener(Server server);
		void						accept_client(int server_socket);
		int							read_from_client(int client_fd);
		int							write_to_client(int client_fd);

		// void						send_response(Client &client, Response response);

		void						command_line(void);
		
		// bool 					check_request(Client &client);
		// bool						manage_request(Client &client, ConfigServer &config);

		// ConfigServer				*give_config(Client &client);

		void						list_clients(void);
		void						list_servers(void);
		void						CGI_output(int fd, Response *response);
		void						CGI_input(int fd, Response *response);

		// Response					get_file(std::string path, Client &client);
		void						disconnect(int client_fd);	
	public:
		Webserver(const char *config_file);
		~Webserver();
		void						config(const char *config_file);
		void						run();
		bool						g_signal;

		class WebserverException : public std::exception {
			private:
				int						_code;
				std::string				_msg;
			public:
				WebserverException(int code) throw();
				WebserverException(const std::string &msg) throw();
				int get_code(void) const;
				virtual ~WebserverException() throw();
				virtual const char *what() const throw();
	};
};
#endif
