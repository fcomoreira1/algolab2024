#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(
        G, s,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));

    return dist_map[t];
}
int main() {
    int t;
    std::cin >> t;
    while (t--) {
        int n, m, k, beg, end;
        std::cin >> n >> m >> k >> beg >> end;
        weighted_graph G(n * (k + 1));
        for (int i = 0; i < m; i++) {
            int u, v, w;
            bool is_river;
            std::cin >> u >> v >> w >> is_river;
            if (is_river) {
                for (int j = 0; j < k; j++) {
                    boost::add_edge(u + j * n, v + (j + 1) * n, w, G);
                    boost::add_edge(v + j * n, u + (j + 1) * n, w, G);
                }
                int shift = k * n;
                boost::add_edge(u + shift, v + shift, w, G);
                boost::add_edge(v + shift, u + shift, w, G);
            } else {
                for (int j = 0; j <= k; j++) {
                    boost::add_edge(u + j * n, v + j * n, w, G);
                    boost::add_edge(v + j * n, u + j * n, w, G);
                }
            }
        }
        std::cout << dijkstra_dist(G, beg, end + k * n) << std::endl;
    }
}
