#ifndef SERVER_HPP
# define SERVER_HPP
# include "../ft_networking.hpp"
# include "../Client/Client.hpp"
# include "../Sockets/ListeningSocket.hpp"
# include "../../parsing/Config/config.hpp"
# include "../Response/Response.hpp"
namespace ft {
	class Server {
	private:
		std::vector<config::ServerConfig>	_configs;
		fd_set								_masterfds;
		int									_fdmax;
		std::map<int, Socket>				_listeners;
		std::map<int, Client *>				_clients;
	
		void								init_server(std::vector<config::ServerConfig> configs);
		void								run(void);
		int									accept_connection(int listener);
		void								disconnect(Client *client);
		int									receive_request(Client *client);
		int									send_response(Client *client);
		bool								is_listener(int fd);
		// void								process_request(Client *client);

	public:
		static bool							shutdown;
		Server(std::vector<config::ServerConfig> configs);
		~Server();
	}; //end of Server class
} //end of namespace 
#endif
