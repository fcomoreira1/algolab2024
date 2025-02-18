#include <iostream>
#include <vector>

using std::vector;
typedef vector<vector<std::pair<int, long>>> Adj;

void solve() {
    int n, m, k;
    long x;
    std::cin >> n >> m >> x >> k;
    // Last vertex is a copy of 0
    Adj adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        long p;
        std::cin >> u >> v >> p;
        adj[u].push_back({v, p});
        if (u == 0) {
            adj[n].push_back({v, p});
        }
    }
    for (int u = 0; u < n; u++) {
        for (auto [v, p] : adj[u]) {
            if (adj[v].size() == 0) {
                adj[u].push_back({n, p});
            }
        }
    }
    vector<long> best_score_dist_i(n + 1, 0);
    vector<long> update_best_score(n + 1, 0);
    int best_k = -1;
    for (int i = 1; i <= k; i++) {
        for (int u = 0; u < n + 1; u++) {
            update_best_score[u] = 0;
            for (auto [v, p] : adj[u]) {
                update_best_score[u] =
                    std::max(update_best_score[u], 
                             p + best_score_dist_i[v]);
            }
        }
        for (int u = 0; u < n + 1; u++) {
            best_score_dist_i[u] = update_best_score[u];
        }
        if (best_score_dist_i[0] >= x) {
            best_k = i;
            break;
        }
    }
    if (best_k != -1) {
        std::cout << best_k << std::endl;
    } else {
        std::cout << "Impossible" << std::endl;
    }
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}