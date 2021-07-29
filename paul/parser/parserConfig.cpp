#include "server.hpp"
#include "utils.hpp"

#include <vector>

size_t length_split(const char *str)
{
    size_t count = 0;
    while (str[count])
    {
        if (std::isspace(str[count]))
            return count;
        count++;
    }
    return count;
}

std::vector<std::string> ft_split(std::string str)
{
    int i = 0;
    std::vector<std::string> tmp;
    std::string::iterator it = str.begin(), ite = str.end();
    while (it != ite)
    {
        if (!std::isspace(*it) && (!i || std::isspace(*(it - 1))))
            tmp.push_back(std::string(it, it + length_split(&str.data()[i])));
        it++;
        i++;
    }
    return tmp;
}

bool set_server_name(ConfigServer &server, const std::vector<std::string> &split)
{
    int count = 1;
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
    //if (access(split[2].c_str(), F_OK | R_OK) < 0)
    //    return false;
    server.error_pages[std::atoi(split[1].c_str())] = split[2];
    return true;
}

bool set_max_body_size(ConfigServer &server, const std::vector<std::string> &split)
{
    if (!str_is_digit(split[1]))
        return false;
    server.client_max_body_size = std::atoi(split[1].c_str());
    return true;
}

bool set_location(ConfigServer &server, const std::vector<std::string> &split, std::ifstream &file, std::string &tmp)
{
    location_t newLocation;
    if (split.size() == 1)
        return false;
    newLocation.route = split[1];
    std::vector<std::string> loc_split;
    while (std::getline(file, tmp) && std::isspace(tmp[0]))
    {
        loc_split = ft_split(tmp);
        if (loc_split.size() && loc_split[0] == "HTTP_methods:")
        {
            for (int i = 1; i < loc_split.size(); i++)
            {
                if (loc_split[i] == "GET")
                    newLocation.methods |= GET_d;
                else if (loc_split[i] == "POST")
                    newLocation.methods |= POST_d;
                else if (loc_split[i] == "DELETE")
                    newLocation.methods |= DELETE_d;
                else
                    return false;
            }
        }
        else if (loc_split.size() && loc_split[0] == "redirect:")
        {
            if (!str_is_digit(loc_split[1]) || newLocation.redirect || loc_split.size() < 3)
                return false;
            newLocation.redirect = new std::pair<int, std::string>(std::atoi(loc_split[1].c_str()), loc_split[2]);
        }
        else if (loc_split.size() && loc_split[0] == "root:")
        {
            if (newLocation.root || loc_split.size() < 2)
                return false;
            newLocation.root = new std::string(loc_split[1]);
        }
        else if (loc_split.size() && loc_split[0] == "directory_listing:")
        {
            if (loc_split.size() < 2)
                return false;
            if (loc_split[1] == "true")
                newLocation.listing = true;
        }
        else if (loc_split.size() && loc_split[0] == "directory_page:")
        {
            if (loc_split.size() < 2 || newLocation.directory_page)
                return false;
            newLocation.directory_page = new std::string(loc_split[1]);
        }
        else if (loc_split.size() && loc_split[0] == "upload-files")
        {
            if (loc_split.size() < 3 || newLocation.upload_route)
                return false;
            if (loc_split[1] == "true")
                newLocation.upload_route = new std::string(loc_split[2]);
        }
    }
    return true;
}

int main()
{
    std::ifstream file("config.conf");
    std::string tmp;
    ConfigServer server;
    while (std::getline(file, tmp))
    {
        if (tmp.find('#') != std::string::npos)     //Supprime les commentaires
            tmp.erase(tmp.find('#'));
        std::vector<std::string> split = ft_split(tmp);
        if (split.size() && split[0] == "server_name:")
            set_server_name(server, split);
        else if (split.size() && split[0] == "error_page:")
            set_error_page(server, split);
        else if (split.size() && split[0] == "max_client_body_size")
            set_max_body_size(server, split);
        else if (split.size() && split[0] == "location")
            set_location(server, split, file, tmp);
        else if (split.size() && split[0] == "---")
            ;
            
    }
    return (0);
    
}