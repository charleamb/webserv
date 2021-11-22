#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#define PORT 4242

int main(int ac, char **av)
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    //server_address.sin_addr.s_addr = inet_addr("192.168.1.61");

    int listen_fd = socket(PF_INET, SOCK_STREAM, 6);

    std::cout << "bind : " << bind(listen_fd, (struct sockaddr*) &server_address, sizeof(server_address)) << '\n';

    std::cout << "listen_fd : " << listen_fd << "\n";
    int listen_ret = listen(listen_fd, 10);
    //std::cout << "listen_ret : " << listen_ret << '\n';
    std::cout << "Debut chat" << '\n';

    sockaddr ret;
    socklen_t length = 0;
    int client = 0;
    std::cout << (client = accept(listen_fd, &ret, &length)) << '\n';


    fd_set readfds;
    char read_buf[100];
    char send_buf[100];
    std::cout << "client = " << client << '\n';
    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(client, &readfds);

        int sret = 0;
        if ((sret = select(FD_SETSIZE, &readfds, NULL, NULL, NULL)) < 0)
            std::cerr << "Error\n";
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &readfds))
            {
                if (!i)
                {
                    memset(send_buf, 0, sizeof(send_buf));
                    sret = read(i, send_buf, sizeof(send_buf));
                    //send_buf[sret == 0 ? 0 : sret - 1] = '\n';
                    if (std::string(send_buf) == "exit\n")
                    {
                        std::cout << "je close\n";
                        close(client);
                        close(listen_fd);
                        return 0;
                    }
                    //int file_fd = open("test.html", O_RDONLY);
                    //sendfile(client, file_fd, 0, 45);
                    send(client, send_buf, sret, 0);
                }
                else
                {
                    memset(read_buf, 0, sizeof(read_buf));
                    sret = read(i, read_buf, sizeof(read_buf));
                    read_buf[sret] = 0;
                    if (std::string(read_buf) == "exit")
                    {
                        std::cout << "on close\n";
                        close(client);
                        close(listen_fd);
                        return 0;
                    }
                    std::cout << '\t' << read_buf << '\n';
                }
            }
            
        }
    }
    close(client);
    close(listen_fd);
    return 0;
}