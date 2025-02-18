// STL includes
#include <iostream>
#include <limits>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
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
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        int n, m, s, a, b;
        std::cin >> n >> m >> s >> a >> b;
        std::vector<weighted_graph> gs(s, weighted_graph(n));
        for (int i = 0; i < m; i++) {
            int t1, t2, w;
            std::cin >> t1 >> t2;
            for (int j = 0; j < s; j++) {
                std::cin >> w;
                boost::add_edge(t1, t2, w, gs[j]);
            }
        }
        for (int i = 0; i < s; i++) {
            int aux;
            std::cin >> aux;
        }
        weighted_graph G(n);
        for (int i = 0; i < s; i++) {
            std::vector<edge_desc> mst;
            auto weight = boost::get(boost::edge_weight, gs[i]);
            boost::kruskal_minimum_spanning_tree(gs[i],
                                                 std::back_inserter(mst));
            for (edge_desc e : mst) {
                boost::add_edge(boost::source(e, gs[i]),
                                boost::target(e, gs[i]), weight[e], G);
            }
        }
        std::cout << dijkstra_dist(G, a, b) << std::endl;
    }
}
