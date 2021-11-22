#include "Webserver.hpp"
#include "../config/config.hpp"
#include <sstream>

//	CONSTRUCTOR / DESTRUCTOR #####################################################################################################################################

Webserver::Webserver(const char *config_file)
: _fdmax(0)
{
	if (!strcmp(config_file, "global"))
		return;
	if (!config_file)
		config_file = DEFAULT_CONFIG_PATH;
	FD_ZERO(&_masterfds);
	if (!setup_config(config_file))
		throw WebserverException("Failed to start webserver");
}

Webserver::~Webserver()
{
	_servers.clear();
	_clients.clear();
}

//	ROUTINE #######################################################################################################################################################

void	Webserver::config(const char *config_file)
{
	if (!config_file)
		config_file = DEFAULT_CONFIG_PATH;
	FD_ZERO(&_masterfds);
	if (!setup_config(config_file))
		throw WebserverException("Failed to start webserver");
}

void	Webserver::run(void)
{
	struct timeval	timeout;
	fd_set			read_fds;
	fd_set			write_fds;
	int				retval;
	
	umask(S_IWOTH | S_IROTH | S_IRGRP | S_IWGRP | S_IWUSR | S_IRUSR); // le process ne pourra pas executer les fichiers uploadés (securité)

	g_signal = false;
	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);
	FD_SET(STDIN, &_masterfds);
	std::memset(&timeout, 0, sizeof(timeout)); //TODO: change
	list_servers();
	
	
	while (g_signal == false)
	{
		read_fds = write_fds = _masterfds;

		retval = select(_fdmax + 1, &read_fds, &write_fds, 0, &timeout);
		
		if (retval < 0)
			throw WebserverException(std::strerror(errno));
		else if (retval > 0)
		{
			for (int fd = 0; fd <= _fdmax; fd++)
			{
				if (FD_ISSET(fd, &read_fds))
				{
					if (fd == STDIN)
					{
						command_line(); //TODO: handle input from commandline
					}
					else if (_servers.count(fd))
					{
						accept_client(fd);
					}
					else if (_clients.count(fd))
					{
						if (read_from_client(fd) < 0)
							disconnect(fd);
					}
					else if (_cgi_output.count(fd))
						CGI_output(fd, _cgi_output[fd]);
				}
				if (FD_ISSET(fd, &write_fds))
				{
					if (_cgi_input.count(fd))
						CGI_input(fd, _cgi_input[fd]);
					else if (write_to_client(fd) < 0)
						disconnect(fd);

				}
			}
		}
	}
}

int							Webserver::read_from_client(int fd)
{
	RequestInterpreter *a;
	char 	buf[BUFFER_SIZE + 1];
	int		bytes_read;
	
	bzero(buf, BUFFER_SIZE + 1);
	bytes_read = recv(fd, buf, BUFFER_SIZE, 0);
	if (bytes_read <= 0) // il faut que ce soit <= sinon on ne traite pas le cas des deconnections == 0
		return -1;
	else if (bytes_read > 0)
	{
		if (!_clients[fd].request)
			_clients[fd].request = new Request();
		_clients[fd].request->buffer += buf;
		_clients[fd].request->parse();
		if (_clients[fd].request->status > BodyParsed) {
			_clients[fd].request->printvalue();
			RequestInterpreter a(&_clients[fd], &_cgi_output, &_cgi_input, &_masterfds, &_fdmax);
		}
	}
	return 0;		
}

int						Webserver::write_to_client(int fd) {
	if (!_clients.count(fd))
		return (0);

	if (!_clients[fd].responses.empty()) {

		Response 		*response = &(_clients[fd].responses.front());
		if (response->offset < response->buffer.size() && response->status == Ready)
		{
			std::string		buffer = response->buffer.substr(response->offset, BUFFER_SIZE);
			int				bytes_sent = send(fd, buffer.c_str(), buffer.size(), 0);
			if (bytes_sent < 0) {
				return -1;
			}
			response->offset += bytes_sent;
		}

		if (response->offset >= response->buffer.size() && response->status == Ready) {
			if (response->get_headers().get_fields()["Connection"] == "keep-alive")
				_clients[fd].connected = true;
			else
				_clients[fd].connected = false;
			std::cout << "sent : \n" << response->buffer << '\n';
			_clients[fd].responses.pop();
			if (!_clients[fd].connected)
				disconnect(fd);
				
		}
	}
	return 0;
}

