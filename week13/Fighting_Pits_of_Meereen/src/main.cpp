#include <algorithm>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

using namespace std;
typedef vector<vector<unordered_map<string, int>>> Cache;

int dp(int i, int diff, string past_vals, Cache &cache, const vector<int> &v,
       const int m) {
    int n = (int)cache.size();
    if (i == n)
        return 0;
    if (cache[i][diff + 12].find(past_vals) != cache[i][diff+12].end()) {
        return cache[i][diff + 12][past_vals];
    }
    // Extract from string
    char last_q0 = past_vals[0];
    char scnd_q0 = past_vals[1];
    char last_q1 = past_vals[2];
    char scnd_q1 = past_vals[3];
    int max_ans = std::numeric_limits<int>::min();

    int variety_q1 = 0, variety_q0 = 0;
    if (m == 2) {
        variety_q0 = 1 + (last_q0 != v[i] && (last_q0 != '0'));
        variety_q1 = 1 + (last_q1 != v[i] && (last_q1 != '0'));
    } else {
        variety_q0 =
            3 - (scnd_q0 == '0') -
            (last_q0 == scnd_q0 || last_q0 == v[i] || scnd_q0 == v[i]) -
            (last_q0 == scnd_q0 && scnd_q0 == v[i]);
        variety_q1 =
            3 - (scnd_q1 == '0') -
            (last_q1 == scnd_q1 || last_q1 == v[i] || scnd_q1 == v[i]) -
            (last_q1 == scnd_q1 && scnd_q1 == v[i]);
    }

    int exp_0 = 1000 * variety_q0 - (1 << abs(diff - 1));
    int exp_1 = 1000 * variety_q1 - (1 << abs(diff + 1));
    if (exp_0 >= 0) {
        past_vals[0] = v[i];
        past_vals[1] = last_q0;
        max_ans =
            max(max_ans, exp_0 + dp(i + 1, diff - 1, past_vals, cache, v, m));
        past_vals[0] = last_q0;
        past_vals[1] = scnd_q0;
    }
    if (exp_1 >= 0) {
        past_vals[2] = v[i];
        past_vals[3] = last_q1;
        max_ans =
            max(max_ans, exp_1 + dp(i + 1, diff + 1, past_vals, cache, v, m));
        past_vals[2] = last_q1;
        past_vals[3] = scnd_q1;
    }
    return cache[i][diff + 12][past_vals] = max_ans;
}

void solve() {
    int n, k, m;
    cin >> n >> k >> m;
    Cache cache(n, vector<unordered_map<string, int>>(24, unordered_map<string, int>()));
    vector<int> v(n);
    for(int i = 0; i < n; i++) {
        cin >>v[i];
        v[i]++;
    }
    cout << dp(0, 0, "0000", cache, v, m) << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
}