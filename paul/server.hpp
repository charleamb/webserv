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

#include <pthread.h> //à enlever lorsque le client ne sera plus multithread

#define HTTP_VERSION "HTTP/1.1"
#define SERVER_VERSION "42Server/1.0.0"

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

typedef struct route_s //partie CGI à ajouter
{
    std::string route;             //le chemin
    std::string *redirect;         //redirection ?
    std::string *root;             //si non-NULL, le chemin à partir lequel chercher les fichiers
    bool listing;                  //directory listing ?
    std::string *directory_route; //si non-NULL, chemin vers fichier à envoyer dans le cas où requete est un dossier
    std::string *upload_route; //si non-NULL, chemin vers lequel les fichiers seront uploadés
}               route_t;

typedef struct server_s
{
    int         port;
    int         listen_fd;
    std::string root;
    route_t     *routes;        //tableau des 'locations'
    size_t      nb_routes;      //nombre de 'location' dans le tableau
    std::string server_name;
    int         client_max_body_size;
    
}               server_t;

std::string getStatus(int code, std::string msg);
std::string getServer(void);
std::string getDate(void);

bool send_header(int socket, int code, const char *msg);
bool send_file(int socket, const char *filename);

void deleteOne(struct pollfd **tab, size_t *size, int fd_to_delete);
void addOne(struct pollfd **tab, size_t *size, int fd_to_add);

#endif