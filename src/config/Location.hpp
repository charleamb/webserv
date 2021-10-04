#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <cstring>
#include <string>
#include <utility>
#include "../utils/utils.hpp"

#define GET_d 1
#define POST_d 2
#define DELETE_d 4

#include <iostream>

class Location //partie CGI à ajouter
{
public:
    Location();
    Location(const Location& ref);
    ~Location();

    Location                    &operator=(const Location &ref);
    void                        print(void);
    void                        clear(void);

    char                        methods;         // opérations binaires : GET_d, POST_d, DELETE_d
    std::string                 route;           //le chemin
    std::pair<int, std::string> *redirect;       //redirection ? pair<code de redirection, chemin de redirection>
    std::string                 *root;           //si non-NULL, le chemin à partir lequel chercher les fichiers
    bool                        listing;         //directory listing ?
    std::string                 *index; //si non-NULL, chemin vers fichier à envoyer dans le cas où requete est un dossier
    std::string                 *upload_route;   //si non-NULL, chemin vers lequel les fichiers seront uploadés
    std::string                 auth_basic;
    std::string                 limit_except;
    std::vector<std::string>    cgi_extension;
};

#endif