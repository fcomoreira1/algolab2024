#include <iostream>
#include <queue>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    graph;
typedef boost::property_map<graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

int maximum_matching(const graph &G) {
    int n = boost::num_vertices(G);
    std::vector<vertex_desc> mate_map(n); // exterior property map
    boost::edmonds_maximum_cardinality_matching(
        G, boost::make_iterator_property_map(
               mate_map.begin(), boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(
        G, boost::make_iterator_property_map(
               mate_map.begin(), boost::get(boost::vertex_index, G)));
    return matching_size;
}

void solve() {
    int n, m, b, p;
    long d;
    std::cin >> n >> m >> b >> p >> d;
    graph G(2 * n + 1);
    weight_map weights = boost::get(boost::edge_weight, G);
    typedef std::pair<long, vertex_desc> dist_vtx;
    std::vector<bool> is_plaza(n, false);
    for (int i = 0; i < b; i++) {
        vertex_desc a;
        std::cin >> a;
        boost::add_edge(2 * n, a, 0, G);
    }
    for (int i = 0; i < p; i++) {
        vertex_desc q;
        std::cin >> q;
        is_plaza[q] = true;
    }
    for (int i = 0; i < m; i++) {
        vertex_desc x, y;
        int l;
        std::cin >> x >> y >> l;
        boost::add_edge(x, y, l, G);
        // x and y cannot be both plazas.
        if (is_plaza[x]) {
            boost::add_edge(x + n, y, l, G);
        }
        if (is_plaza[y]) {
            boost::add_edge(x, y + n, l, G);
        }
    }
    std::vector<long> dist_map(2 * n + 1);

    boost::dijkstra_shortest_paths(
        G, 2 * n,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));
    
    std::vector<bool> reachable(2 * n, false);
    for (int i = 0; i < 2 * n; i++) {
        if (dist_map[i] <= d){
            reachable[i] = true;
        }
    }

    graph G_filtered(2 * n);
    edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
        int u = boost::source(*ebeg, G), v = boost::target(*ebeg, G);
        if (reachable[u] && reachable[v])
            boost::add_edge(u, v, G_filtered);
    }
    std::cout << maximum_matching(G_filtered) << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}