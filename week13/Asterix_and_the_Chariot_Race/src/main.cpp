#include <iostream>
#include <limits>
#include <vector>

using std::vector;
typedef vector<vector<int>> Adj;

int dp(int i, int status, const Adj &adj, const vector<int> &c,
       vector<vector<int>> &cache) {
    int n = (int)c.size();
    if (cache[i][status] != -1)
        return cache[i][status];
    int include_current = c[i];
    for (auto j : adj[i]) {
        include_current += dp(j, 0, adj, c, cache);
    }
    int not_include_current = 0;
    if (status == 0) {
        for (auto j : adj[i]) {
            not_include_current += dp(j, 1, adj, c, cache);
        }
    } else if (status == 1) {
        int min_ans = std::numeric_limits<int>::max();
        int total_sum = 0;
        for (auto j : adj[i]) {
            total_sum += dp(j, 1, adj, c, cache);
        }
        for (auto j : adj[i]) {
            int cur_ans = total_sum + dp(j, 2, adj, c, cache) - dp(j, 1, adj, c, cache);
            min_ans = std::min(cur_ans, min_ans);
        }
        not_include_current = min_ans;
    } else if (status == 2) {
        not_include_current = std::numeric_limits<int>::max();
    }
    // std::cout << i << " " << status << " " << include_current << " " << not_include_current << std::endl;
    return cache[i][status] = std::min(include_current, not_include_current);
}

void solve() {
    int n;
    std::cin >> n;
    Adj adj(n);
    vector<int> c(n);
    vector<vector<int>> cache(n, vector<int>(3, -1));
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
    }
    for (int i = 0; i < n; i++) {
        std::cin >> c[i];
    }
    std::cout << dp(0, 1, adj, c, cache) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}