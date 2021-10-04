#ifndef CONFIGSERVER_HPP
#define CONFIGSERVER_HPP

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


#include "Location.hpp"

class ConfigServer
{
public:
    ConfigServer();
    ConfigServer(const ConfigServer &ref);
    ~ConfigServer();

    int                         port;
    std::string                 host;
    std::string                 root;
    std::map<int, std::string>  error_pages;
    std::vector<Location>       locations;            //tableau des 'locations'
    std::vector<std::string>    server_names;
    int                         client_max_body_size; //-1 if no limit
    
    Location                    *has_location(std::string host);
};


#endif
