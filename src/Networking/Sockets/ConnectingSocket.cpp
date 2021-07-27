# include "ConnectingSocket.hpp"

ft::ConnectingSocket::ConnectingSocket(const struct sockaddr_in &server_addr)
: Socket() {

	try {
		_connect(get_fd(), server_addr);

	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	// std::cout << "Socket connected to server" << std::endl;
}

ft::ConnectingSocket::~ConnectingSocket() {
	// std::cout << "Connecting Socket destructor called !" << std::endl;
}

void	ft::ConnectingSocket::_connect(int socket, const struct sockaddr_in &address) {
	int connection_status;
	connection_status = connect(socket, (struct sockaddr *)&address, sizeof(address));
	if (connection_status < 0)
		throw SocketException("Error when connecting the socket");
}