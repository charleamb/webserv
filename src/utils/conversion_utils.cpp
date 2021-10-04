#include "utils.hpp"

std::string itostr(int n) {
	std::ostringstream str;
	str << n;
	return str.str();
}

int			strtoi(const std::string &s) {
	std::stringstream ss;
    int n;
	// ss.clear();
    ss << s;
    ss >> n;
	return n;
}

std::string splitToPath(std::vector<std::string> split)
{
    if (!split.size())
        return ("/");
    std::string result;

    for (size_t i = 0; i < split.size(); i++)
    {
        result += "/" + split[i];
    }
    return result;
}

std::string strToPath(std::string str)
{
    //std::cout << "from " << str;
    std::vector<std::string> tab = ft_split(str, "/");
    std::string result = splitToPath(tab);

    if (tab.size() && str[str.size() - 1] == '/')
        result += "/";
    //std::cout << " to " << result << '\n';
    return result;
}