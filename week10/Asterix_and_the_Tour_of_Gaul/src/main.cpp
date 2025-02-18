#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<
        boost::edge_capacity_t, long,
        boost::property<
            boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                            boost::property<boost::edge_weight_t, long>>>>>
    graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator
                                                                   //

// Custom edge adder class
class edge_adder {
    graph &G;

  public:
    explicit edge_adder(graph &G) : G(G) {}
    void add_edge(int from, int to, long capacity, long cost) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
        w_map[e] = cost;      // new assign cost
        w_map[rev_e] = -cost; // new negative cost
    }
};

void solve() {
    int n, m;
    std::cin >> n >> m;
    graph G(2 * n);
    const vertex_desc src = boost::add_vertex(G);
    const vertex_desc snk = boost::add_vertex(G);

    edge_adder adder(G);
    // 0 is the index for the residual of 0;
    const int max_edge_cost = 128;

    int past_c = 0;
    int sum_c = 0;
    for (int i = 0; i < n - 1; i++) {
        int c;
        std::cin >> c;
        sum_c += c;
        int base = 2 * i;
        // adder.add_edge(base, base + 3, 100 - c, max_edge_cost);
        // adder.add_edge(base + 1, base + 2, c, 0);
        // adder.add_edge(base + 2, base + 4, c, max_edge_cost);
        adder.add_edge(base + 1, base + 3, c, max_edge_cost);
        adder.add_edge(base, base + 2, 100 - c, 0);
        if (c > past_c) {
            adder.add_edge(base, base + 1, c - past_c, 0);
        } else {
            adder.add_edge(base + 1, base, past_c - c, 0);
        }
        past_c = c;
    }
    int last_base = 2 * (n - 1);
    adder.add_edge(src, 0, 100, 0);
    adder.add_edge(2 * n - 2, snk, 100 - past_c, 0);
    adder.add_edge(2 * n - 1, snk, past_c, 0);

    for (int i = 0; i < m; i++) {
        int a, b, d;
        std::cin >> a >> b >> d;
        adder.add_edge(2 * a + 1, 2 * b + 1, 1, (b - a) * max_edge_cost - d);
    }
    boost::successive_shortest_path_nonnegative_weights(G, src, snk);
    int cost = boost::find_flow_cost(G);

    // auto c_map = boost::get(boost::edge_capacity, G);
    // auto r_map = boost::get(boost::edge_reverse, G);
    // auto rc_map = boost::get(boost::edge_residual_capacity, G);
    // int s_flow = 0;
    // out_edge_it e, eend;
    // for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src, G), G); e != eend; ++e) {
    //     std::cout << "edge from " << boost::source(*e, G) << " to " << boost::target(*e, G) 
    //         << " with capacity " << c_map[*e] << " and residual capacity " << rc_map[*e] << "\n";
    //     s_flow += c_map[*e] - rc_map[*e];     
    // }
    // std::cout << "s-out flow " << s_flow << "\n"; // 5
    std::cout << max_edge_cost * sum_c - cost << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}