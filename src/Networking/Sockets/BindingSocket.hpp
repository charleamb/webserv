#ifndef BINDING_SOCKET_HPP
# define BINDING_SOCKET_HPP

# include "Socket.hpp"

namespace ft {
	class BindingSocket : public Socket {
		private:
		void	_bind(int socket, struct sockaddr_in &address);

		public:
		BindingSocket(uint16_t port, const std::string &host);
		virtual ~BindingSocket();
	}; // end of BindingSocket class
}
#endif