#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef K::Point_2 P;
typedef std::pair<P, Index> IPoint;

int find_greatest_num_groups(std::multiset<int> &arr, const int k) {
    int count = 0;
    int cur_set_sum = 0;
    while (true) {
        cur_set_sum = 0;
        while (cur_set_sum < k && arr.size() > 0) {
            auto it = arr.lower_bound(k - cur_set_sum);
            if (it == arr.end()) {
                --it;
            }
            cur_set_sum += *it;
            arr.erase(it);
        }
        count += (cur_set_sum >= k);
        if (arr.size() == 0)
            break;
    }
    return count;
}

int max_f(int n, const int k, const Delaunay &t, const long s) {
    boost::disjoint_sets_with_storage<> uf(n);
    for (auto &e : t.finite_edges()) {
        Index i1 = e.first->vertex((e.second + 1) % 3)->info();
        Index i2 = e.first->vertex((e.second + 2) % 3)->info();
        P p1 = e.first->vertex((e.second + 1) % 3)->point();
        P p2 = e.first->vertex((e.second + 2) % 3)->point();
        if (CGAL::squared_distance(p1, p2) < s) {
            Index c1 = uf.find_set(i1);
            Index c2 = uf.find_set(i2);
            if (c1 != c2) {
                uf.link(c1, c2);
            }
        }
    }
    std::vector<int> sizes(n);
    for (int i = 0; i < n; i++) {
        Index c = uf.find_set(i);
        sizes[c]++;
    }
    std::multiset<int> arr;
    for (int i = 0; i < n; i++) {
        if (sizes[i] > 0) {
            arr.insert(sizes[i]);
        }
    }
    return find_greatest_num_groups(arr, k);
}

void solve() {
    int n, k, f0;
    long s0;
    std::cin >> n >> k >> f0 >> s0;
    std::vector<IPoint> points(n);
    for (Index i = 0; i < n; ++i) {
        int x, y;
        std::cin >> x >> y;
        points[i] = {P(x, y), i};
    }
    Delaunay t;
    t.insert(points.begin(), points.end());
    std::vector<long> distances;
    for (auto &e: t.finite_edges()) {
        P p1 = e.first->vertex((e.second + 1) % 3)->point();
        P p2 = e.first->vertex((e.second + 2) % 3)->point();
        distances.push_back(CGAL::squared_distance(p1, p2));
    }
    std::sort(distances.begin(), distances.end());
    int l = 0, r = distances.size(), mid;
    long a, b = max_f(n, k, t, s0);
    while (l < r) {
        mid = (l + r) / 2;
        if (max_f(n, k, t, distances[mid]) >= f0) {
            a = distances[mid];
            l = mid + 1;
        } else {
            r = mid;
        }
    }
    std::cout << a << " " << b << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}