#include <iostream>
#include <limits>
#include <vector>

using namespace std;

int inter_pttn(vector<int> pttn1, vector<int> pttn2) {
    int k = pttn1.size();
    int i = 1;
    for (; i < k; i++) {
        bool flag = true;
        for (int j = 0; j < k - i; j++) {
            flag = flag & (pttn1[j + i] == pttn2[j]);
        }
        if (flag)
            break;
    }
    return k - i;
}

int dp(int i, int last_idx, vector<vector<int>> &cache,
       const vector<vector<vector<int>>> &err) {
    int n = (int)err.size();
    if (i == n) {
        return 0;
    } else if (cache[i][last_idx] != -1) {
        return cache[i][last_idx];
    }
    int ans = 1000000;
    for (int j = 0; j < err[i].size(); j++) {
        int k = err[i][j].size();
        int intersection =
            (i != 0 ? inter_pttn(err[i - 1][last_idx], err[i][j]) : 0);
        bool flag = true;
        for (int h = intersection; h < k; h++) {
            if (err[i][j][h] < i){
                flag = false;
                break;
            }
        }
        if (flag) {
            ans = min(ans, k - intersection + dp(i + 1, j, cache, err));
        }
    }
    return cache[i][last_idx] = ans;
}

void solve() {
    int n, m, k, d;
    cin >> n >> m >> k >> d;
    vector<vector<vector<int>>> err(n);
    vector<int> pttn(k);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            cin >> pttn[j];
        }
        err[pttn[k - 1]].push_back(pttn);
    }
    vector<vector<int>> cache(n, vector<int>(d, -1));
    int ans = dp(0, 0, cache, err);
    if (ans > 100000) {
        std::cout << "Impossible!\n";
    } else {
        std::cout << ans << endl;
    }
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
}