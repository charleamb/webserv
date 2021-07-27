#include "BindingSocket.hpp"

ft::BindingSocket::BindingSocket(uint16_t port, const std::string &host) 
: Socket() {

	try {
		set_address(port, host);

		make_non_blocking();

		_bind(get_fd(), get_address());
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		std::cout << strerror(errno) << std::endl;
		close(get_fd());
		exit(EXIT_FAILURE);
	}
	// std::cout << "Socket successfully binded !" << std::endl;
}

ft::BindingSocket::~BindingSocket(void) {
	//TODO: release memory address
	// std::cout << "Binding Socket destructor called" << std::endl;
}

void ft::BindingSocket::_bind(int socket, struct sockaddr_in &address) {
	//set server socket to allow multiple connections
	int opt = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) 
		throw SocketException("Error setsockopt");
	//bind address to socket
	if (bind(socket, reinterpret_cast<struct sockaddr *>(&address), sizeof(address)) < 0)
		throw SocketException("Error while binding the socket");
}
