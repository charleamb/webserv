#ifndef FT_NETWORKING_HPP
# define FT_NETWORKING_HPP
# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <unistd.h>
# include <vector>
# include <map>
# include <string>
# include <fcntl.h>
# include <sys/select.h>
# include <cstdlib>
# include <cstring>
# include <sys/errno.h>
# include <cstddef>
# define SIZEOF_SOCKADDR_IN sizeof(sockaddr_in)
# define ERROR(x) (x == -1 ? 1 : 0)
# define BACKLOG 100
# define NB_OF_STATUS_CODE 64
# define SPACE ' '
# define CRLF "\r\n"
# endif