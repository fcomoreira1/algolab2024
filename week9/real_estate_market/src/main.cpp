// ALGOLAB BGL Tutorial 3
// Code demonstrating
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using
// successive_shortest_path_nonnegative_weights

// Includes
// ========
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
    int N, M, S;
    std::cin >> N >> M >> S;
    graph G(N + M + S);
    edge_adder adder(G);
    const vertex_desc src = boost::add_vertex(G);
    const vertex_desc snk = boost::add_vertex(G);
    for (int i = 0; i < S; i++) {
        int l;
        std::cin >> l;
        adder.add_edge(N + M + i, snk, l, 0);
    }
    for (int i = 0; i < M; i++) {
        int s;
        std::cin >> s;
        adder.add_edge(N + i, N + M + s - 1, 1, 0);
    }
    const int MAX_COST = 100 * 100 + 1;
    for (int i = 0; i < N; i++) {
        adder.add_edge(src, i, 1, 0);
        for (int j = 0; j < M; j++) {
            int c;
            std::cin >> c;
            adder.add_edge(i, N + j, 1, MAX_COST - c);
        }
    }
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    boost::successive_shortest_path_nonnegative_weights(G, src, snk);
    int cost = boost::find_flow_cost(G);
    // Iterate over all edges leaving the source to sum up the flow values.
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src, G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
    std::cout << s_flow << " " << s_flow * MAX_COST - cost << std::endl;
}

int main() {
    size_t t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
