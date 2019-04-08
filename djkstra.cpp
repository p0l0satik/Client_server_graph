#include "headers/djkstra.h"

const int INF = 1000000000;
void fill_dist_and_mark(node_l_t &dist, mark_t &mark, graph_t graph) {
    for(auto it = graph.begin(); it != graph.end(); ++it) {
        dist[it->first] = INF;
        mark[it->first] = false;
    }
}

int DJ(node_l_t &dist, mark_t &marks, graph_t &graph){
    int op = 0;
    for(auto t = graph.begin(); t != graph.end(); ++t){
        int v = -1;
        for (auto j = graph.begin(); j != graph.end(); ++j) {
            op++;
            if (marks[j->first] != true && (v == -1 || dist[j->first] < dist[v])){
                v = j->first;
            }
        }
        if (dist[v] == INF) {
            break;
        }
        marks[v] = true;
        for (auto k = (graph[v]).begin(); k != (graph[v]).end(); ++k) {
            op++;
            if (dist[v] + k->second < dist[k->first]){
                dist[k->first] = dist[v] + k->second;
            }   
        }
    }
    return op;
}

int calc_dist_DJ(int v, int to, graph_t &graph, int &op) {
    node_l_t dist;
    mark_t marks;
    fill_dist_and_mark(dist, marks, graph);
    dist[v] = 0;
    op = DJ(dist, marks, graph);
    return dist[to];
}