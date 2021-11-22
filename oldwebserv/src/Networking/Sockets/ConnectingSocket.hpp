#ifndef CONNECTING_SOCKET_HPP
# define CONNECTING_SOCKET_HPP

# include "Socket.hpp"

namespace ft {
	class ConnectingSocket : public Socket {
		private:
		void	_connect(int socket, const struct sockaddr_in &address);

		public:
		ConnectingSocket(const struct sockaddr_in &server_addr);
		virtual ~ConnectingSocket();
	}; //end of ConnectingSocket class
}
#endif