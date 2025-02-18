#include <iostream>
#include <unordered_map>
#include <vector>

void solve() {
    int n, k, w;
    std::cin >> n >> k >> w;
    std::vector<int> cost(n);
    for (int i = 0; i < n; i++) {
        std::cin >> cost[i];
    }
    // Maps an obtainable sum with the current maximum known to attain it;
    std::unordered_map<int, int> s_to_q;
    int cur_max = (k == cost[0]) ? 1 : 0;
    s_to_q[0] = 0;
    std::vector<int> par_sum(n, 0);
    for (int i = 0; i < w; i++) {
        int l, aux;
        std::unordered_map<int, int> cur_branch;
        cur_branch[0] = 0;
        std::cin >> l;
        for (int j = 1; j <= l; j++) {
            std::cin >> aux;
            par_sum[j] = par_sum[j - 1] + cost[aux];
            int cmpl = par_sum[j] - k;
            if (cur_branch.find(cmpl) != cur_branch.end()) 
                cur_max = std::max(cur_max, j - cur_branch[cmpl]);
            cur_branch[par_sum[j]] = j;
            
            if (par_sum[j] > k + cost[0])
                continue;

            if (par_sum[j] == k)
                cur_max = std::max(cur_max, j);

            cmpl = k - par_sum[j] + cost[0];
            if (cmpl >= 0 && s_to_q.find(cmpl) != s_to_q.end()) 
                cur_max = std::max(cur_max, j - 1 + s_to_q[cmpl]);
        }
        for (int j = 1; j <= l; j++) {
            if (par_sum[j] > k + cost[0])
                continue;
            if (s_to_q.find(par_sum[j]) != s_to_q.end()) {
                s_to_q[par_sum[j]] = std::max(j, s_to_q[par_sum[j]]);
            } else {
                s_to_q[par_sum[j]] = j;
            }
        }
    }
    std::cout << cur_max << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
