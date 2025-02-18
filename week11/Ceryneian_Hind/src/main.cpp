// Algolab BGL Tutorial 2 (Max flow, by taubnert@ethz.ch)
// Flow example demonstrating how to use push_relabel_max_flow using a custom edge adder
// to manage the interior graph properties required for flow algorithms
#include <iostream>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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
    size_t n, m;
    std::cin >> n >> m;
    graph G(n);
    edge_adder adder(G);
    const vertex_desc src = boost::add_vertex(G);
    const vertex_desc snk = boost::add_vertex(G);
    int max_flow_from_source = 0;
    for(size_t i = 0; i < n; ++i) {
        int s;
        std::cin >> s;
        if (s > 0) {
            max_flow_from_source += s;
            adder.add_edge(src, i, s);
        } else {
            adder.add_edge(i, snk, -s);
        }
    }
    for (size_t i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        // Max capacity means this edge is not part of a minimal cut;
        adder.add_edge(u, v, 500 * 1024);
    }
    long flow = boost::push_relabel_max_flow(G, src, snk);
    if (max_flow_from_source <= flow) {
        std::cout << "impossible\n";
    } else {
        std::cout << max_flow_from_source - flow << "\n";
    }
}

int main() {
    size_t t;
    std::cin >> t;
    while(t--) {
        solve();
    }
}