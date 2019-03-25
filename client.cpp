#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

char message[] = "Hello there!\n";
char buf[sizeof(message)];


void con_to_sv(int &sockfd, const int port, const char* serv_ip) {
    struct sockaddr_in addr; //socket address

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    struct in_addr *inp = &addr.sin_addr;
    inet_aton(serv_ip, inp);

    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
}

int send_arr(int sockfd, int *arr, int len, int flags) {
    int alr_sent = 0;
    int n = 0;
    while(alr_sent < len / sizeof(int)) {
        
        n = send(sockfd, (arr + alr_sent), len - alr_sent * sizeof(int), flags);
        if (n == -1) {
            std::cout << "error while sending" << std::endl;
            return -1;
        }
        alr_sent += n / sizeof(int);
    }
    return alr_sent;
}

int recv_arr(int sockfd, int *arr, int len, int flags) {
    int alr_recv = 0;
    int n = 0;
    while(alr_recv < len / sizeof(int)) {
        
        n = recv(sockfd, (arr + alr_recv), len - alr_recv * sizeof(int), flags);
        if (n == -1) {
            std::cout << "error while receiving" << std::endl;
            return -1;
        }
        alr_recv += n / sizeof(int);
    }
    return alr_recv;
}

int main()
{
    int sockfd; //client fd
    const char serv_ip[] = "127.0.0.1";
    const int port = 14888;
    con_to_sv(sockfd, port, serv_ip);
    std::cout << "s" << std::endl;
    int arr1[] = {1, 2, 3, 4, 5, 6, 7};
    send_arr(sockfd, arr1, sizeof(int) * 7, 0);
    
    // puts(buf);
    close(sockfd);

    return 0;
}