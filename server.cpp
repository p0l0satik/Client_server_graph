#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

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

void start_sv(int &listener, const int port, const int q_size) {
    struct sockaddr_in addr;
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    if (listen(listener, q_size) == -1){
        perror("listen");
        exit(3);
    }
} 

void ac_cl(int &sockfd, const int listener) {
    sockfd = accept(listener, NULL, NULL);
    if(sockfd < 0)
    {
        perror("accept");
        exit(3);
    }
}

int main()
{
    int client, listener; // client and server fds
    const int port = 14888;
    const int q_size = 4;
    start_sv(listener, port, q_size);

    
    while(1)
    {
        ac_cl(client, listener);   
        int arr[10];
        int bytes = recv_arr(client, arr, sizeof(int) * 7, 0);
        std::cout << "read:" << bytes << std::endl;
        for(int t = 0; t < 7; t++) {
            std::cout << arr[t] << std::endl;
        }
    
        close(client);
    }
    
    return 0;
}