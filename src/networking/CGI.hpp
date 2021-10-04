#ifndef CGI_HPP
#define CGI_HPP

#include <vector>
#include <utility>
#include <string>
#include "Client.hpp"

/* Utils */
std::vector<char *>		str_tab_to_char(const std::vector<std::string> &tab);


void		treat_Output(Response &response, std::string buffer);

std::vector<std::string>    CGI_getenv(std::string path, const Request *request);
std::string CGI_getOutput(int fd);
bool CGI_check_php(std::string path);


#endif