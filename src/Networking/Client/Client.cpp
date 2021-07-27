#include  "Client.hpp"
//demarrer offset au debut de la string si elle vide, sinon à la fin
ft::Client::Client(int fd, sockaddr_in address) : Socket(fd, address) {
	std::vector<unsigned char> start;
	start.push_back(0);
	// std::cout << "Client successfully created" << std::endl;
}

ft::Client::~Client() {
	// std::cout << "Client destructor called" << std::endl;
	//TODO: destroy queue of response
}


std::queue<ft::Response>		&ft::Client::get_responses() {
	return _responses;
}

ft::Response						&ft::Client::get_front_response() {
	return _responses.front();
}
