#include "ListeningSocket.hpp"

ft::ListeningSocket::ListeningSocket(uint16_t port, const std::string &host) 
: BindingSocket(port, host) {
	try {
		_listen(get_fd());
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	// std::cout << "Listening socket successfully created" << std::endl;
};

ft::ListeningSocket::~ListeningSocket() {
	// std::cout << "Listening Socket destructor called" << std::endl;
}

void	ft::ListeningSocket::_listen(int socket) {
	if (ERROR(listen(socket, BACKLOG)))
		throw SocketException("Error when listening for connections on the socket");
}