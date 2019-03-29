#include <iostream>
#include <map>
#include <vector>

#ifndef FB_H_
#define FB_H_
struct edge
{
    int a, b, cost;
    edge(const int &v_a, const int &v_b, const int &e_cost):
        a(v_a), b(v_b), cost(e_cost){}
};
using graph_t = std::map<int, std::map<int, int> >; //node to node and cost
using node_l_t = std::map<int, int>; // list of distances; node to dist
using edge_v_t = std::vector<edge>; // vector of edges


void graph_to_arr(const graph_t &graph, std::vector<edge> &edge_arr, const int &n);
void fill_dist(node_l_t &dist, const graph_t &graph);
void FB (const edge_v_t & edge_arr, node_l_t &dist);
void print_calcs(int v, const node_l_t &dist);
int calc_dist(int v, int to, const graph_t &graph);

#endif