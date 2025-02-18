#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/detail/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <iostream>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s) {
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);

    boost::dijkstra_shortest_paths(
        G, s,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));

    return *std::max_element(dist_map.begin(), dist_map.end());
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        int n, m;
        std::cin >> n >> m;
        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);
        for (int i = 0; i < m; i++) {
            int a, b, w;
            std::cin >> a >> b >> w;
            edge_desc e;
            e = boost::add_edge(a, b, G).first;
            weights[e] = w;
        }
        std::vector<edge_desc> mst;
        boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
        int total_weight = 0;
        for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
            total_weight += weights[*it];
        }
        std::cout << total_weight << " " << dijkstra_dist(G, 0) << std::endl;
    }
}
