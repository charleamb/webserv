#include "server.hpp"

#define HTTP_VERSION "HTTP/1.1"
#define SERVER_VERSION "42Server/1.0.0"

bool send_file(int socket, const char *filename)
{
    std::ifstream fd(filename);
    if (!fd.is_open())
    {
        std::cerr << "Could not open file\n";
        return false;
    }
    std::filebuf *buf = fd.rdbuf();
    std::streamsize size = buf->in_avail();
    // a finir
}


bool send_header(int socket, int code, const char *msg)
{
    std::string tmp = getStatus(code, msg);
    tmp += getServer();
    tmp += getDate();
    if (send(socket, tmp.data(), tmp.size(), 0) < 0)
    {
        std::cerr << "Could not send header\n";
        return false;
    }
    return true;
}