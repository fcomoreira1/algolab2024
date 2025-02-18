#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/find_flow_cost.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>

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
using std::pair;
using std::vector;
typedef vector<vector<vector<int>>> Patterns;

const int MAX_ANS = 100 * 1000;

int dp(int i, int idx_cur_pttn, vector<vector<int>> &memo,
       const Patterns &pttns) {
    int n = (int)pttns.size();
    if (i == n) {
        return 0;
    }
    if (memo[i][idx_cur_pttn] != -1) {
        return memo[i][idx_cur_pttn];
    }
    vector<int> cur_pttn;
    if (i >= 1) {
        cur_pttn = pttns[i - 1][idx_cur_pttn];
    }
    int best = MAX_ANS;
    for (int j = 0; j < pttns[i].size(); j++) {
        const vector<int> &p = pttns[i][j];
        int k = p.size();
        int intersec = (i >= 1 ? k - 1 : 0);
        for (; intersec > 0; intersec--) {
            bool coincide = true;
            for (int j = 0; j < intersec; j++) {
                if (cur_pttn[j + k - intersec] != p[j]) {
                    coincide = false;
                    break;
                }
            }
            if (coincide) {
                break;
            }
        }
        bool is_valid = true;
        for (int jj = intersec; jj < k; jj++) {
            is_valid = is_valid && (p[jj] >= i);
        }
        if (is_valid) {
            best = std::min(best, k - intersec + dp(i + 1, j, memo, pttns));
        }
    }
    return memo[i][idx_cur_pttn] = best;
}

void solve() {
    int n, m, k, d;
    std::cin >> n >> m;
    std::cin >> k >> d;
    Patterns pttns(n);
    for (int i = 0; i < m; i++) {
        std::vector<int> v(k);
        for (int j = 0; j < k; j++) {
            std::cin >> v[j];
        }
        pttns[v[k - 1]].push_back(v);
    }
    vector<vector<int>> memo(n, std::vector<int>(d, -1));
    int ans = dp(0, 0, memo, pttns);
    if (ans == MAX_ANS) {
        std::cout << "Impossible!" << std::endl;
    } else {
        std::cout << ans << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}