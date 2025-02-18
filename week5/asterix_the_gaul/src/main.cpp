///1
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

typedef std::pair<long, long> pll;
typedef std::vector<pll> vpll;

vpll subsetsum(vpll &v, int i, int j, long bonus) {
    vpll result;
    int n = j - i;
    for (int mask = 0; mask < (1 << n); mask++) {
        pll sum(0, 0);
        for (int bit = 0; bit < n; bit++) {
            if (mask & (1 << bit)) {
                sum.first += v[i + bit].first + bonus;
                sum.second += v[i + bit].second;
            }
        }
        result.push_back(sum);
    }
    return result;
}

bool best_dist(vpll &v, long D, long T) {
    int low = 0;
    int high = v.size();
    while (low < high) {
        int mid = (low + high) / 2;
        if (v[mid].first >= D) {
            if (v[mid].second < T)
                return true;
            high = mid; 
        } else {
            low = mid + 1;
        }
    }
    return false;
}

bool test_potion(vpll &steps, long D, long T, long potion) {
    int n = steps.size();
    vpll l1 = subsetsum(steps, 0, n / 2, potion);
    vpll l2 = subsetsum(steps, n / 2, n, potion);
    std::sort(l2.begin(), l2.end(), [](pll &a, pll &b) {
        if (a.first == b.first) {
            return (a.second > b.second);
        }
        return a.first < b.first;
    });
    long min_time = std::numeric_limits<long>::max();
    for (int i = l2.size() - 1; i >= 0; i--) {
        l2[i].second = std::min(min_time, l2[i].second);
        min_time = l2[i].second;
    }
    for (auto &v : l1) {
        if (best_dist(l2, D - v.first, T - v.second)) {
            return true;
        }
    }
    return false;
}

void solve() {
    long n, m, D, T;
    std::cin >> n >> m >> D >> T;
    vpll steps(n);
    std::vector<long> potions(m + 1);
    potions[0] = 0;
    for (int i = 0; i < n; i++) {
        long d, t;
        std::cin >> d >> t;
        steps[i] = std::make_pair(d, t);
    }
    for (int i = 0; i < m; i++) {
        std::cin >> potions[i + 1];
    }
    int low = 0, high = m + 1, k = -1;
    while (low < high) {
        int mid = (high + low) / 2;
        if (test_potion(steps, D, T, potions[mid])) {
            k = mid;
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    if (k == -1) {
        std::cout << "Panoramix captured\n";
    } else {
        std::cout << k << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
