// Includes
#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
    graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        int n, m;
        std::cin >> n >> m;
        graph G(n);
        for (int i = 0; i < m; i++) {
            int src, tgt;
            std::cin >> src >> tgt;
            boost::add_edge(src, tgt, G);
        }
        std::map<edge_desc, int> component_map;
        boost::biconnected_components(G, boost::make_assoc_property_map(component_map));
        std::vector<int> num_edges_per_comp(n, 0);
        for (auto v : component_map) {
            num_edges_per_comp[v.second]++;
        }
        std::vector<std::pair<int, int>> bridges;
        for (auto v : component_map) {
            if (num_edges_per_comp[v.second] == 1) {
                int a = boost::source(v.first, G);
                int b = boost::target(v.first, G);
                bridges.push_back({std::min(a, b), std::max(a, b)});
            }
        }
        sort(bridges.begin(), bridges.end());
        std::cout << bridges.size() << std::endl;
        for (auto e : bridges) {
            std::cout << e.first << " " << e.second << std::endl;
        }
    }
}
