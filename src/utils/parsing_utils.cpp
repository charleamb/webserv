#include "utils.hpp"

std::string combine_split(std::vector<std::string> split, int index)
{
    std::string result;
    while (index < split.size())
    {
        result += split[index];
        if (index + 1 < split.size())
            result += " ";
        index++;
    }
    return result;
}

std::string get_extension(std::string str)
{
    if (!str.find('.') || str.find('.') == str.size() - 1)
        return ("");
    return str.substr(str.find('.') + 1, str.size());
}

bool str_is_digit(const std::string &tmp)
{
    for (size_t i = 0; i < tmp.size(); i++)
        if (!std::isdigit(tmp[i]))
            return false;
    return true;
}

void delete_comments(std::string &tmp)
{
    if (tmp.find('#') != std::string::npos)     //Supprime les commentaires
        tmp.erase(tmp.find('#'));
}

std::vector<std::string> ft_split(std::string str, std::string charset)
{
    int i = 0;
    std::vector<std::string> tmp;
    std::string::iterator it = str.begin(), ite = str.end();
    while (it != ite)
    {
        if (!std::isspace(*it) && charset.find(*it) == std::string::npos && (!i || (std::isspace(*(it - 1)) || charset.find(*(it - 1)) != std::string::npos)))
            tmp.push_back(std::string(it, it + length_split(&str.data()[i], charset)));
        it++;
        i++;
    }
    return tmp;
}

size_t length_split(const char *str, const std::string charset)
{
    size_t count = 0;
    while (str[count])
    {
        if (std::isspace(str[count]) || charset.find(str[count]) != std::string::npos)
            return count;
        count++;
    }
    return count;
}

std::string message_error(std::string msg, int line)
{
    std::stringstream ss;
    std::string tmp;
    ss << line;
    msg += ": line ";
    ss >> tmp;
    msg += tmp;
    return msg;
}

bool    is_inside_root(std::string path, std::string root)
{
    std::vector<std::string> split = ft_split(path, "/");
    int delta = 1;
    for (std::vector<std::string>::iterator it = split.begin(); it < split.end(); it += delta)
    {
        if (split[0] == "..")
            return false;
        if (!delta)
            delta = 1;

        if (*it == "..")
        {
            split.erase(it);
            split.erase(it - 1);
            it = split.begin();
            delta = 0;
            continue;
        }
    }
    return (splitToPath(split).substr(0, root.size()) == root);
    //      /var/www/html/../html/
}