#include "server.hpp"

std::string getDate(void)
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[40];
    time(&rawtime);
    timeinfo = gmtime(&rawtime);

    strftime(buffer, 40, "Date: %a, %d %b %G %X GMT\n", timeinfo);
    return std::string(buffer);
}

std::string getServer(void)
{
    std::string tmp = "Server :";
    tmp += SERVER_VERSION;
    tmp += "\n";
    return tmp;
}

std::string getStatus(int code, std::string msg)
{
    std::string header = HTTP_VERSION;
    std::stringstream ss;
    ss << code;

    header += " ";
    ss >> header;
    header += " " + msg + "\n";
    return header;
}
