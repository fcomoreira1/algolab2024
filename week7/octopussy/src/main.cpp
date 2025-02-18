#include <iostream>
#include <queue>

typedef std::pair<int, int> pii;
using std::vector;

void solve() {
    int n;
    std::cin >> n;
    vector<int> t(n);
    for (int i = 0; i < n; i++) {
        std::cin >> t[i];
    }
    for (int i = 0; i <= (n - 3) / 2; i++) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        t[l] = std::min(t[l], t[i] - 1);
        t[r] = std::min(t[r], t[i] - 1);
    }
    vector<int> standing(n, 2);
    std::priority_queue<pii, vector<pii>, std::greater<pii>> pq;
    for (int i = (n - 1) / 2; i < n; i++) {
        pq.push({t[i], i});
    }
    int cur_timer = 0;
    while (!pq.empty()) {
        cur_timer++;
        pii top = pq.top();
        pq.pop();
        if (top.first < cur_timer) {
            std::cout << "no\n";
            return;
        }
        int idx_up = (top.second - 1) / 2;
        standing[idx_up]--;
        if (standing[idx_up] == 0) {
            pq.push({t[idx_up], idx_up});
        }
    }
    std::cout << "yes\n";
}

int main() {
    int t;
    std::cin >> t;
    while(t--) {
        solve();
    }
}
