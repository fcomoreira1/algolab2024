// Includes
// ========
// STL includes
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    traits;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
                    boost::property<boost::edge_residual_capacity_t, long,
                                    boost::property<boost::edge_reverse_t,
                                                    traits::edge_descriptor>>>>
    graph;
// Interior Property Maps
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
    graph &G;

  public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity) {
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        const edge_desc e = boost::add_edge(from, to, G).first;
        const edge_desc rev_e = boost::add_edge(to, from, G).first;
        c_map[e] = capacity;
        c_map[rev_e] = 0; // reverse edge has no capacity!
        r_map[e] = rev_e;
        r_map[rev_e] = e;
    }
};

void add_knight_jumps(edge_adder &adder, int n, int i, int j,
                      const std::vector<bool> &is_cell) {
    std::vector<std::pair<int, int>> dirs = {
        {1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {-2, -1}, {-2, 1}, {2, -1}, {2, 1}};
    for (auto &dir : dirs) {
        std::pair<int, int> pos(dir.first + i, dir.second + j);
        if (pos.first >= 0 && pos.first < n && pos.second >= 0 &&
            pos.second < n) {
            // Check if position and valid and if it is a hole
            if (is_cell[n * pos.first + pos.second]) {
                adder.add_edge(n * i + j, n * pos.first + pos.second, 1);
            }
        }
    }
}

void solve() {
    size_t n;
    std::cin >> n;
    graph G(n * n);
    const vertex_desc src = boost::add_vertex(G);
    const vertex_desc snk = boost::add_vertex(G);
    // G is a bipartite flow graph with A having the even sum indices
    // and B having the odd sum indices. A is connected to src, B to snk
    edge_adder adder(G);

    // Empty cells are discarted from the graph like this
    std::vector<bool> is_cell(n * n, false);
    size_t num_holes = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int hole;
            std::cin >> hole;
            is_cell[i * n + j] = hole;
            num_holes += (1 - hole);
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (is_cell[i * n + j]) {
                if ((i + j) % 2 == 0) {
                    add_knight_jumps(adder, n, i, j, is_cell);
                    adder.add_edge(src, n * i + j, 1);
                } else {
                    adder.add_edge(n * i + j, snk, 1);
                }
            }
        }
    }
    int flow = boost::push_relabel_max_flow(G, src, snk);
    std::cout << n * n - flow - num_holes << std::endl;
}

int main() {
    size_t t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}