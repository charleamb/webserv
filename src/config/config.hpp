#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <sys/socket.h>
#include <sys/types.h>
// #include <sys/epoll.h>
#include <poll.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include <cstring>
#include <ctime>

#include <fstream>
#include <sstream>

#include <fstream>

#include <map>
#include <vector>

#include <pthread.h> //à enlever lorsque le client ne sera plus multithread

#define HTTP_VERSION "HTTP/1.1"
#define SERVER_VERSION "42Server/1.0.0"

#include "../networking/Server.hpp"

#include "../utils/utils.hpp"
#include "Location.hpp"

bool set_server_name(ConfigServer &server, const std::vector<std::string> &split);
bool set_error_page(ConfigServer &server, const std::vector<std::string> &split);
bool set_port(ConfigServer &server, const std::vector<std::string> &split);
bool set_host(ConfigServer &server, const std::vector<std::string> &split);
bool set_root(ConfigServer &server, const std::vector<std::string> &split);
bool set_max_body_size(ConfigServer &server, const std::vector<std::string> &split);
bool set_location(ConfigServer &server, const std::vector<std::string> &split, std::ifstream &file, std::string &tmp);

#endif
