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

int main()
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("0.0.0.0");
    //server_address.sin_addr.s_addr = inet_addr("192.168.1.61");

    int listen_fd = socket(PF_INET, SOCK_DGRAM, 0);

    std::cout << "bind : " << bind(listen_fd, (struct sockaddr*) &server_address, sizeof(server_address)) << '\n';

    std::cout << "listen_fd : " << listen_fd << "\n";
    //int listen_ret = listen(listen_fd, 10);
    //std::cout << "listen_ret : " << listen_ret << '\n';
    std::cout << "Debut chat" << '\n';

    sockaddr ret;
    socklen_t length = 0;

    fd_set readfds;
    char read_buf[100];
    char send_buf[100];
    int n = 0;
    struct sockaddr_in client;

    while (1)
    {
        memset(read_buf, 0, sizeof(read_buf));
        socklen_t len = 0;
        n = recvfrom(listen_fd, read_buf, sizeof(read_buf), MSG_WAITALL, (sockaddr*) &client, &len);
        if (n > 0)
            printf("\t%s : %s\n", inet_ntoa(client.sin_addr),read_buf);
        else
            printf("rip\n");
        usleep(50);
    }
    close(listen_fd);
    return 0;
}