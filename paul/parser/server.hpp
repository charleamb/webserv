#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <poll.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#include <cstring>
#include <ctime>

#include <fstream>
#include <sstream>

#include <fstream>

#include <map>
#include <vector>

#include <pthread.h> //à enlever lorsque le client ne sera plus multithread

#define HTTP_VERSION "HTTP/1.1"
#define SERVER_VERSION "42Server/1.0.0"

#define GET_d 1
#define POST_d 2
#define DELETE_d 4

typedef struct request_s
{
    int type; //1 : GET     2 : POST    3 : DELETE      0 : UNKNOWN
    std::string host;
    std::string body;
    std::string UserAgent;
    std::string Accept;
    std::string AcceptLanguage;
    std::string AcceptEncoding;
    bool connection; //true : keep-alive   false : close
    bool complete; //request is complete ? 
}               request_t;

typedef struct location_s //partie CGI à ajouter
{
    char                        methods;         // opérations binaires : GET_d, POST_d, DELETE_d
    std::string                 route;           //le chemin
    std::pair<int, std::string> *redirect;       //redirection ? pair<code de redirection, chemin de redirection>
    std::string                 *root;           //si non-NULL, le chemin à partir lequel chercher les fichiers
    bool                        listing;         //directory listing ?
    std::string                 *directory_page; //si non-NULL, chemin vers fichier à envoyer dans le cas où requete est un dossier
    std::string                 *upload_route;   //si non-NULL, chemin vers lequel les fichiers seront uploadés
}               location_t;


typedef struct ConfigServer_s
{
    std::string                 *root;
    std::map<int, std::string>  error_pages;
    std::vector<location_t>     locations;            //tableau des 'locations'
    std::vector<std::string>    server_names;
    int                         client_max_body_size; //-1 if no limit
    
}               ConfigServer;

typedef int socket_t;

typedef struct Server_s
{
    int                         port;
    int                         listen_fd;

    ConfigServer                config;
    std::vector<socket_t>       clients;
}                               Server_t;


std::string getStatus(int code, std::string msg);
std::string getServer(void);
std::string getDate(void);

bool send_header(int socket, int code, const char *msg);
bool send_file(int socket, const char *filename);

void deleteOne(struct pollfd **tab, size_t *size, int fd_to_delete);
void addOne(struct pollfd **tab, size_t *size, int fd_to_add);

#endif