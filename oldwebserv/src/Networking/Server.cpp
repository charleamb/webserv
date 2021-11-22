#include "Server.hpp"

//	CONSTRUCTORS / DESTRUCTOR #####################################################################################################

Server::Server()
: _webserver()//, _port()
{
	std::cout << ")))constru NOTHING" << std::endl;
}

Server::~Server() {}

Server::Server(Webserver *webserver, int port, ConfigServer &config)
: _webserver(webserver), _port(port)
{
	std::cout << "constru fill" << std::endl;
	_config.push_back(config);
}

Server::Server(const Server &x)
: _webserver(x._webserver),
_port(x._port),
_fd(x._fd),
_config(x._config)
{}

Server &Server::operator=(const Server &x)
{
	_port = x._port;
	_fd = x._fd;
	_config = x._config;
	return *this;
}

// FUNCTIONS ###########################################################################################################################

int											Server::start_listening(void) {
	int opt = 1;
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd < 0)
		throw ServerException(std::strerror(errno));
	else if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		throw ServerException(std::strerror(errno));
	set_address();
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0)
		throw ServerException(std::strerror(errno));
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int)) < 0)
		throw ServerException(std::strerror(errno));
	if (bind(_fd, reinterpret_cast<struct sockaddr *>(&_address), sizeof(_address)) < 0)
		throw ServerException(std::strerror(errno));
	if (listen(_fd, BACKLOG) < 0)
		throw ServerException(std::strerror(errno));
	return _fd;
}

//returns the nb of characters that matched
//in positive number if string is starting with wildcard
//in negative number if string is ending with wildcard
int								Server::match_wildcard(std::string &host, const std::string &hostField) {
	//TODO: check that wildcards are next to dot border???
	std::string::iterator it = host.begin();
	if (*it == '*') {
		host.erase(it);
		size_t index = hostField.find(host, 0);
		if (index == std::string::npos || index != (hostField.size() - host.size()))
			return 0;
		return host.size();
	}
	it = host.end();
	--it;
	if (*it == '*') {
		host.erase(it);
		size_t index = hostField.find(host, 0);
		if (index == std::string::npos || index != 0)
			return 0;
		return host.size() * -1;
	}
	return 0;
}

ConfigServer					*Server::find_hostname(const std::string &hostField) {
	int index = 0;
	int oldMatchLevel = 0;
	if (!hostField.empty()) {
		for (size_t i = 0; i < _config.size(); i++) {

			if (_config[i].host.compare(hostField) == 0) //found exact name
				return &_config[i];
			else {
				int matchLevel = match_wildcard(_config[i].host, hostField);
				if ((matchLevel > 0 && matchLevel > oldMatchLevel) || (oldMatchLevel < 0 && matchLevel < oldMatchLevel)) { //prioritize starting wildcard over ending ones
					index = i;
					oldMatchLevel = matchLevel;
				}
			}
		}
		//if oldmatchlevel == 0
		//	on cherche le premier regex
	}
	return &_config[index];
}

// SETTERS & GETTERS ###################################################################################################################

void										Server::set_address(void) {
	std::memset(&_address, 0, sizeof(_address));
	_address.sin_family = AF_INET;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr = inet_addr("0.0.0.0");
}

void										Server::set_port(int port) {
	_port = port;
}

void										Server::set_fd(int fd) {
	_fd = fd;
}

void										Server::set_config(const std::vector<ConfigServer> &config) {
	_config = config;
}

int											Server::get_port(void) const {
		return _port;
}

int											Server::get_fd(void) const {
	return _fd;
}

std::vector<ConfigServer>					&Server::get_config(void) {
	return _config;
}


//	SERVEREXCEPTION #######################################################################################################################

Server::ServerException::ServerException(const std::string &msg) throw() : _msg(msg) {}

Server::ServerException::~ServerException() throw() {}

const char *Server::ServerException::what(void) const throw() {
	return _msg.c_str();
} 
