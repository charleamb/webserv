#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>

//#define PORT 35497

int main(int ac, char **av)
{
    int PORT = 4242;
    if (ac == 2)
        PORT = std::atoi(av[1]);
     
    int online = 1;

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("192.168.1.61");
    //server_address.sin_addr.s_addr = inet_addr("192.168.1.5");


    int socket_fd = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(PORT);
    client_address.sin_addr.s_addr = inet_addr("192.168.1.3");
    bind(socket_fd, (struct sockaddr*)&client_address, sizeof(client_address));
    char read_buf[100];
    char send_buf[100];
    fd_set readfds;

    std::cout << "Debut chat : " << socket_fd << '\n';
    //FD_SET(socket_fd, &tmp);

    while (online)
    {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(socket_fd, &readfds);

        int sret = 0;
        if ((sret = select(FD_SETSIZE, &readfds, NULL, NULL, NULL)) < 0)
        {
            std::cerr << "Error 2\n";
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
                        std::cout << "je close\n";
                        close(socket_fd);
                        return (0);
                    }
                    if (sendto(socket_fd, send_buf, sret - 1, 0, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
                    {
                        std::cout << "Error send\n";
                        return 2;
                    }
                    
                }
                else
                {
                    memset(read_buf, 0, sizeof(read_buf));
                    sret = read(i, read_buf, sizeof(read_buf));
                    if (sret < 0)
                    {
                        std::cout << "Error read\n";
                        return 3;
                    }
                    if (std::string(read_buf) == "exit\n")
                    {
                        std::cout << "on close\n";
                        sleep(1);
                        close(socket_fd);
                        return (0);
                    }
                    std::cout << '\t' << read_buf << " avec " << (int)read_buf[sret - 1] << '\n';
                    //std::cout << "Lecture\n";
                }
            }
        }
    }
    close(socket_fd);

    return 0;
}