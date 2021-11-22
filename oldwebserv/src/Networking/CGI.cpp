# include "CGI.hpp"

void		treat_Output(Response &response, std::string buffer)
{
	std::string line;
	std::stringstream ss;
	ss << buffer;

	std::getline(ss, line);
	line.erase(0, sizeof("Content-Type:"));
	// response.get_header().get_fields()["Content-Type"] = line;
	response.insert_header("Content-Type", line);
	response.body = "";
	if (ss)
	{
		std::getline(ss, line); //ligne vide
		while (ss)
		{
			std::getline(ss, line);
			response.body += line;
			//line += "\n";
		}
		// response.body(line);
	}

}

std::string CGI_getOutput(int fd)
{
	char buf[2048];
	int rd = 0;
	std::string result;
	std::memset(buf, 0, sizeof(buf));
	while ((rd = read(fd, buf, sizeof(buf))) > 0)
	{
		result.append(buf);
		std::memset(buf, 0, sizeof(buf));
	}
	return result;
}

bool CGI_check_php(std::string path)
{
	if (path.size() <= 4)
		return false;
	if (path.substr(path.size() - 4) == ".php")
		return true;
	return false;
}

std::vector<char *>		str_tab_to_char(const std::vector<std::string> &tab)
{
	std::vector<char *> result;
	size_t count = 0;
	while (count < tab.size())
	{
		result.push_back(const_cast<char *>(tab[count].c_str()));
		count++;
	}
	result.push_back(0);
	return result;
}

std::vector<std::string>    CGI_getenv(std::string path, const Request *request)
{
    std::vector<std::string> tab;
	std::pair<bool, std::string> tmp;
	if (request && (tmp = request->env_querystring()).first)
		tab.push_back(tmp.second);
	if (request && (tmp = request->env_method()).first)
		tab.push_back(tmp.second);
	//if ((tmp = client.get_request()->env_Content_Length()).first)
	//	tab.push_back(tmp.second);
	//if ((tmp = client.get_request()->env_Content_Type()).first)
	//	tab.push_back(tmp.second);
	tab.push_back("SCRIPT_FILENAME=" + path);
	tab.push_back("REDIRECT_STATUS=200");
	tab.push_back("SERVER_PROTOCOL=HTTP/1.1");
	tab.push_back("PATH_INFO=" + path);

	if (request && request->body.size())
	{
		std::stringstream ss;
		std::string tmp;
		ss << request->body.size();
		ss >> tmp;
		tmp.insert(0, "CONTENT_LENGTH=");
		tab.push_back(tmp);
	}
    
    // std::stringstream ss;
    // ss.clear();
    // ss << client.get_port();
    // std::string str_tmp;
    // ss >> str_tmp;
    // str_tmp.insert(0, "SERVER_PORT=");
	// tab.push_back(str_tmp);

    /*for (int i = 0; i < tab.size() ; i++)
		std::cout << "tab : " << tab[i] << '\n';
	*/
    return tab;//str_tab_to_char(tab);
}