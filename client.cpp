#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char message[] = "Hello there!\n";
char buf[sizeof(message)];

int main()
{
    int sockfd;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(14888);
    struct in_addr *inp = &addr.sin_addr;
    inet_aton("172.17.0.6", inp);;

    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    send(sockfd, message, sizeof(message), 0);
    int pid = getpid();
    send(sockfd, &pid, sizeof(int), 0);
    recv(sockfd, buf, sizeof(message), 0);
    
    puts(buf);
    close(sockfd);

    return 0;
}