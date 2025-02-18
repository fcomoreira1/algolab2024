#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>

using std::pair;
using std::tuple;
using std::vector;
typedef vector<vector<pair<int, long>>> Adj;
// First is max val starting from the node
// Second is {total_time, num_nodes} for the subtree
typedef pair<long, long> pll;
typedef pair<long, pll> node_info;

node_info dfs(int u, const Adj &adj, const vector<long> &gold) {
    if (adj[u].size() == 0) {
        return {gold[u], {0, 1}};
    }
    vector<pll> info(adj[u].size());
    long total_val = gold[u];
    long acc_time = 0, num_vertices = 1;
    for (int i = 0; i < adj[u].size(); i++) {
        auto [v, l] = adj[u][i];
        auto [val, info_i] = dfs(v, adj, gold);
        info_i.first += 2 * l;
        info[i] = info_i;

        val -= l * info_i.second;
        total_val += val;
    }
    std::sort(info.begin(), info.end(), [](pll &a, pll& b) {
        return a.first * b.second < a.second * b.first;
    });
    for(auto [t, n] : info) {
        total_val -= n * acc_time;
        acc_time += t;
        num_vertices += n;
    }
    return {total_val, {acc_time, num_vertices}};
}

void solve() {
    int n;
    std::cin >> n;
    vector<long> gold(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        std::cin >> gold[i];
    }
    Adj adj(n + 1);
    for (int i = 0; i < n; i++) {
        int u, v, l;
        std::cin >> u >> v >> l;
        adj[u].push_back({v, l});
    }
    std::cout << dfs(0, adj, gold).first << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}