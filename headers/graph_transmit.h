#include <cstdlib>

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "send_recv.h"


const int BUF_SIZE = 3*400;
using graph_t = std::map<int, std::map<int, int> >; //node to node and cost

void send_data_about_graph(int sockfd, int mode);
void send_from_file(int sockfd);
void build_graph(int sockfd, graph_t &graph);