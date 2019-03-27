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

struct edge
{
    int a, b, cost;
    edge(const int &v_a, const int &v_b, const int &e_cost):
        a(v_a), b(v_b), cost(e_cost){}
};

using graph_t = std::map<int, std::map<int, int> >; //node to node and cost
using node_l_t = std::map<int, int>; // list of distances; node to dist
using edge_v_t = std::vector<edge>; // vector of edges
const int INF = 1000000000;

void graph_to_arr(const graph_t &graph, std::vector<edge> &edge_arr, const int &n){

    for (auto i = graph.begin(); i != graph.end(); ++i) {
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            edge_arr.push_back(edge(i->first, j->first, j->second));
        }
    }
}

void fill_dist(node_l_t &dist, const graph_t &graph){
    for(auto it = graph.begin(); it != graph.end(); ++it) {
        dist[it->first] = INF;
    }
}

void FB (const edge_v_t & edge_arr, node_l_t &dist) {
    bool change = true;
    while(change){
        change = false;
        for (int t = 0; t < edge_arr.size(); ++t){
            if (dist[edge_arr[t].a] < INF) {
                if (dist[edge_arr[t].b] > dist[edge_arr[t].a] + edge_arr[t].cost) {
                    dist[edge_arr[t].b] =  dist[edge_arr[t].a] + edge_arr[t].cost;
                    change = true;
                }
            }
        }
    }
}

void print_calcs(int v, const node_l_t &dist){
    std::cout << v << "->" << std::endl;
    for (auto it = dist.begin(); it != dist.end(); ++it){
        std::cout << it->first << ": " << it->second << ", ";
    }
}
int calc_dist(int v, int to, const graph_t &graph) {
    int n = graph.size();
    edge_v_t edge_arr;
    node_l_t dist;

    graph_to_arr(graph, edge_arr, n);
    fill_dist(dist, graph);
    dist[v] = 0;

    FB(edge_arr, dist);
    print_calcs(v, dist);
    return dist[to];
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

int send_int(int sockfd, int snd, int flags) {
    int bytes = 0;
    while (bytes < sizeof(snd)) {
        bytes = send(sockfd, &snd, sizeof(snd), flags);
        if (bytes == -1) {
            std::cout << "error while sending" << std::endl;
            return -1;
        }
    }
    return bytes;
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

int recv_int(int sockfd, int &snd, int flags) {
    int bytes = 0;
    while (bytes < sizeof(snd)) {
        bytes = recv(sockfd, &snd, sizeof(snd), flags);
        if (bytes == -1) {
            std::cout << "error while sending" << std::endl;
            return -1;
        }
    }
    return bytes;
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

void build_graph(int sockfd, graph_t &graph) {
    int m, n;
    int BUF_SIZE = 0;
    recv_int(sockfd, BUF_SIZE, 0);
    int buf[BUF_SIZE];

    recv_int(sockfd, n, 0);

    for (int t = 0; t <= n / BUF_SIZE; ++t) {
        if (t < n / BUF_SIZE) { // receive by whole buffers
            recv_arr(sockfd, buf, BUF_SIZE * sizeof(int), 0);
            for (int j = 0; j < BUF_SIZE; j++){
                graph[buf[j]] = std::map <int, int> ();
            }
        } else if (n % BUF_SIZE != 0) { //receive what is left
            recv_arr(sockfd, buf, (n % BUF_SIZE) * sizeof(int), 0);
            for (int j = 0; j < n % BUF_SIZE; j++){
                graph[buf[j]] = std::map <int, int> ();
            }
        }
    }

    recv_int(sockfd, m, 0);

    for (int t = 0; t <= m / (BUF_SIZE / 3); ++t) {
        if (t < m / (BUF_SIZE / 3)) {// receive by whole buffers
            recv_arr(sockfd, buf, BUF_SIZE * sizeof(int), 0);
            for(int j = 0; j < BUF_SIZE; j += 3) {
                graph[buf[j]][buf[j + 1]] = buf[j + 2]; 
            }
        } else if (m % (BUF_SIZE / 3)) {//receive what is left
            std::cout << m % (BUF_SIZE / 3) << std::endl;
            recv_arr(sockfd, buf, m % (BUF_SIZE / 3) * 3 * sizeof(int), 0);
            for (int t = 0; t < m % (BUF_SIZE / 3) * 3; t++) {
                std::cout << t <<" " << buf[t] << std::endl;
            }
            for(int j = 0, t = 0; t < m % (BUF_SIZE / 3); ++t, j += 3) {
                graph[buf[j]][buf[j + 1]] = buf[j + 2]; 
                std::cout << graph[buf[j]][buf[j + 1]] << std::endl;
            }
        }
        
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