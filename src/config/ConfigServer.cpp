#include "ConfigServer.hpp"

ConfigServer::ConfigServer()
:port(0),
host(std::string("")),
root(std::string("")),
error_pages(std::map<int, std::string>()),
locations(std::vector<Location>()),
server_names(std::vector<std::string>()),
client_max_body_size(-1)
{}

ConfigServer::ConfigServer(const ConfigServer &ref)
: port(ref.port),
host(ref.host),
root(ref.root),
error_pages(ref.error_pages),
locations(ref.locations),
server_names(ref.server_names),
client_max_body_size(ref.client_max_body_size)
{}

ConfigServer::~ConfigServer()
{}

Location       *ConfigServer::has_location(std::string path)
{
    std::vector<Location>::iterator it = locations.begin(), ite = locations.end();

    while (it != ite)
    {
        std::string tmp = it->route;
        if (tmp[it->route.size() - 1] != '/')
            tmp.push_back('/');

        if (path.find(tmp.c_str(), 0, tmp.length()) != std::string::npos)
            return it.base(); // on a trouve la location
        it++;
    }
    return 0;
}
