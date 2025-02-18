#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

typedef vector<vector<int>> Adj;
typedef pair<int, int> Edge;
typedef pair<int, Edge> Wedge;

long kruskal(int n, const vector<Wedge> &edges, int i, int j) {
    boost::disjoint_sets_with_storage<> ds(n);
    long mst_weight = 0;
    int num_comp = n;
    for (auto &edge : edges) {
        if (num_comp == 1)
            break;
        int u = edge.second.first;
        int v = edge.second.second;
        if (u == i && v == j) {
            continue;
        }
        int weight = edge.first;
        int c1 = ds.find_set(u);
        int c2 = ds.find_set(v);
        if (c1 != c2) {
            ds.link(c1, c2);
            mst_weight += (long)weight;
            num_comp--;
        }
    }
    return mst_weight;
}
void solve() {
    int n, tat, c;
    cin >> n >> tat;
    tat--;
    Adj cost(n, vector<int>(n));
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            cin >> cost[i][j];
            cost[j][i] = cost[i][j];
        }
    }

    // Build Maximal ST graph
    auto cmp = [](Wedge &a, Wedge &b) {
        if (a.first != b.first)
            return a.first > b.first;
        else if (a.second.first != b.second.first) {
            return a.second.first < b.second.first;
        } else {
            return a.second.second < b.second.second;
        }
    };
    // Assumes `el.second.first` is in the set and `el.second.second` is outside
    priority_queue<Wedge, vector<Wedge>, decltype(cmp)> pq(cmp);
    for (int i = 0; i < n; i++) {
        if (i == tat)
            continue;
        pq.push({cost[tat][i], {tat, i}});
    }
    vector<int> visited(n, 0);
    visited[tat] = 1;
    vector<Edge> edges;
    int remaining_to_add = n - 1;
    while (remaining_to_add > 0) {
        Wedge top = pq.top();
        int w = top.first;
        Edge e = top.second;
        pq.pop();
        if (visited[e.second] != 0)
            continue;
        visited[e.second] = 1;
        edges.push_back({e.first, e.second});
        remaining_to_add--;
        for (int i = 0; i < n; i++) {
            if (visited[i] == 0) {
                pq.push({cost[e.second][i], {e.second, i}});
            }
        }
    }
    long min_st = numeric_limits<long>::max();
    vector<Wedge> sorted_wedges;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            sorted_wedges.push_back({cost[i][j], {i, j}});
        }
    }
    sort(sorted_wedges.begin(), sorted_wedges.end());
    for (auto e : edges) {
        if (e.first > e.second) {
            swap(e.first, e.second);
        }
        min_st = min(min_st, kruskal(n, sorted_wedges, e.first, e.second));
    }
    cout << min_st << std::endl;
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        solve();
    }
}