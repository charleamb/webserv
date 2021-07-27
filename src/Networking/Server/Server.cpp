#include "Server.hpp"
#include "../../parsing/HTTP/Request.hpp"
#include "../../Constants.hpp"

bool 				ft::Server::shutdown = false;

ft::Server::Server(std::vector<config::ServerConfig> configs) : _configs(configs), _fdmax(0) {
	init_server(configs);
	std::cerr << "Server listening for incoming connections" << std::endl;
	run();
}

void	ft::Server::init_server(std::vector<config::ServerConfig> configs) {
	FD_ZERO(&_masterfds);
	for (size_t i = 0; i < configs.size(); i++)
	{
		Socket listener = ListeningSocket(configs[i].get_port(), configs[i].get_addr());
		_listeners.insert(std::make_pair(listener.get_fd(), listener));
		FD_SET(listener.get_fd(), &_masterfds);
		_fdmax = std::max(_fdmax, listener.get_fd());
	}
}

void	ft::Server::run(void) {

	struct timeval					timeout;
	fd_set							readfds;
	fd_set							writefds;

	bzero(&timeout, sizeof(timeval)); //non-blocking
	while (!shutdown)
	{
		readfds = writefds = _masterfds;
		if (select(_fdmax + 1, &readfds, &writefds, NULL, &timeout) < 0)
			std::cerr << "select error" << std::endl;
			// shutdown(strerror(errno));

		for (int fd = 0; fd <= _fdmax; fd++) {
			if (FD_ISSET(fd, &writefds)) {
				if (send_response(_clients[fd]) < 0)
					disconnect(_clients[fd]);
			}
			if (FD_ISSET(fd, &readfds)) {
				if (is_listener(fd)) {
					accept_connection(fd);
				}
				else {
					if (receive_request(_clients[fd]) < 0)
						disconnect(_clients[fd]);
					// else
					// 	process_request(_clients[fd]);
				}
			}
		}
	}
}

bool	ft::Server::is_listener(int fd) {
	return _listeners.count(fd);
}

ft::Server::~Server() {
	std::cout << "Server destructor called" << std::endl;
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		close(it->first);
		delete it->second;
	}

	for (std::map<int, Socket>::iterator it = _listeners.begin(); it != _listeners.end(); it++) {
		close(it->first);
	}
}

int		ft::Server::accept_connection(int listener) {

	struct sockaddr_in	client_addr;
	socklen_t			addr_len = (socklen_t)SIZEOF_SOCKADDR_IN;
	int					client_fd;

	client_fd = accept(listener, (sockaddr *)&client_addr, &addr_len);
	if (client_fd < 0) {
		std::cerr << strerror(errno) << std::endl;
		return -1;
	}

	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	FD_SET(client_fd, &_masterfds);
	Client *client = new Client(client_fd, client_addr);
	_clients.insert(std::make_pair(client_fd, client));
	_fdmax = std::max(_fdmax, client_fd);
	// std::cerr << "There are " << _clients.size() << " clients!" << std::endl;
	return 0;
}

void	ft::Server::disconnect(Client *client) {

	FD_CLR(client->get_fd(), &_masterfds);
	close(client->get_fd());
	if (_fdmax == client->get_fd())
		--_fdmax;
	_clients.erase(client->get_fd());
}


int		ft::Server::receive_request(Client *client) {

	int		bytes_read;
	char	buffer[BUFFER_SIZE];

	bzero(buffer, BUFFER_SIZE);
	bytes_read = recv(client->get_fd(), buffer, BUFFER_SIZE, 0);
	if (bytes_read == -1)
		std::cerr << "Recv failed" << std::endl;
	else if (bytes_read == 0)
		std::cerr << "Client closed connection" << std::endl;
	else
		client->_req.update(buffer, bytes_read);
	return bytes_read;
}

int		ft::Server::send_response(Client *client) {

	int				bytes_sent;

	if (client->get_responses().empty())
		return 1;

	ft::Response	response = client->get_front_response();
	std::string		buffer = response.get_buffer().substr(response._offset, BUFFER_SIZE);

	bytes_sent = send(client->get_fd(), buffer.c_str(), buffer.size(), 0);
	if (bytes_sent == -1) {
		std::cerr << "Error with send" << std::endl;
		//erase response ?
		//update response status ?
	}
	else
	{
		client->get_front_response()._offset += bytes_sent;
		const status::Status response_status = client->get_front_response().update_status();
		if (response_status == status::Complete)
			client->get_responses().pop();
	}
	return bytes_sent;
}