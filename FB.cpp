#include <vector>
#include <iostream>

struct edge
{
    int a, b, cost;
    edge(const int &v_a, const int &v_b, const int &e_cost):
        a(v_a), b(v_b), cost(e_cost){}
};


void graph_to_arr( std::vector<std::pair<int, int> >  *graph, 
    std::vector<edge> &edge_arr, const int &n){

    for (int i = 0; i < n; ++i) {
        for (auto j = graph[i].begin(); j < graph[i].end(); ++j) {
            edge_arr.push_back(edge(i,  j->first, j->second));
        }
    }
}

const int INF = 1000000000;
int solve(int n, int v, int to,  std::vector<std::pair<int, int> >  *graph ) {
    std::vector<edge> edge_arr;
    graph_to_arr(graph, edge_arr, n);
    std::vector<int> dist (n, INF);
    dist[v] = 0;
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
    std::cout << v + 1 << "->" << std::endl;
    for (int t = 0; t < n; t++) {
        std::cout << t + 1 << ": " << dist[t] << ", ";
    }
    return dist[to];
}

int main(void) {
    int n;
    std::cin >> n;
    std::vector<std::pair<int, int> > graph[n];
    int a, b, c;
    int m;
    std::cin >> m;
    for(int t = 0; t < m; t++) {
        std::cin >> a >> b >> c;
        graph[a - 1].push_back(std::make_pair(b - 1, c));
    }
    for (int t = 0; t < n; ++t)
    {
        solve(n, t, 1, graph);
        std::cout << std::endl;
    }
}