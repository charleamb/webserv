#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/socket.h>
# include <sys/errno.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cstring>

# include "../config/ConfigServer.hpp"
# include "Request.hpp"

# define SIZEOF_SOCKADDR_IN sizeof(sockaddr_in)
# define BACKLOG 100

class Webserver;
class Request;
class Server {
	private:
		Webserver									*_webserver;
		struct sockaddr_in 							_address;
		int											_port;
		int											_fd;
		std::vector<ConfigServer>					_config;
	public:
		Server();
		~Server();
		Server(Webserver *webserver, int port, ConfigServer &config);
		Server(const Server &x);
		Server &operator=(const Server &x);

		ConfigServer								*find_hostname(const std::string &hostField);
		int											match_wildcard(std::string &host, const std::string &hostField);
		int											start_listening(void);

		int											get_port(void) const;
		int											get_fd(void) const;
		std::vector<ConfigServer>					&get_config(void);
		void										set_port(int port);
		void										set_fd(int fd);
		void										set_config(const std::vector<ConfigServer> &config);
		void										set_address(void);

		class ServerException : public std::exception {
			private:
				std::string		_msg;

			public:
				ServerException(const std::string &msg) throw();
				virtual ~ServerException() throw();
				virtual const char *what() const throw();
		};
};
#endif
