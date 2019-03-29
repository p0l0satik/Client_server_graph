#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>

#include "headers/send_recv.h"
#include "headers/graph_transmit.h"
#include "headers/FB.h"


using graph_t = std::map<int, std::map<int, int> >; //node to node and cost
const int INF = 1000000000;

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

void print_graph(const graph_t &graph) {
    for (auto i = graph.begin(); i != graph.end(); ++i) {
        std::cout << i->first << ":" << std::endl;
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            std::cout << j->first << " weight " << j->second << std::endl; 
        }
    }
}

int  client_ctl(int sockfd, int &pid, int type, const graph_t &graph) {
    if (type == 1) {
        recv_int(sockfd, pid, 0);
        std::cout << "Client connected, his pid:" << pid << std::endl;
    }
    if (type == 2) {
        std::cout << "Client " << pid <<" sent a graph:" << std::endl;
    }
    
    if (type == 3) {
        int command = 0;
        recv_int(sockfd, command, 0);
        if (command == 1) {
            int args[2];
            recv_arr(sockfd, args, 2 * sizeof(int), 0);
            int res = calc_dist(args[0], args[1], graph);
            send_int(sockfd, res, 0);
        } else if (command == 0) {
            std::cout << "End od sessin with Client:" << pid << std::endl;
            close(sockfd);
            return 0;
        } else {
            std::cout << "Undefined command";
            close(sockfd);
            return 0;
        }
    }
    return 1;
}

int main()
{
    int client, listener; // client and server fds
    const int port = 49150;
    const int q_size = 4;
    start_sv(listener, port, q_size);

    
    while(1)
    {   
        ac_cl(client, listener);
        if (fork() == 0) {
            int pid = 0;
            graph_t graph;

            client_ctl(client, pid, 1, graph);
            build_graph(client, graph);

            client_ctl(client, pid, 2, graph);
            print_graph(graph);

            while(client_ctl(client, pid, 3, graph)) {}
        }
        close(client);
        
    }
    
    return 0;
}