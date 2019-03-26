#include <vector>
#include <map>
#include <iostream>

struct edge
{
    int a, b, cost;
    edge(const int &v_a, const int &v_b, const int &e_cost):
        a(v_a), b(v_b), cost(e_cost){}
};

using graph_t = std::map<int, std::map<int, int> >; //node to node and cost
using node_l_t = std::map<int, int>; // list of distances; node to dist
using edge_v_t = std::vector<edge>; // vector of edges
const int INF = 1000000000;



void graph_to_arr(const graph_t &graph, std::vector<edge> &edge_arr, const int &n){

    for (auto i = graph.begin(); i != graph.end(); ++i) {
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            edge_arr.push_back(edge(i->first, j->first, j->second));
        }
    }
}

void fill_dist(node_l_t &dist, const graph_t &graph){
    for(auto it = graph.begin(); it != graph.end(); ++it) {
        dist[it->first] = INF;
    }
}

void FB (const edge_v_t & edge_arr, node_l_t &dist) {
    bool change = true;
    while(change){
        change = false;
        for (int t = 0; t < edge_arr.size(); ++t){
            if (dist[edge_arr[t].a] < INF) {
                if (dist[edge_arr[t].b] > dist[edge_arr[t].a] + edge_arr[t].cost) {
                    dist[edge_arr[t].b] =  dist[edge_arr[t].a] + edge_arr[t].cost;
                    change = true;
                }
            }
        }
    }
}

void print_calcs(int v, const node_l_t &dist){
    std::cout << v << "->" << std::endl;
    for (auto it = dist.begin(); it != dist.end(); ++it){
        std::cout << it->first << ": " << it->second << ", ";
    }
}
int calc_dist(int v, int to, const graph_t &graph) {
    int n = graph.size();
    edge_v_t edge_arr;
    node_l_t dist;

    graph_to_arr(graph, edge_arr, n);
    fill_dist(dist, graph);
    dist[v] = 0;

    FB(edge_arr, dist);
    print_calcs(v, dist);
    return dist[to];
}

void enter_graph(graph_t &graph) {
    int a, b, c;
    int m, n;
    std::cout << "enter number of nodes:" << std::endl;
    std::cin >> n;
    std::cout << "enter the nodes:" << std::endl;
    for(int t = 0; t < n; ++t) {
        std::cin >> a;
        graph[a] = std::map <int, int> ();
    }
    std::cout << "enter number of edges:" << std::endl;
    std::cin >> m;

    std::cout << "enter the edges from, to and cost:" << std::endl;
    for(int t = 0; t < m; t++) {
        std::cin >> a >> b >> c;
        graph[a][b] = c;
    }
}

void print_graph(const graph_t &graph) {
    for (auto i = graph.begin(); i != graph.end(); ++i) {
        std::cout << i->first << ":" << std::endl;
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            std::cout << j->first << " weight " << j->second << std::endl; 
        }
    }
}

void print_distances(const graph_t &graph) {
    for (auto it = graph.begin(); it != graph.end(); ++it){
        calc_dist(it->first, it->first, graph);
        std::cout << std::endl;
    }
}
int main(void) {
    graph_t  graph;
    enter_graph(graph);
    print_distances(graph);
}