void		Webserver::accept_client(int server_socket) {

	struct sockaddr_in	client_addr;
	socklen_t			addr_len = (socklen_t)SIZEOF_SOCKADDR_IN;
	int					client_fd;

	client_fd = accept(server_socket, reinterpret_cast<sockaddr*>(&client_addr), &addr_len);
	if (client_fd < 0)
		throw WebserverException(std::strerror(errno));
	else if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
		throw WebserverException(std::strerror(errno)); //must close the fd 
	else {
		Client /**client = new */client(client_fd, &(_servers[server_socket]));
		_clients.insert(std::make_pair(client_fd, client));
		FD_SET(client_fd, &_masterfds);
		_fdmax = std::max(_fdmax, client_fd);
	//	delete client;
	}
	std::cout << "\tClient connected\n";
}

int			Webserver::start_listener(Server server) {
	return server.start_listening();
}


void			Webserver::disconnect(int client_fd)
{
	if (_clients.count(client_fd)) 
	{
		delete _clients[client_fd].get_request();
		_clients.erase(client_fd); //+delete ?
		if (client_fd == _fdmax)
			_fdmax--;
		FD_CLR(client_fd, &_masterfds);
		std::cout << "\tClient disconnected: " << client_fd << '\n';
	}
}

void			Webserver::CGI_input(int fd, Response *response) {

	std::cout << "test cgi input : " << response->code << '\n';
	if (_cgi_input[fd]->status != Waiting_CGI_Input)
		return ;
	std::cout << "Sending CGI input :" << &response->request_body[response->cgi_input_offset] << " et " << response->cgi_input_offset << '\n';
	int rd = 0;
	if ((rd = write(fd, &response->request_body[response->cgi_input_offset], response->request_body.size() - response->cgi_input_offset)) < 0)
		return ;
	response->cgi_input_offset += rd;
	if (response->cgi_input_offset >= response->request_body.size())
	{
		close(fd);
		_cgi_input.erase(fd);
		FD_CLR(fd, &_masterfds);
		response->status = Waiting_CGI_Output;
		_fdmax = fd == _fdmax ? fd - 1 : _fdmax;
	}
}

void			Webserver::CGI_output(int fd, Response *response) {
	if (!_cgi_output.count(fd) || _cgi_output[fd]->status != Waiting_CGI_Output)
		return ;
	std::cout << "Receiving CGI output\n";
	int rd = 0;
	char buf[BUFFER_SIZE];
	if ((rd = read(fd, buf, sizeof(buf))) < 0)
	{
		close(fd);
		_cgi_output.erase(fd);
		FD_CLR(fd, &_masterfds);
		response->status = Ready;
		_fdmax = _fdmax == fd ? _fdmax - 1 : _fdmax;
		treat_Output(*response, response->body);
		response->format(0);
		return ;
	}
	buf[rd] = 0;
	response->body += buf;
	if (waitpid(response->cgi_child, NULL, WNOHANG)  ) //child process is dead
	{
		close(fd);
		_cgi_output.erase(fd);
		FD_CLR(fd, &_masterfds);
		response->status = Ready;
		_fdmax = _fdmax == fd ? _fdmax - 1 : _fdmax;
		treat_Output(*response, response->body);
		response->format(0);
	}
}



// TODO : n'a pas sa place ici  ######################################################################################################################

