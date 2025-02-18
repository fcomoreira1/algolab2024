#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <iostream>
#include <limits>

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

class edge_adder {
    graph &G;
    boost::property_map<graph, boost::edge_capacity_t>::type c_map;
    boost::property_map<graph, boost::edge_reverse_t>::type r_map;

  public:
    explicit edge_adder(graph &G) : G(G) {
        c_map = boost::get(boost::edge_capacity, G);
        r_map = boost::get(boost::edge_reverse, G);
    }

    void add_edge(int from, int to, long capacity) {
        const auto e = boost::add_edge(from, to, G).first;
        const auto rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

void solve() {
    int l, p;
    std::cin >> l >> p;
    graph G(l);
    edge_adder adder(G);
    const vertex_desc src = boost::add_vertex(G);
    const vertex_desc snk = boost::add_vertex(G);
    // Keeps the supply and demand of each node;
    std::vector<std::pair<int, int>> gd(l);
    for (int i = 0; i < l; i++) {
        int s, d;
        std::cin >> s >> d;
        gd[i] = {s, d};
    }
    int total_s = 0;
    for (int i = 0; i < p; i++) {
        int u, v, cmin, cmax;
        std::cin >> u >> v >> cmin >> cmax;
        // Converting minimal flow to supply and demand info
        gd[u].second += cmin;
        gd[v].first += cmin;
        adder.add_edge(u, v, cmax - cmin);
        total_s += cmax - cmin;
    }
    int total_demand = 0;
    for (int i = 0; i < l; i++) {
        // We connect to source if supply > demand, to sink otherwise
        if (gd[i].first > gd[i].second) {
            adder.add_edge(src, i, gd[i].first - gd[i].second);
        } else if (gd[i].first < gd[i].second) {
            adder.add_edge(i, snk, gd[i].second - gd[i].first);
            total_demand += gd[i].second - gd[i].first;
        }
    }
    if (total_demand == boost::push_relabel_max_flow(G, src, snk)) {
        std::cout << "yes\n";
    } else {
        std::cout << "no\n";
    }
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
