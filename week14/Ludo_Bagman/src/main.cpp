#include <iostream>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

// Graph Type with nested interior edge properties for flow algorithms
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
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator
typedef traits::vertex_descriptor vertex_desc;

using std::vector;
typedef vector<vector<int>> Adj;
const int MAX_INT = std::numeric_limits<int>::max();

// Custom edge adder class, highly recommended
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
;

void solve() {
    int e, w, m, d, p, l;
    std::cin >> e >> w >> m >> d >> p >> l;
    graph G(2 * e + 2 * w);
    const vertex_desc src_ess = boost::add_vertex(G);
    const vertex_desc src_ext = boost::add_vertex(G);
    const vertex_desc src = boost::add_vertex(G);
    const vertex_desc snk_ess = boost::add_vertex(G);
    const vertex_desc snk_ext = boost::add_vertex(G);
    const vertex_desc snk = boost::add_vertex(G);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

    adder.add_edge(src, src_ess, l * e, 0);
    adder.add_edge(src, src_ext, p - l * e, 0);

    adder.add_edge(snk_ess, snk, l * w, 0);
    adder.add_edge(snk_ext, snk, p - l * w, 0);

    const int skip_w = 2 * e;

    for (int i = 0; i < e; i++) {
        int ind = 2 * i;
        // Essential
        adder.add_edge(src_ess, ind, l, 0);
        // Extra
        adder.add_edge(src_ext, ind, p, 0);
        // Bad
        adder.add_edge(src_ext, ind + 1, p, 0);
    }
    for (int i = 0; i < w; i++) {
        int ind = skip_w + 2 * i;
        // Essential
        adder.add_edge(ind, snk_ess, l, 0);
        // Extra
        adder.add_edge(ind, snk_ext, p, 0);
        // Bad
        adder.add_edge(ind + 1, snk_ext, p, 0);
    }

    for (int i = 0; i < m; i++) {
        int u, v, r;
        std::cin >> u >> v >> r;
        int u_ind = 2 * u;
        int v_ind = skip_w + 2 * v;
        adder.add_edge(u_ind, v_ind, 1, r);
    }
    for (int i = 0; i < d; i++) {
        int u, v, r;
        std::cin >> u >> v >> r;
        int u_ind = 2 * u + 1;
        int v_ind = skip_w + 2 * v + 1;
        adder.add_edge(u_ind, v_ind, 1, r);
    }
    if (p >= l * e && p >= l * w) {
        boost::successive_shortest_path_nonnegative_weights(G, src, snk);
        int cost2 = boost::find_flow_cost(G);
        int s_flow = 0;
        out_edge_it e, eend;
        for (boost::tie(e, eend) =
                 boost::out_edges(boost::vertex(src, G), G);
             e != eend; ++e) {
            s_flow += c_map[*e] - rc_map[*e];
        }
        if (s_flow == p) {
            std::cout << cost2 << std::endl;
        } else {
            std::cout << "No schedule!\n";
        }
    } else {
        std::cout << "No schedule!\n";
    }
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}