std::vector<Server> Webserver::parseConfig(const char *filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw Webserver::WebserverException("Could not open config file\n");
    }
    std::string tmp;
    std::vector<ConfigServer> servers;
    servers.push_back(ConfigServer());
    int count = 0;
    bool err = true;
	int line = 1;
    std::vector<Server> tab;


    while (std::getline(file, tmp))
    {
        delete_comments(tmp);
        std::vector<std::string> split = ft_split(tmp);
        if (split.size() && split[0] == "server_name:")
            err = set_server_name(servers[count], split);
		else if (split.size() && split[0] == "host:")
			err = set_host(servers[count], split);
        else if (split.size() && split[0] == "port:")
            err = set_port(servers[count], split);
        else if (split.size() && split[0] == "error_page:")
            err = set_error_page(servers[count], split);
        else if (split.size() && split[0] == "max_client_body_size:")
            err = set_max_body_size(servers[count], split);
        else if (split.size() && split[0] == "location:")
            err = set_location(servers[count], split, file, tmp);
		else if (split.size() && split[0] == "root:")
			err = set_root(servers[count], split);
        else if (split.size() && split[0] == "---")
        {
            //check si serveur est actuellement complet sinon exception
            if (!servers[count].port)
                throw Webserver::WebserverException(message_error("Server was not setup correctly", line));
            servers.push_back(ConfigServer());
            count++;
        }
        else if (split.size() || !err)
            throw Webserver::WebserverException(message_error("Config file was not formatted correctly", line));
		line++;
    }


    if (!servers[count].port)
        throw Webserver::WebserverException(message_error("Server was not setup correctly", line));
    //check si serveur est actuellement complet sinon exception

    for (size_t i = 0; i < servers.size(); i++)
	{
		bool skip = false;
		// doublon de lecture 
		for (size_t j = 0; j < i; j++)
		{
			std::cout << "get port j " << j << "et i " << i << "server size="<< servers.size()<< std::endl;
			if (tab[j].get_port() == servers[i].port)
			{
				skip = true;
				tab[j].get_config().push_back(servers[i]);
			std::cout << "INSIDE  get port j " << j << "et i " << i << std::endl;
			}

		}
		std::cout << "get port " << servers[i].port << "et i " << i << std::endl;
		if (!skip)
        	tab.push_back(Server(this, servers[i].port, servers[i]));
	}
    return tab;
}

