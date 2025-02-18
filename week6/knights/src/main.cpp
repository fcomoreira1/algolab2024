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
    int m, n, k, c;
    std::cin >> m >> n >> k >> c;
    graph G(2 * m * n);
    edge_adder adder(G);
    const vertex_desc src = boost::add_vertex(G);
    const vertex_desc snk = boost::add_vertex(G);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int v_in = i * m + j;
            int v_out = n * m + v_in;
            adder.add_edge(v_in, v_out, c);
            if (i > 0)
                adder.add_edge(v_out, v_in - m, 1);
            if (i < n - 1)
                adder.add_edge(v_out, v_in + m, 1);
            if (j > 0)
                adder.add_edge(v_out, v_in - 1, 1);
            if (j < m - 1)
                adder.add_edge(v_out, v_in + 1, 1);

            int num_exits = (i == 0) + (i == n - 1) + (j == 0) + (j == m - 1);
            if (num_exits > 0)
                adder.add_edge(v_out, snk, num_exits);
        }
    }
    for (int i = 0; i < k; i++) {
        int a, b;
        std::cin >> a >> b;
        adder.add_edge(src, b * m + a, 1);
    }
    std::cout << boost::push_relabel_max_flow(G, src, snk) << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
