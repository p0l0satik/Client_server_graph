#include "headers/FB.h"


const int INF = 1000000000;


void graph_to_arr(const graph_t &graph, std::vector<edge> &edge_arr, const int &n) {

    for (auto i = graph.begin(); i != graph.end(); ++i) {
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            edge_arr.push_back(edge(i->first, j->first, j->second));
        }
    }
}

void fill_dist_and_parent(node_l_t &dist, parent_l_t &parent, const graph_t &graph) {
    for(auto it = graph.begin(); it != graph.end(); ++it) {
        dist[it->first] = INF;
        parent[it->first] = INF;
    }
}

void get_path(parent_l_t parent, int to, int v, std::vector <int> &path) {
    path.push_back(to);
    while(v != to) {
        to = parent[to];
        path.push_back(to);
    }
}



int FB (const edge_v_t & edge_arr, parent_l_t &parent, node_l_t &dist) {
    bool change = true;
    int operations = 0;
    while(change){
        change = false;
        for (int t = 0; t < edge_arr.size(); ++t){
            operations++;
            if (dist[edge_arr[t].a] < INF) {
                if (dist[edge_arr[t].b] > dist[edge_arr[t].a] + edge_arr[t].cost) {
                    dist[edge_arr[t].b] =  dist[edge_arr[t].a] + edge_arr[t].cost;
                    parent[edge_arr[t].b] = edge_arr[t].a;
                    change = true;
                }
            }
        }
    }
    return operations;
}

void print_calcs(int v, const node_l_t &dist){
    std::cout << v << "->" << std::endl;
    for (auto it = dist.begin(); it != dist.end(); ++it){
        std::cout << it->first << ": " << it->second << ", ";
    }
}

int calc_dist(int v, int to, const graph_t &graph, std::vector <int> &path, int &op) {
    int n = graph.size();
    edge_v_t edge_arr;
    node_l_t dist;
    parent_l_t parent;

    graph_to_arr(graph, edge_arr, n);
    fill_dist_and_parent(dist, parent, graph);
    dist[v] = 0;

    op = FB(edge_arr, parent, dist);
    if (dist[to] < INF) {
        get_path(parent, to, v, path);
    }
    return dist[to];
}

int calc_dist(int v, int to, const graph_t &graph) {
    int n = graph.size();
    edge_v_t edge_arr;
    node_l_t dist;
    parent_l_t parent;

    graph_to_arr(graph, edge_arr, n);
    fill_dist_and_parent(dist, parent, graph);
    dist[v] = 0;

    FB(edge_arr, parent, dist);
    return dist[to];
}

//what does it do?
void print_distances(const graph_t &graph) {
    for (auto it = graph.begin(); it != graph.end(); ++it){
        calc_dist(it->first, it->first, graph);
        std::cout << std::endl;
    }
}