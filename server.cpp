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
            std::cout << "    " << j->first << " weight " << j->second << std::endl; 
        }
    }
}

enum 
{
    CONNECTED = 1,
    SEND_GRAPH = 2,
    CLIENT_TALK = 3,
    DIST_SIMPL = 1,
    DIST_PATH = 2,
    CL_QUIT = 0,
    ADD_NODE = 3,
    ADD_EDGE = 4,
    EXISTS = 0,
    ADDED = 1,
    NO_NODES = 3
};

void add_node(int sockfd, graph_t &graph) {
    int node;
    recv_int(sockfd, node, 0);
    if (graph.find(node) != graph.end()) {
        send_int(sockfd, EXISTS, 0);
    } else {
        graph[node] = std::map <int, int> ();
        std::cout << "Client added node:" << node << std::endl;
        send_int(sockfd, ADDED, 0);
    }
}

void add_edge(int sockfd, graph_t &graph) {
    int arr[3];
    recv_arr(sockfd, arr, 3 * sizeof(*arr), 0);
    if (graph.find(arr[0]) == graph.end() || graph.find(arr[1]) == graph.end()){
        send_int(sockfd, NO_NODES, 0);
        return;
    }
    if (graph[arr[0]].find(arr[1]) != graph[arr[0]].end()) {
        send_int(sockfd, EXISTS, 0);
    } else {
        graph[arr[0]][arr[1]] = arr[2];
        std::cout << "Client added edge:" << arr[0] << " " << arr[1] << " " << arr[2] << std::endl;
        send_int(sockfd, ADDED, 0);
    }
}

int client_ctl(int sockfd, int type, graph_t &graph) {
    if (type == CONNECTED) {
        std::cout << "Client connected."<< std::endl;
    }
    if (type == SEND_GRAPH) {
        std::cout << "Client sent a graph:" << std::endl;
        print_graph(graph);
    }
    
    if (type == CLIENT_TALK) {
        int command = 0;
        recv_int(sockfd, command, 0);
        if (command == DIST_SIMPL) {

            int args[2];
            recv_arr(sockfd, args, 2 * sizeof(int), 0);
            int res = calc_dist(args[0], args[1], graph);
            send_int(sockfd, res, 0);

        } else if (command == CL_QUIT) {

            std::cout << "End of session with client." << std::endl;
            close(sockfd);
            return 0;

        } else if (command == DIST_PATH) {
            int args[2];
            recv_arr(sockfd, args, 2 * sizeof(int), 0);
            std::vector <int> path;
            int res = calc_dist(args[0], args[1], graph, path);
            send_int(sockfd, res, 0);
            send_path(sockfd, path);
            
        } else if(command == ADD_NODE) {
            add_node(sockfd, graph);
        } else if(command == ADD_EDGE) {
            add_edge(sockfd, graph);
        } else {
            std::cout << "Undefined command. Closing conection.";
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
        graph_t graph;

        client_ctl(client, CONNECTED, graph);
        build_graph(client, graph);

        client_ctl(client, SEND_GRAPH, graph);
        

        while(client_ctl(client, CLIENT_TALK, graph)) {}
   
        close(client);
        
    }
    
    return 0;
}