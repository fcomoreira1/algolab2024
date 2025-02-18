#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

using std::vector;
typedef vector<std::pair<long, long>> vpll;

vector<vector<vector<long>>> memo;

long dp(const vpll &v, int i, int k, int H) {
    if (i == v.size() || k == 0) {
        if (H <= 0) {
            return 0;
        } else {
            return std::numeric_limits<long>::min();
        }
    }
    H = std::max(0, H);
    if (memo[i][k][H] != -1)
        return memo[i][k][H];

    return memo[i][k][H] =
               std::max(dp(v, i + 1, k, H),
                        v[i].first + dp(v, i + 1, k - 1, H - v[i].second));
}

void solve() {
    long n, m, a, b, P, H, W;
    std::cin >> n >> m >> a >> b >> P >> H >> W;
    vpll pA(n);
    vector<long> pB(m); // Make it long for the partial sum
    memo = vector<vector<vector<long>>>(
        n, vector<vector<long>>(n + 1, vector<long>(H + 1, -1)));
    for (int i = 0; i < n; i++) {
        std::cin >> pA[i].first >> pA[i].second;
    }
    for (int i = 0; i < m; i++) {
        std::cin >> pB[i];
    }
    std::sort(pB.begin(), pB.end(), [](long a, long b) { return a > b; });
    vector<long> parsum(m);
    std::partial_sum(pB.begin(), pB.end(), parsum.begin());

    for (int i = 1, prev_max_k = 1; i <= m; i++) {
        if (parsum[i - 1] < W)
            continue;
        long max_k = (a == 0) ? n : (parsum[i - 1] - W) / a;
        long k = -1, low = prev_max_k, high = std::min(n, max_k) + 1;
        while (low < high) {
            long mid = (low + high) / 2;
            if (dp(pA, 0, mid, H) >= P + i * b) {
                k = mid + i;
                high = mid;
            } else {
                low = mid + 1;
            }
        }
        if (k != -1) {
            std::cout << k << std::endl;
            return;
        }
        prev_max_k = max_k;
    }
    std::cout << -1 << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}

