#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <iostream>

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
    int n, m;
    std::cin >> n >> m;
    graph G(n + m);
    edge_adder adder(G);
    const vertex_desc src = boost::add_vertex(G);
    const vertex_desc snk = boost::add_vertex(G);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        std::cin >> a >> b >> c;
        adder.add_edge(n + i, a, (int)(c == 0 || c == 1));
        adder.add_edge(n + i, b, (int)(c == 0 || c == 2));
        adder.add_edge(src, n + i, 1);
    }
    int total_points = 0;
    for (int i = 0; i < n; i++) {
        int s;
        std::cin >> s;
        adder.add_edge(i, snk, s);
        total_points += s;
    }
    if (total_points != m ||
        total_points != boost::push_relabel_max_flow(G, src, snk)) {
        std::cout << "no\n";
        return;
    }
    std::cout << "yes\n";
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
