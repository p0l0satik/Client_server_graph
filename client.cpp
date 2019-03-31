#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "headers/send_recv.h"
#include "headers/graph_transmit.h"
#include "headers/UI.h"

const int INF = 1000000000;


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
    } else {
        std::cout << "Successfully connected to server!" << std::endl;
    }
}

int main()
{
    int sockfd; //client fd
    std::cout << "Please, enter server IP:" << std::endl;
    char serv_ip[15];
    scanf("%s", serv_ip);
    const int port = 49150;
    con_to_sv(sockfd, port, serv_ip);
    talk_with_serv(sockfd);
    close(sockfd);
    return 0;
}