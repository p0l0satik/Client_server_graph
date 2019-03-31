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
    } else {
        std::cout << "Successfully connected to server!" << std::endl;
    }
}

enum {
    CL_E = 1, //information via a CLI
    FILE_E = 2, //information via a file
    DIST_SIMPL = 1,
    DIST_PATH = 2,
    CL_QUIT = 0
};

void talk_with_serv(int sockfd) {

    int how;
    std::cout << "How would you like to enter the graph? Please, enter the number:" << std::endl;
    std::cout << "1 - via a command line" << std::endl;
    std::cout << "2 - via a file" << std::endl;
    std::cin >> how;
    while (how != CL_E && how != FILE_E) {
        std::cout << "Wrong mode. Please, try again." << std::endl;
        std::cin >> how;
    }
    if (how == CL_E) {
        send_data_about_graph(sockfd);
    } else if (how == FILE_E) {
        send_from_file(sockfd);
    } 
    while (1) {
        std::cout << "Please, enter request to server:" << std::endl;
        std::cout << "1 - get distance between two nodes" << std::endl;
        std::cout << "2 - get the shortest path between two nodes" << std::endl;
        std::cout << "0 - quit" << std::endl;
        int command;
        std::cin >> command;
        while (command != CL_QUIT && command != DIST_PATH && command != DIST_SIMPL) {
            std::cout << "Wrong command. Please, try again." << std::endl;
            std::cin >> command;
        }
        if (command == CL_QUIT) {
            send_int(sockfd, command, 0);
            return;
        } else if (command == DIST_SIMPL || command == DIST_PATH) {
            send_int(sockfd, command, 0);
            std::cout << "Please, enter two nodes of your graph:" << std::endl;
            int args[2];
            std::cin >> args[0] >> args[1];
            send_arr(sockfd, args, sizeof(int) * 2, 0);

            int len;
            recv_int(sockfd, len, 0);

            if (len == INF) {
                std::cout << "There is no connection between nodes." << std::endl;
            } else {
                std::cout << "Distance between nodes is:" << std::endl << len << std::endl;
                if (command == 2){
                    std::vector <int> path;
                    get_path(sockfd, path);
                    std::cout << "The shortest route is:" << std::endl;
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
    std::cout << "Please, enter server IP:" << std::endl;
    char serv_ip[15];
    scanf("%s", serv_ip);
    const int port = 49150;
    con_to_sv(sockfd, port, serv_ip);
    talk_with_serv(sockfd);
    close(sockfd);
    return 0;
}