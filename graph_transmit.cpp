#include "headers/graph_transmit.h"



void send_data_about_graph(int sockfd) {
    int a, b, c;
    int m, n;
    int *buf = new int[BUF_SIZE];

    std::cout << "Please, enter number of nodes:" << std::endl;
    std::cin >> n;

    send_int(sockfd, n, 0);

    std::cout << "Please, enter the nodes:" << std::endl;
    for(int t = 0, j = 0; t <= n; ++t, ++j) {
        if (t < n) {
            std::cin >> a;
            buf[j] = a;
            if (j + 1 == BUF_SIZE) { //send when buf is full
                send_arr(sockfd, buf, BUF_SIZE * sizeof(*buf), 0);
                j = -1;
            }
        } else {
            if (j != 0) { //send what is left(if there is smth)
                send_arr(sockfd, buf, j * sizeof(*buf), 0);
            }
        }
    }

    std::cout << "Please, enter number of edges:" << std::endl;
    std::cin >> m;   
   
    send_int(sockfd, m, 0);
   
    std::cout << "Please, enter the edges in format(three numbers): node_from node_to edge_cost:" << std::endl;
    
    for(int t = 0, j = 0; t <= m; t++) {
        if (t < m) {
            std::cin >> a >> b >> c;
            buf[j++] = a;
            buf[j++] = b;
            buf[j++] = c;
            if(j == BUF_SIZE) { //send when buf is full
                send_arr(sockfd, buf, BUF_SIZE * sizeof(int), 0);
                j = 0;
            }
        } else {
            if (j != 0) { //send what is left(if there is smth)
                send_arr(sockfd, buf, j * sizeof(int), 0);
            }
        }
    }

    delete[] buf;
}

void send_data_about_graph(int sockfd, std::ifstream &inp) {
    int a, b, c;
    int m, n;
    int *buf = new int[BUF_SIZE];
    inp >> n;
    send_int(sockfd, n, 0);
    for(int t = 0, j = 0; t <= n; ++t, ++j) {
        if (t < n) {
            
            inp >> a;
            
            buf[j] = a;
            if (j + 1 == BUF_SIZE) { //send when buf is full
                send_arr(sockfd, buf, BUF_SIZE * sizeof(*buf), 0);
                j = -1;
            }
        } else {
            if (j != 0) { //send what is left(if there is smth)
                send_arr(sockfd, buf, j * sizeof(*buf), 0);
            }
        }
    }

  
    inp >> m;
    
    
    send_int(sockfd, m, 0);
    for(int t = 0, j = 0; t <= m; t++) {
        if (t < m) {
        
            inp >> a >> b >> c;
            buf[j++] = a;
            buf[j++] = b;
            buf[j++] = c;
            if(j == BUF_SIZE) { //send when buf is full
                send_arr(sockfd, buf, BUF_SIZE * sizeof(int), 0);
                j = 0;
            }
        } else {
            if (j != 0) { //send what is left(if there is smth)
                send_arr(sockfd, buf, j * sizeof(int), 0);
            }
        }
    }
    delete[] buf;
}

void send_from_file(int sockfd) {
    std::cout << "Data in your file should be specified in a special order:" << std::endl;
    std::cout << "Number of nodes" << std::endl; 
    std::cout << "The list of nodes (a number for each node)" << std::endl;
    std::cout << "Number of edges" << std::endl;
    std::cout << "The list of edges (three numbers for each edge: node_from, node_to and edge_cost)" << std::endl;
    std::cout << "Please, enter path to you file:" << std::endl;
    const int MAX_PATH = 4096;
    char *filepath = new char[MAX_PATH];
    std::cin >> filepath;
    std::ifstream inp;
    inp.open(filepath); 
    while (!inp.is_open()) {
        std::cout << "Unable to open your file, please try again" << std::endl;
        std::cin >> filepath;
        inp.open(filepath);
    } 
    send_data_about_graph(sockfd, inp);
}

void build_graph(int sockfd, graph_t &graph) {
    int m, n;
    int *buf = new int[BUF_SIZE];

    recv_int(sockfd, n, 0);

    for (int t = 0; t <= n / BUF_SIZE; ++t) {
        if (t < n / BUF_SIZE) { // receive by whole buffers
            recv_arr(sockfd, buf, BUF_SIZE * sizeof(*buf), 0);
            for (int j = 0; j < BUF_SIZE; j++){
                graph[buf[j]] = std::map <int, int> ();
            }
        } else if (n % BUF_SIZE != 0) { //receive what is left
            recv_arr(sockfd, buf, (n % BUF_SIZE) * sizeof(*buf), 0);
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
            recv_arr(sockfd, buf, m % (BUF_SIZE / 3) * 3 * sizeof(int), 0);
            for(int j = 0, t = 0; t < m % (BUF_SIZE / 3); ++t, j += 3) {
                graph[buf[j]][buf[j + 1]] = buf[j + 2]; 
            }
        }
        
    }
    delete[] buf;
}


void get_path(int sockfd, std::vector <int> &path) {
    int n;
    int *buf = new int[BUF_SIZE];

    recv_int(sockfd, n, 0);

    for (int t = 0; t <= n / BUF_SIZE; ++t) {
        if (t < n / BUF_SIZE) { // receive by whole buffers
            recv_arr(sockfd, buf, BUF_SIZE * sizeof(*buf), 0);
            for (int j = 0; j < BUF_SIZE; j++){
                path.push_back(buf[j]);
            }
        } else if (n % BUF_SIZE != 0) { //receive what is left
            recv_arr(sockfd, buf, (n % BUF_SIZE) * sizeof(*buf), 0);
            for (int j = 0; j < n % BUF_SIZE; j++){
                path.push_back(buf[j]);
            }
        }
    }

    delete[] buf;
}   

void send_path(int sockfd, std::vector<int> &path){
    int n = path.size();
    send_int(sockfd, n, 0);
    int *buf = new int[BUF_SIZE];
    for(int t = 0, j = 0; t <= n; ++t, ++j) {
        if (t < n) {
            buf[j] = path[t];
            if (j + 1 == BUF_SIZE) { //send when buf is full
                send_arr(sockfd, buf, BUF_SIZE * sizeof(*buf), 0);
                j = -1;
            }
        } else {
            if (j != 0) { //send what is left(if there is smth)
                send_arr(sockfd, buf, j * sizeof(*buf), 0);
            }
        }
    }
    delete[] buf;
}

