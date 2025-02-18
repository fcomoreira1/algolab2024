// STL includes
#include <iostream>
#include <unordered_map>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>
    graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Weighted
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor weighted_edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor
    weighted_vertex_desc;

using std::pair;
using std::vector;
typedef pair<int, int> pii;

// Custom edge adder class, highly recommended
class edge_adder {
    graph &G;

  public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

void solve() {
    int n, m, s, p;
    std::cin >> n >> m >> s >> p;
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    vector<vector<int>> capac(n, vector<int>(n, 0));
    for (int i = 0; i < m; i++) {
        int u, v, c, d;
        std::cin >> u >> v >> c >> d;
        // Bug prone
        auto e = boost::edge(u, v, G);
        if (e.second) {
            int cur_w = weights[e.first];
            if (cur_w > d) {
                weights[e.first] = d;
            } else if (cur_w == d) {
                c += capac[u][v];
            } else {
                c = capac[u][v];
            }
        } else {
            boost::add_edge(u, v, d, G);
        }
        capac[u][v] = c;
        capac[v][u] = c;
    }

    std::vector<int> s_dist_map(n), p_dist_map(n);

    boost::dijkstra_shortest_paths(
        G, s,
        boost::distance_map(boost::make_iterator_property_map(
            s_dist_map.begin(), boost::get(boost::vertex_index, G))));
    boost::dijkstra_shortest_paths(
        G, p,
        boost::distance_map(boost::make_iterator_property_map(
            p_dist_map.begin(), boost::get(boost::vertex_index, G))));

    // for (int i = 0; i < n; i++) {
    //     std::cout << "i: " << i << " -> " << s_dist_map[i] << ", " <<
    //     p_dist_map[i] << std::endl;
    // }

    int min_dist = s_dist_map[p];

    // std::cout << "Min dist is: " << min_dist << std::endl;

    vector<pii> to_add;
    boost::graph_traits<weighted_graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(G); ei != ei_end; ++ei) {
        weighted_vertex_desc u = boost::source(*ei, G);
        weighted_vertex_desc v = boost::target(*ei, G);
        int w = weights[*ei];
        if (s_dist_map[u] + w + p_dist_map[v] == min_dist) {
            to_add.push_back({u, v});
        }
        if (p_dist_map[u] + w + s_dist_map[v] == min_dist) {
            to_add.push_back({v, u});
        }
    }

    graph G_flow(n);
    edge_adder adder(G_flow);
    for (auto e : to_add) {
        adder.add_edge(e.first, e.second, capac[e.first][e.second]);
    }
    std::cout << boost::push_relabel_max_flow(G_flow, s, p) << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}