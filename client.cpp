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

const int BUF_SIZE = 3*400;
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

void send_data_about_graph(int sockfd, int mode) {
    int a, b, c;
    int m, n;
    int buf[BUF_SIZE];
    std::ifstream inp;

    if (mode == 2) {
        const int MAX_PATH = 4096;
        char filepath[MAX_PATH];
        std::cin >> filepath;
        inp.open(filepath); 
        while (!inp.is_open()) {
            std::cout << "Unable to open your file, please try again" << std::endl;
            std::cin >> filepath;
            inp.open(filepath);
        } 
    }

    send_int(sockfd, BUF_SIZE, 0);

    if (mode == 1) {
        std::cout << "enter number of nodes:" << std::endl;
        std::cin >> n;
    } else {
        inp >> n;
    }
    
    send_int(sockfd, n, 0);

    std::cout << "enter the nodes:" << std::endl;
    for(int t = 0, j = 0; t <= n; ++t, ++j) {
        // std::cout <<"j is " << j << std:: endl;
        if (t < n) {
            if (mode == 1) {
                std::cin >> a;
            } else {
                inp >> a;
            }
            buf[j] = a;
            if (j + 1 == BUF_SIZE) { //send when buf is full
                send_arr(sockfd, buf, BUF_SIZE * sizeof(int), 0);
                j = -1;
            }
        } else {
            if (j != 0) { //send what is left(if there is smth)
                send_arr(sockfd, buf, j * sizeof(int), 0);
            }
        }
    }

    if (mode == 1) {
        std::cout << "enter number of edges:" << std::endl;
        std::cin >> m;   
    } else {
        inp >> m;
    }
    
    send_int(sockfd, m, 0);
    if (mode == 1){
        std::cout << "enter the edges in format - node_from, node_to and edge_cost:" << std::endl;
    }
    for(int t = 0, j = 0; t <= m; t++) {
        if (t < m) {
            if (mode == 1) {
                std::cin >> a >> b >> c;
            } else {
                inp >> a >> b >> c;
            }
            
            buf[j++] = a;
            buf[j++] = b;
            buf[j++] = c;
            if(j == BUF_SIZE) { //send when buf is full
                send_arr(sockfd, buf, BUF_SIZE * sizeof(int), 0);
                j = 0;
            }
        } else {
            std::cout << "j: " << j << std::endl;
            if (j != 0) { //send what is left(if there is smth)
                send_arr(sockfd, buf, j * sizeof(int), 0);
            }
            
        }
    }
}

void send_from_file(int sockfd) {
    std::cout << "Data in your file should be specified in a special order:" << std::endl;
    std::cout << "Number of nodes" << std::endl; 
    std::cout << "The list of nodes (a number for each node)" << std::endl;
    std::cout << "number of edges" << std::endl;
    std::cout << "The list of edges (three numbers for each edge:node_from, node_to and edge_cost)" << std::endl;
    std::cout << "Enter path to you file:" << std::endl;
    send_data_about_graph(sockfd, 2);
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
        send_data_about_graph(sockfd, 1);
    } else if (how == 2) {
        send_from_file(sockfd);
    } else {
        std::cout << "wrong mode" << std::endl;
    }
    
    
    while (1) {
        std::cout << "Do you want to enter request? Enter 1 for 'yes', 0 for 'no':" << std::endl;
        int command;
        std::cin >> command;
        if (!command) {
            send_int(sockfd, command, 0);
            break;
        }
        send_int(sockfd, command, 0);
        std::cout << "Enter two nodes of your graph:" << std::endl;
        int args[2];
        std::cin >> args[0] >> args[1];
        send_arr(sockfd, args, sizeof(int) * 2, 0);
        int len;
        recv_int(sockfd, len, 0);
        if (len == INF) {
            
        }
        std::cout << "Result:" << len << std::endl;

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