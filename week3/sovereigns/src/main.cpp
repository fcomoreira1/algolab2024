///1
#include <iostream>
#include <limits>
#include <vector>

std::vector<int> x;
std::vector<std::vector<int>> memo;

// Solves the problem assuming k = 0;
int solve_fp(int i, int j, int m) {
    if (i > j - 1)
        return 0;
    if (memo[i][j] != -1)
        return memo[i][j];
    int best_left = std::numeric_limits<int>::max();
    int best_right = std::numeric_limits<int>::max();
    for(int l = 0; l < m; l++) {
        best_left = std::min(best_left, solve_fp(i + l + 1, j - m + l + 1, m));
        best_right = std::min(best_right, solve_fp(i + l, j - m + l, m));
    }
    return memo[i][j] = std::max(x[i] + best_left, x[j - 1] + best_right);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        int n, m, k;
        std::cin >> n >> m >> k;
        x.clear();
        x.reserve(n);
        memo.clear();
        memo.assign(n + 1, std::vector<int>(n + 1, -1));
        int sum = 0;
        for (int i = 0; i < n; i++) {
            std::cin >> x[i];
            sum += x[i];
        }

        int res = std::numeric_limits<int>::max();
        for (int i = 0; i <= k; i++) {
            res = std::min(res, solve_fp(i, n - k + i, m));
        }
        std::cout << res << std::endl;
    }
}
