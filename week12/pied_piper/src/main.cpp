#include <iostream>
#include <limits>
#include <vector>

using std::vector;
typedef vector<vector<std::pair<int, int>>> Adj;

int dp(vector<vector<int>> &memo, const Adj &f_adj, const Adj &b_adj, int in,
       int out) {
    // Highest score in a path that goes from in to end and end to out without
    // crossing
    int n = memo.size();
    // std::cout << in << " " << out << std::endl;
    if (memo[in][out] != -1) {
        return memo[in][out];
    }
    int ans = std::numeric_limits<int>::min();
    if (in == n - 1 && out == n - 1) {
        return 0;
    }
    int maximum = std::max(in, out);
    if (maximum == n - 1) {
        if (in == maximum) {
            for (auto v : b_adj[out]) {
                if (v.first == n - 1) {
                    ans = std::max(ans, v.second);
                }
            }
        } else {
            for (auto v : f_adj[in]) {
                if (v.first == n - 1) {
                    ans = std::max(ans, v.second);
                }
            }
        }
    }
    for (auto v : f_adj[in]) {
        if (v.first > out) {
            ans =
                std::max(ans, v.second + dp(memo, f_adj, b_adj, v.first, out));
        }
    }
    for (auto v : b_adj[out]) {
        if (v.first > in) {
            ans = std::max(ans, v.second + dp(memo, f_adj, b_adj, in, v.first));
        }
    }
    return memo[in][out] = ans;
}
void solve() {
    int n, m;
    std::cin >> n >> m;
    Adj f_graph(n), b_graph(n);
    for (int i = 0; i < m; i++) {
        int u, v, f;
        std::cin >> u >> v >> f;
        if (u < v) {
            f_graph[u].push_back({v, f});
        } else {
            b_graph[v].push_back({u, f});
        }
    }
    vector<vector<int>> memo(n, vector<int>(n, -1));
    std::cout << dp(memo, f_graph, b_graph, 0, 0) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
