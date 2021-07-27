#include "Socket.hpp"

ft::Socket::Socket() {

	try {
		create_fd();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	// std::cout << "Socket successfully created !" << std::endl;
}

ft::Socket::Socket(int fd, const sockaddr_in &address) {

	this->_fd = fd;
	this->_address = address;
	// std::cout << "Socket successfully created !" << std::endl;
}

ft::Socket::~Socket() {
	// std::cout << "Socket desctructor called" << std::endl;
}


//	GETTERS / SETTERS
int ft::Socket::get_fd(void) const {
	return _fd;
}

struct sockaddr_in &ft::Socket::get_address(void) {
	return _address;
}

void ft::Socket::set_address(uint16_t port, const std::string &host) {
	bzero(&_address, sizeof(_address));
	_address.sin_family = AF_INET;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = inet_addr(host.c_str());
}


//	MEMBER FUNCTIONS
void	ft::Socket::create_fd() {
	_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (ERROR(_fd))
		throw SocketException("Error while creating socket");
}

void	ft::Socket::make_non_blocking(void) {
	if (ERROR(fcntl(this->get_fd(), F_SETFL, O_NONBLOCK)))
		throw SocketException("Error with fnctl");
}


// SOCKETEXCEPTION CLASS
ft::Socket::SocketException::SocketException(const std::string &msg) throw() : _msg(msg) {}

ft::Socket::SocketException::~SocketException() throw() {}

const char *ft::Socket::SocketException::what(void) const throw() {
	return _msg.c_str();
} 
