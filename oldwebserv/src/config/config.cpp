#include "config.hpp"
#include "../utils/utils.hpp"
#include "../networking/Webserver.hpp"

#include <vector>

bool set_server_name(ConfigServer &server, const std::vector<std::string> &split)
{
    size_t count = 1;
    while (count < split.size())
    {
        server.server_names.push_back(split[count]);
        count++;
    }
    return true;
}

bool set_error_page(ConfigServer &server, const std::vector<std::string> &split)
{
    if (!str_is_digit(split[1]))
        return false;
    server.error_pages[std::atoi(split[1].c_str())] = split[2];
    return true;
}

bool set_port(ConfigServer &server, const std::vector<std::string> &split)
{
    if (!str_is_digit(split[1]))
        return false;
    server.port = std::atoi(split[1].c_str()); //check si le port est valable
    return true;
}

bool set_max_body_size(ConfigServer &server, const std::vector<std::string> &split)
{
    if (!str_is_digit(split[1]))
        return false;
    server.client_max_body_size = std::atoi(split[1].c_str());
    return true;
}

bool set_host(ConfigServer &server, const std::vector<std::string> &split)
{
    if (split.size() <= 1)
        return false;
    server.host = split[1];
    return true;
}

bool set_root(ConfigServer &server, const std::vector<std::string> &split)
{
    if (split.size() <= 1)
        return false;
    server.root = std::string(split[1]);
    return true;
}

bool set_location(ConfigServer &server, const std::vector<std::string> &split, std::ifstream &file, std::string &tmp)
{
    if (split.size() == 1)
        return false;
    int index = server.locations.size();
    server.locations.push_back(Location());
    server.locations[index].route = split[1];
    if (server.locations[index].route.size() > 1 && server.locations[index].route[server.locations[index].route.size() - 1] == '/')
        server.locations[index].route.pop_back();
    std::vector<std::string> loc_split;
    while (std::getline(file, tmp) && std::isspace(tmp[0]))
    {
        delete_comments(tmp);
        loc_split = ft_split(tmp);
        if (loc_split.size() && loc_split[0] == "HTTP_methods:")
        {
            for (size_t i = 1; i < loc_split.size(); i++)
            {
                if (loc_split[i] == "GET")
                    server.locations[index].methods |= GET_d;
                else if (loc_split[i] == "POST")
                    server.locations[index].methods |= POST_d;
                else if (loc_split[i] == "DELETE")
                    server.locations[index].methods |= DELETE_d;
                else
                    return false;
            }
        }
        else if (loc_split.size() && loc_split[0] == "redirect:")
        {
            if (!str_is_digit(loc_split[1]) || server.locations[index].redirect || loc_split.size() < 3)
                return false;
            server.locations[index].redirect = new std::pair<int, std::string>(std::atoi(loc_split[1].c_str()), loc_split[2]);
        }
        else if (loc_split.size() && loc_split[0] == "root:")
        {
            if (server.locations[index].root || loc_split.size() < 2)
                return false;
            server.locations[index].root = new std::string(loc_split[1]);
            if ((*server.locations[index].root)[server.locations[index].root->size() - 1] != '/')
                *(server.locations[index].root) += '/';
        }
        else if (loc_split.size() && loc_split[0] == "directory_listing:")
        {
            if (loc_split.size() < 2)
                return false;
            if (loc_split[1] == "true")
                server.locations[index].listing = true;
        }
        else if (loc_split.size() && loc_split[0] == "index:")
        {
            if (loc_split.size() < 2 || server.locations[index].index)
                return false;
            server.locations[index].index = new std::string(loc_split[1]);
        }
        else if (loc_split.size() && loc_split[0] == "upload-files:")
        {
            if (loc_split.size() < 3 || server.locations[index].upload_route)
                return false;
            if (loc_split[1] == "on")
                server.locations[index].upload_route = new std::string(loc_split[2]);
        }
        else if (loc_split.size() > 1 && loc_split[0] == "cgi-extension:")
        {
            for (size_t j = 1; j < loc_split.size(); j++)
            {
                std::cout << "cgi-extension: " << loc_split[j] << '\n';
                server.locations[index].cgi_extension.push_back(loc_split[j]);
            }
        }
    }
    return true;
}
