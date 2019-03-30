#include <cstdlib>

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "send_recv.h"

#ifndef GRAPH_TRANSMIT_H_
#define GRAPH_TRANSMIT_H_


const int BUF_SIZE = 3*400;
using graph_t = std::map<int, std::map<int, int> >; //node to node and cost

void send_data_about_graph(int sockfd);
void send_data_about_graph(int sockfd, std::ifstream &inp);
void send_from_file(int sockfd);
void build_graph(int sockfd, graph_t &graph);
void get_path(int sockfd, std::vector <int> &path);
void send_path(int sockfd, std::vector<int> &path);

#endif