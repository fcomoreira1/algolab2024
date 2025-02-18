#include <iostream>
#include <set>
#include <vector>

using std::multiset;
using std::set;
using std::vector;

// bool dfs(int i, int n, int m, int k, const vector<int> &h,
//          const vector<vector<int>> &adj, set<int> &valid_starts,
//          multiset<int> &vals) {
//     auto min_el = *vals.begin();
//     auto max_el = *vals.rbegin();
//     if (max_el - min_el <= k && vals.size() == m) {
//         valid_starts.insert(i);
//         return true;
//     }

// }

void solve() {
    int n, m, k;
    std::cin >> n >> m >> k;
    vector<int> adj(n, -1);
    vector<int> deg(n, 0);
    vector<int> h(n);
    for (int i = 0; i < n; i++) {
        std::cin >> h[i];
    }
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        adj[v] = u;
    }
    set<int> valid_starts;
    vector<int> is_processed(n, 0);
    for (int ii = 0; ii < n; ii++) {
        if (deg[ii] != 1) {
            std::multiset<int> vals;
            int i = ii;
            int j = i;
            vals.insert(h[i]);

            while (true) {
                if (is_processed[i])
                    break;
                if (vals.size() == m) {
                    is_processed[i] = 1;
                }
                auto min_el = *vals.begin();
                auto max_el = *vals.rbegin();
                if (max_el - min_el <= k && vals.size() == m) {
                    valid_starts.insert(j);
                }
                j = adj[j];
                if (j == -1)
                    break;
                vals.insert(h[j]);

                while (vals.size() > m) {
                    auto it = vals.find(h[i]);
                    if (it != vals.end()) {
                        vals.erase(it); // Remove only the found occurrence
                    }
                    i = adj[i];
                }
            }
        }
    }
    if (valid_starts.size() > 0) {
        for (auto &i : valid_starts) {
            std::cout << i << " ";
        }
    } else {
        std::cout << "Abort mission";
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}