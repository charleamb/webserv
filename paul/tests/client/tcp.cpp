#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <fstream>
#include <cstdlib>

#define DEFAULT_PORT 80

bool connected(int socket)
{
    char buf;
    if (recv(socket, &buf, 1, MSG_PEEK) <= 0)
        return false;
    return true;
}

int main(int ac, char **av)
{
    const char *IP = "0.0.0.0";
    int PORT = DEFAULT_PORT;
    if (ac >= 2)
        IP = av[1];
    if (ac >= 3)
        PORT = std::atoi(av[2]);
    int online = 1;

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    //server_address.sin_addr.s_addr = inet_addr("192.168.1.61");
    server_address.sin_addr.s_addr = inet_addr(IP);

    int socket_fd = socket(PF_INET, SOCK_STREAM, 6);

    if (connect(socket_fd, (struct sockaddr*) &server_address, sizeof(server_address)) == -1)
    {
        std::cerr << "Error 1\n";
        return 1;
    }
    char read_buf[100];
    char send_buf[100];
    fd_set readfds;

    std::cout << "Debut chat : " << socket_fd << '\n';

    std::ofstream file("response.server", std::ios_base::out);

    while (online)
    {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(socket_fd, &readfds);

        int sret = 0;
        if ((sret = select(FD_SETSIZE, &readfds, NULL, NULL, NULL)) < 0)
        {
            std::cerr << "Error 2\n";
            file.close();
            close(socket_fd);
            return 2;
        }
        for (int i = 0;i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &readfds)){
                if (!i)
                {
                    memset(send_buf, 0, sizeof(send_buf));
                    sret = read(i, send_buf, sizeof(send_buf));
                    if (std::string(send_buf) == "exit\n")
                    {
                        file.close();
                        close(socket_fd);
                        return (0);
                    }
                    if (send(socket_fd, send_buf, sret, 0) < 0)
                    {
                        std::cout << "Error send\n";
                        file.close();
                        close(socket_fd);
                        return 2;
                    }
                }
                else
                {
                    if (!connected(socket_fd))
                    {
                        std::cout << "Disconnected\n";
                        close(socket_fd);
                        file.close();
                        return 0;
                    }
                    memset(read_buf, 0, sizeof(read_buf));
                    sret = recv(i, read_buf, sizeof(read_buf), 0);
                    if (sret < 0)
                    {
                        std::cout << "Error read\n";
                        file.close();
                        close(socket_fd);
                        return 3;
                    }
                    std::cout << read_buf;
                    file << read_buf;
                }
            }
        }
    }
    file.close();
    close(socket_fd);

    return 0;
}