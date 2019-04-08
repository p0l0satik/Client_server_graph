#include <iostream>
#include <map>
#include <vector>

#ifndef DJKSTRA_H_
#define DJKSTRA_H_
using graph_t = std::map<int, std::map<int, int> >; //node to node and cost
using node_l_t = std::map<int, int>; // list of distances; node to dist
using mark_t = std::map<int, bool>;//list of marks for nodes; node to mark 

int calc_dist_DJ(int v, int to, graph_t &graph, int &op);
int DJ(node_l_t &dist, mark_t &marks, graph_t &graph);
void fill_dist_and_mark(node_l_t &dist, mark_t &mark, graph_t graph);

#endif
