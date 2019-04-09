#include <iostream>
#include <vector>
#include <string>

#include "send_recv.h"
#include "graph_transmit.h"


#ifndef UI_H_
#define UI_H_


void dist_pr(int sockfd, int command);
void add_node_dub_say();
void add_node_serv_talk(int sockfd, int &response);
void add_node(int sockfd);
void add_edge_dub_say();
void add_edge_serv_talk(int sockfd, int &response);
void add_edge(int sockfd);
void talk_with_serv(int sockfd);

#endif