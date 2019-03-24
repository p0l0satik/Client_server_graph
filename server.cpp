#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int sockfd, listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(14888);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 4);
    
    while(1)
    {
        sockfd = accept(listener, NULL, NULL);
        if(sockfd < 0)
        {
            perror("accept");
            exit(3);
        }

        while(1)
        {
            bytes_read = recv(sockfd, buf, 1024, 0);
            if(bytes_read <= 0) break;
            send(sockfd, buf, bytes_read, 0);
            int pid = 0;
            bytes_read = recv(sockfd, &pid, sizeof(pid), 0);
            printf("%d\n", pid);
            
        }
    
        close(sockfd);
    }
    
    return 0;
}