bool					Webserver::setup_config(const char *filename)
{
	std::vector<Server> tmp;
	try {
		tmp = parseConfig(filename);
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	for (size_t i = 0; i < tmp.size(); i++)
	{
		int fd_tmp = 0;
		try {
			for (size_t j = 0; j < i; j++)
			{

				if (tmp[j].get_port() == tmp[i].get_port())
				{
					tmp[i].set_fd(tmp[j].get_fd());
					fd_tmp = 1;
				}
			}
			fd_tmp = fd_tmp ? fd_tmp : start_listener(tmp[i]);
		}
		catch (const std::exception &e) {
			std::cerr << "Could not setup one or more server(s) : " << e.what() << '\n';
			continue ;
		}
		_servers[fd_tmp] = tmp[i];
		FD_SET(fd_tmp, &_masterfds);
		_fdmax = std::max(_fdmax, fd_tmp);
	}

	int count_serv = 0;
	for (std::map<int, Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
		count_serv += it->second.get_config().size();
	std::cout << "Nombre de serveurs : " << count_serv << " avec " << "\n\n";
	if (count_serv != _servers.size())
		return (_servers.empty());
	return (!_servers.empty());
}

//	WEBSERVER EXCEPTION ##############################################################################################################################


Webserver::WebserverException::WebserverException(const std::string &msg) throw() : _msg(msg) {}

Webserver::WebserverException::WebserverException(int code) throw() : _code(code) {}

Webserver::WebserverException::~WebserverException() throw() {}

int	Webserver::WebserverException::get_code(void) const { return _code; }

const char *Webserver::WebserverException::what(void) const throw() {
	return _msg.c_str();
}


// AUTRE #########################################


void		Webserver::list_clients(void) {
	std::cout << "\tList of clients (size " << _clients.size() << ") : \n";
		std::map<int, Client>::iterator it = _clients.begin(), ite = _clients.end();
		for (; it != ite; it++)
		 	std::cout << '\t' <<it->first << " | " << (it->second.get_request() ? "   Request" : "No request") << " | " << it->second.responses.size() << " responses\n";
		std::cout << '\n';
}

void		Webserver::list_servers(void) {
	std::cout << "\n\tList of servers (size " << _servers.size() << ") : \n";
		std::map<int, Server>::iterator it = _servers.begin(), ite = _servers.end();
		for (; it != ite; it++)
			if (it->first)
			{
				int count = 0;
				for (std::map<int, Client>::iterator it_c = _clients.begin(); it_c != _clients.end(); it_c++)
					count += it_c->second.get_server() == &it->second ? 1 : 0;
				std::cout << '\t' << it->first << " | " << it->second.get_port() << " | " << count << " client(s)\n";
			}
		std::cout << '\n';
}

void		Webserver::command_line(void) {
	char buf[50];
	int rd = 0;
	if ((rd = read(STDIN_FILENO, buf, sizeof(buf))) == -1)
	{
		std::cerr << "Error reading input\n";
		return;
	}
	buf[rd - 1] = 0;
	if (!std::strcmp(buf, "clients")) //TODO : enlever le client qui se met au FD 0
		list_clients();
	else if (!std::strcmp(buf, "servers"))
		list_servers();
	else if (!std::strncmp(buf, "request", 7))
	{
		std::vector<std::string> split = ft_split(buf);
		if (split.size() != 2)
		{
			std::cerr << "Formatting of command request is wrong\n";
			return ;
		}
		int fd = std::atoi(split[1].c_str());
		if (_clients.count(fd) && _clients[fd].get_request())
			_clients[fd].get_request()->printvalue();
	}
	else if (!std::strcmp(buf, "locations"))
	{
		for (std::map<int, Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
		{
			std::cout << "\tServer : " << it->second.get_port() << " on fd " << it->second.get_fd() << '\n';
			for (int j = 0; j < it->second.get_config().size(); j++)
			{
				std::cout << "\t\tConfig : " << j << '\n';
				for (int k = 0; k < it->second.get_config()[j].locations.size(); k++)
				{
					std::cout << "\t\t\tLocation : " << k << '\n';
					it->second.get_config()[j].locations[k].print();
				}
			}
		}
	}
	else if (!std::strncmp(buf, "response", 8))
	{
		/*std::vector<std::string> split = ft_split(buf);
		if (split.size() != 2)
		{
			std::cerr << "Formatting of command response is wrong\n";
			return ;
		}
		int fd = std::atoi(split[1].c_str());
		if (_clients.count(fd) && _clients[fd].get_response())
			_clients[fd].get_response()->print();*/
	}
	else if (!std::strcmp(buf, "fd_max"))
		std::cout << _fdmax << '\n';
	else if (!std::strcmp(buf, "CGI"))
	{
		/*std::cout << "List of CGI (size " << _cgi_output.size() << ") :\n";
		for (std::map<int, CGI>::iterator it = _cgi_output.begin() ; it != _cgi_output.end() ; it++)
		{
			std::cout << it->first << " | " << it->second._client->get_fd() << '\n';
		}*/
	}
	else if (!std::strcmp(buf, "CGI input"))
		std::cout << _cgi_input.size() << '\n';
	else if (!std::strcmp(buf, "CGI output"))
		std::cout << _cgi_output.size() << '\n';
	else if (!std::strncmp(buf, "write", 5))
	{
		std::vector<std::string> split = ft_split(buf);
		if (split.size() != 2)
		{
			std::cerr << "Formatting of command write is wrong\n";
			return ;
		}
		int fd = std::atoi(split[1].c_str());
		write_to_client(fd);
	}
	else
		std::cerr << "\tCommand was not found\n";
	
}
