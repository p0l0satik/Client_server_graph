#include "headers/UI.h"

const int INF = 1000000000;
enum {
    CL_E = 1, //information via a CLI
    FILE_E = 2, //information via a file
    DIST_SIMPL = 1,
    DIST_PATH = 2,
    CL_QUIT = 0,
    ADD_EDGE = 4,
    ADD_NODE = 3,
    ADDED = 1,
    QUIT_ADD = 0,
    DUBL = 0,
    CONT_ADD = 1,
    NO_NODES = 3
};

void dist_pr(int sockfd, int command) {
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

void add_node_dub_say() {
    std::cout << "Sorry, such node already exists. Would you like to try again?" <<std::endl;
    std::cout << "Please, enter:" << std::endl;
    std::cout << "1 if you want to enter another node" << std::endl;
    std::cout << "0 if you want to return to the previous menu" << std::endl;
}

void add_node_serv_talk(int sockfd, int &response) {
    int node;
    send_int(sockfd, ADD_NODE, 0);
    std::cout << "Please, enter the number of your node:" << std:: endl;
    std::cin >> node;
    send_int(sockfd, node, 0);
    recv_int(sockfd, response, 0);
} 

void add_node(int sockfd) {
    int response;
    add_node_serv_talk(sockfd, response);
    if (response == ADDED) {
        std::cout << "A new node was successfully added!" << std::endl;
    } else {
        add_node_dub_say();
        int command;
        std::cin >> command;
        while (response != ADDED && command != QUIT_ADD) {
            if (command == CONT_ADD) {
                add_node_serv_talk(sockfd, response);
                if (response == DUBL) {
                    add_node_dub_say();
                    std::cin >> command;
                }
            } else {
                std::cout << "Please, enter correct command:" << std::endl;
                std::cin >> command;
            }
        }
        if (response == ADDED) {
            std::cout << "A new node was successfully added!" << std::endl;
        }
    }
}

void add_edge_dub_say() {
    std::cout << "Sorry, such edge already exists. Would you like to try again?" <<std::endl;
    std::cout << "Please, enter:" << std::endl;
    std::cout << "1 if you want to enter another edge" << std::endl;
    std::cout << "0 if you want to return to the previous menu" << std::endl;
}

void add_edge_serv_talk(int sockfd, int &response) {
    int arr[3];
    send_int(sockfd, ADD_EDGE, 0);
    std::cout << "Please, enter the number of your edge(three numbers, from to cost):" << std:: endl;
    std::cin >> arr[0] >> arr[1] >> arr[2];
    send_arr(sockfd, arr, 3 * sizeof(*arr), 0);
    recv_int(sockfd, response, 0);
} 

void add_edge(int sockfd) {
    int response;
    add_edge_serv_talk(sockfd, response);
    if (response == ADDED) {
        std::cout << "A new edge was successfully added!" << std::endl;
    } else {
        if (response == NO_NODES) {
            std::cout << "Sorry, there is no such nodes. Please add them before adding the edge." << std::endl;
            return;
        }
        add_edge_dub_say();
        int command;
        std::cin >> command;
        while (response != ADDED && command != QUIT_ADD) {
            if (response == NO_NODES) {
                std::cout << "Sorry, there is no such nodes. Please add them before adding the edge." << std::endl;
                return;
            }
            if (command == CONT_ADD) {
                add_edge_serv_talk(sockfd, response);
                if (response == DUBL) {
                    add_edge_dub_say();
                    std::cin >> command;
                }
            } else {
                std::cout << "Please, enter correct command:" << std::endl;
                std::cin >> command;
            }
        }
        if (response == ADDED) {
            std::cout << "A new edge was successfully added!" << std::endl;
        }
    }
}

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
        std::cout << "3 - add a new node" << std::endl;
        std::cout << "4 - add a new edge" << std::endl;
        std::cout << "0 - quit" << std::endl;
        int command;
        std::cin >> command;
        while (command != CL_QUIT && command != DIST_PATH && command != DIST_SIMPL 
            && command != ADD_NODE && command != ADD_EDGE)  {
            std::cout << "Wrong command. Please, try again." << std::endl;
            std::cin >> command;
        }
        if (command == CL_QUIT) {
            send_int(sockfd, command, 0);
            return;
        } else if (command == DIST_SIMPL || command == DIST_PATH) {
            dist_pr(sockfd, command);
        } else if (command == ADD_EDGE) {
            add_edge(sockfd);
        } else if (command == ADD_NODE) {
            add_node(sockfd);
        }
    }
}