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
    }
}

void talk_with_serv(int sockfd) {
    int pid = getpid();
    send_int(sockfd, pid, 0);

    int how;
    std::cout << "How would you like to enter the graph? Enter the number:" << std::endl;
    std::cout << "1 - via a command line" << std::endl;
    std::cout << "2 - via a file" << std::endl;
    std::cin >> how;
    if (how == 1) {
        send_data_about_graph(sockfd);
    } else if (how == 2) {
        send_from_file(sockfd);
    } else {
        std::cout << "Wrong mode" << std::endl;
    }
    
    
    while (1) {
        std::cout << "Please, enter request:" << std::endl;
        std::cout << "1 to get distance between two nodes" << std::endl;
        std::cout << "2 to get the shortest path between two nodes" << std::endl;
        std::cout << "0 to quit" << std::endl;
        int command;
        std::cin >> command;
        if (!command) {
            send_int(sockfd, command, 0);
            return;
        } else if (command == 1 || command == 2) {
            send_int(sockfd, command, 0);
            std::cout << "Enter two nodes of your graph:" << std::endl;
            int args[2];
            std::cin >> args[0] >> args[1];
            send_arr(sockfd, args, sizeof(int) * 2, 0);

            int len;
            recv_int(sockfd, len, 0);

            if (len == INF) {
                std::cout << "No connection between nodes" << std::endl;
            } else {
                std::cout << "Result:" << len << std::endl;
                if (command == 2){
                    std::vector <int> path;
                    get_path(sockfd, path);
                    for(auto it = path.rbegin(); it != path.rend(); ++it) {
                        if (it != --path.rend()){
                            std::cout << *it << "->";
                        } else {
                            std::cout << *it << std::endl;
                        }
                    }
                }   
            }
        }
    }
}
int main()
{
    int sockfd; //client fd
    std::cout << "Enter IP:" << std::endl;
    char serv_ip[15];
    scanf("%s", serv_ip);
    const int port = 49150;
    con_to_sv(sockfd, port, serv_ip);
    talk_with_serv(sockfd);
    close(sockfd);
    return 0;
}