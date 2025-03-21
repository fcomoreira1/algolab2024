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
    int n, m;
    std::cin >> n >> m;
    graph G(n);
    edge_adder adder(G);
    for(int i = 0; i < m; i++) {
        int a, b, c;
        std::cin >> a >> b >> c;
        adder.add_edge(a, b, c);
    }
    int min_cut, best_min_cut = std::numeric_limits<int>::max();
    for (int i = 1; i < n; i++) {
        min_cut = boost::push_relabel_max_flow(G, 0, i);
        best_min_cut = std::min(best_min_cut, min_cut);
        min_cut = boost::push_relabel_max_flow(G, i, 0);
        best_min_cut = std::min(best_min_cut, min_cut);
    }
    std::cout << best_min_cut << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t = 0;
    std::cin >> t;
    while(t--) {
        solve();
    }
}
