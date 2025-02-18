#include <algorithm>
#include <iostream>
#include <vector>

bool test_k(std::vector<int> &c, std::vector<int> &w, int k) {
    int n = c.size(), m = w.size();
    int i = 0, j = 0, counter = 0;
    while (i < n && j < m) {
        if (counter == k || c[i] < w[j]) {
            counter = 0;
            i++;
        }
        if (i < n && c[i] >= w[j]) {
            counter++;
            j++;
        }     
    }
    return j == m;
}

void solve() {
    int n, m;
    std::cin >> n >> m;
    std::vector<int> c(n), w(m);
    for (int i = 0; i < n; i++)
        std::cin >> c[i];
    for (int i = 0; i < m; i++)
        std::cin >> w[i];
    std::sort(c.begin(), c.end());
    std::sort(w.begin(), w.end());
    if (c[n - 1] < w[m - 1]){
        std::cout << "impossible" << std::endl;
        return;
    }
    // Binary Search for k
    int low = 1, high = m + 1;
    int k = -1;
    while (low < high) {
        int mid = (high + low) / 2;
        if (test_k(c, w, mid)) {
            k = mid;
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    std::cout << 3 * k - 1 << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
