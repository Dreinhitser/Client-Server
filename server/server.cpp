#include "../erproc.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
    // создание сокета
    int server = socket(AF_INET, SOCK_STREAM, 0);

    // привязка сокета к адресу
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(34543);
    Bind(server, (struct sockaddr *) &adr, sizeof(adr));
    Listen(server, 5);
    socklen_t adrlen = sizeof(adr);
    int fd = Accept(server, (struct sockaddr *) &adr, &adrlen);

    Message msg;

    while (true)
    {
        ssize_t nread;
        char buff[2048];
        nread = read(fd, &msg, sizeof(msg));
        if (nread == -1)
        {
            perror("read failed");
            exit(EXIT_FAILURE);
        }
        if (nread == 0)
        {
            printf("END OF FILE occured\n");
        }

        FILE *popen_result;
        popen_result = popen(msg.message, "r");

        if(!popen_result)
        {
            char message[] = "Can't execute command\n";
            write(fd, message, strlen(message));
            continue;
        }

        while(fgets(buff, sizeof(buff), popen_result) != NULL)
        {
            strcpy(msg.message, buff);
            msg.count_next_message = 2;
            write(fd, &msg, sizeof(msg));
        }

        msg.message[0] = 0;
        msg.count_next_message = 0;
        write(fd, &msg, sizeof(msg));

        pclose(popen_result);
    }
    
    return 0;
}