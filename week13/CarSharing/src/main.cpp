/// 5
#include <iostream>
#include <limits>
#include <vector>
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
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; // Iterator
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

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

struct Booking {
    int s, t, d, a, profit;
};

void solve() {
    int N, S;
    std::cin >> N >> S;
    std::vector<int> l(S);
    std::vector<Booking> reserv(N);
    std::vector<std::map<int, int>> station_time_node(S, std::map<int, int>());

    int total_in_flow = 0;
    for (int i = 0; i < S; i++) {
        std::cin >> l[i];
        total_in_flow += l[i];
    }
    int node_count = 0;
    int max_timestamp = 0;
    int max_profit = 100;
    for (int i = 0; i < N; i++) {
        std::cin >> reserv[i].s >> reserv[i].t >> reserv[i].d >> reserv[i].a >>
            reserv[i].profit;
        reserv[i].s--;
        reserv[i].t--;
        station_time_node[reserv[i].s].insert({reserv[i].d, node_count++});
        station_time_node[reserv[i].t].insert({reserv[i].a, node_count++});
        max_timestamp = std::max(max_timestamp, reserv[i].a);
        max_profit = std::max(reserv[i].profit, max_profit);
    }
    for (int i = 0; i < S; i++) {
        station_time_node[i].insert({0, node_count++});
        station_time_node[i].insert({max_timestamp, node_count++});
    }

    graph G(node_count);
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    edge_adder adder(G);

    for (int i = 0; i < S; i++) {
        int prev_timestamp = 0;
        int prev_node = v_source;
        int flow = l[i];
        for (auto entry : station_time_node[i]) {
            int cur_timestamp = entry.first;
            int cur_node = entry.second;
            adder.add_edge(prev_node, cur_node, flow,
                           (cur_timestamp - prev_timestamp) * max_profit);
            flow = total_in_flow;
            prev_timestamp = cur_timestamp;
            prev_node = cur_node;
        }
        adder.add_edge(station_time_node[i][max_timestamp], v_sink,
                       total_in_flow, 0);
    }
    for (auto bk : reserv) {
        adder.add_edge(station_time_node[bk.s][bk.d],
                       station_time_node[bk.t][bk.a], 1,
                       (bk.a - bk.d) * max_profit - bk.profit);
    }
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);
    std::cout << total_in_flow * max_profit * max_timestamp - cost << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
