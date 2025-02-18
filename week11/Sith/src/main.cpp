#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef K::Point_2 P;

using std::pair;
typedef std::pair<long, long> pll;
using std::vector;

long squared_distance(const pll &p1, const pll &p2) {
    long dx = p1.first - p2.first;
    long dy = p1.second - p2.second;
    return dx * dx + dy * dy;
}

bool check(const vector<pair<P, Index>> &points, int k, long r) {
    Delaunay t;
    vector<int> num_per_comp(points.size(), 1);
    boost::disjoint_sets_with_storage<> uf(points.size());
    int max_comp = 1;
    t.insert(points.begin() + k, points.end());
    for (auto &e : t.finite_edges()) {
        auto v1 = e.first->vertex((e.second + 1) % 3);
        auto v2 = e.first->vertex((e.second + 2) % 3);
        if (CGAL::squared_distance(v1->point(), v2->point()) <= r) {
            Index c1 = uf.find_set(v1->info());
            Index c2 = uf.find_set(v2->info());
            if (c1 != c2) {
                uf.link(c1, c2);
                int c3 = uf.find_set(v1->info());
                num_per_comp[c3] = num_per_comp[c1] + num_per_comp[c2];
                max_comp = std::max(num_per_comp[c3], max_comp);
            }
        }
    }
    return max_comp >= k;
}

void solve() {
    int n;
    long r;
    std::cin >> n >> r;
    r = r * r;
    vector<pair<P, Index>> points(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        points[i] = {P(x, y), i};
    }
    int beg = 0;
    int end = n;
    int k = -1;
    while (beg < end) {
        int m = (beg + end) / 2;
        if (check(points, m, r)) {
            k = m;
            beg = m + 1;
        } else {
            end = m;
        }
    }
    std::cout << k << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}