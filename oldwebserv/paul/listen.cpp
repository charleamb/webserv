#include "server.hpp"
#define QUEUE_SIZE 10

#include <cstdlib> // pour le atoi

bool connected(int socket) // Vérifie si le client est toujours connecté
{
    char buf;
    if (recv(socket, &buf, 1, MSG_PEEK) <= 0)
        return false;
    return true;
}

bool manageClient(int client) // Lit le message du client
{
    std::cout << "client : " << client << '\n';
    if (!connected(client))
    {
        std::cout << "Client disconnected\n";
        return false;
    }

    char read_buf[100];
    memset(read_buf, 0, sizeof(read_buf));
    int sret = recv(client, read_buf, sizeof(read_buf), 0);
    std::cout << read_buf;
    return true;
}

int main(int ac, char **av) // Va écouter tous les file descriptor à surveiller
{
    int PORT = 8080;
    if (ac == 2)
        PORT = std::atoi(av[1]);
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int listen_fd = socket(PF_INET, SOCK_STREAM, 6);
    if (listen_fd < 0)
    {
        std::cerr << "Could not create listener socket\n";
        return 2;
    }

    if (bind(listen_fd, (struct sockaddr*) &server_address, sizeof(server_address)) < 0)
    {
        std::cerr << "Can not listen on port " << PORT << '\n';
        return 1;
    }

    if (listen(listen_fd, QUEUE_SIZE))
    {
        std::cerr << "Could not set the socket on passive mode\n";
        return 3;
    }
    
    sockaddr ret;
    struct sockaddr_in *tmp = (sockaddr_in*)&ret;
    socklen_t length = 0;
    int client = 0;

    /* Un tableau des file descriptors à surveiller, au début il n'écoute que le listener donc un tableau d'une seule case */
    struct pollfd *fd_to_watch = new struct pollfd[1];
    /**/

    /* La taille du tableau, à maintenir à jour */
    size_t         nb_fd = 1;
    /**/

    fd_to_watch[0].fd = listen_fd; // Le fd à surveiller
    fd_to_watch[0].events = POLLIN; // On surveille l'entrée de n'importe quelle data
    fd_to_watch[0].revents = 0; // La value qui va être return par poll()

    while (1)
    {
        int pret = poll(fd_to_watch, nb_fd, -1); // Va return lorsque de la data sera recue
        if (pret)
        {
            for (int i = 0; i < nb_fd; i++)
            {
                if ((fd_to_watch[i].revents & POLLIN) == POLLIN)
                {
                    if (fd_to_watch[i].fd == listen_fd)     // New client
                    {
                        if ((client = accept(listen_fd, &ret, &length)) < 1)
                            std::cerr << "Error accepting client\n";
                        else
                        {
                            addOne(&fd_to_watch, &nb_fd, client);
                            std::cout << "New client\n";
                        }
                    }
                    else if (!manageClient(fd_to_watch[i].fd))
                        deleteOne(&fd_to_watch, &nb_fd, fd_to_watch[i].fd);
                }
            }
        }
    }
    for (int i = 0; i < nb_fd; i++)
        close(fd_to_watch[i].fd);
    delete[] fd_to_watch;
    return 0;
}