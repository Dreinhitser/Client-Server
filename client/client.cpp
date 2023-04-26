#include "../erproc.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
    Connect(fd, (struct sockaddr *) &adr, sizeof(adr));

    char command[512];
    char flags[512];

    Message msg;

    while (true)
    {
        fgets(msg.message, 2048, stdin);
        msg.count_next_message = 0;

        write(fd, &msg, sizeof(msg));

        // прием сообщений клиентом
        char buf[2048];
        ssize_t nread;
        do
        {
            nread = read(fd, &msg, sizeof(msg));
            if (nread == -1)
            {
                perror("read failed");
                exit(EXIT_FAILURE);
            }
            if (nread == 0)
            {
                printf("EOF occured\n");
            }
            printf("%s", msg.message);
            msg.count_next_message--;
        }
        while (msg.count_next_message > 0);
    }
    
    return 0;
}