#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef K::Point_2 P;
typedef Delaunay::Edge Edge;
typedef std::vector<Edge> Edges;

using std::pair;
using std::vector;

long get_edge_sq_dist(const Edge &e) {
    P v1 = e.first->vertex((e.second + 1) % 3)->point();
    P v2 = e.first->vertex((e.second + 2) % 3)->point();
    return CGAL::squared_distance(v1, v2);
}

int solve_given_radius(size_t n, long s, const vector<pair<size_t, long>> &nuts,
                       const Edges &edges) {
    boost::disjoint_sets_with_storage<> uf(n);
    // Build union-find
    for (auto e : edges) {
        size_t v1 = e.first->vertex((e.second + 1) % 3)->info();
        size_t v2 = e.first->vertex((e.second + 2) % 3)->info();
        if (get_edge_sq_dist(e) <= s) {
            size_t c1 = uf.find_set(v1);
            size_t c2 = uf.find_set(v2);
            if (c1 != c2) {
                uf.link(c1, c2);
            }
        }
    }
    vector<int> nuts_per_comp(n, 0);
    int best = 0;
    for (auto v: nuts) {
        if (v.second > s)
            continue;
        size_t c = uf.find_set(v.first);
        nuts_per_comp[c]++;
        best = std::max(best, nuts_per_comp[c]);
    }
    return best;
}

void solve() {
    size_t n, m;
    int k;
    long s;
    std::cin >> n >> m >> s >> k;
    vector<pair<P, size_t>> points(n);
    // Nuts contain the closest point to it, and how far it is.
    vector<pair<size_t, long>> nuts(m);
    Delaunay t;
    vector<long> distances;
    Edges edges;

    for (size_t i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        points[i] = {P(x, y), i};
    }
    t.insert(points.begin(), points.end());
    for (size_t i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        P tmp = P(x, y);
        auto nearest = t.nearest_vertex(tmp);
        long dist = 4 * CGAL::squared_distance(nearest->point(), tmp);
        distances.push_back(dist);
        nuts[i] = std::make_pair(nearest->info(), dist);
    }
    for (Edge e : t.finite_edges()) {
        edges.push_back(e);
        distances.push_back(get_edge_sq_dist(e));
    }
    std::sort(distances.begin(), distances.end());
    int a = solve_given_radius(n, s, nuts, edges);
    long b = -1;
    size_t low = 0, high = distances.size();
    while (low < high) {
        size_t mid = (low + high) / 2;
        if (k <= solve_given_radius(n, distances[mid], nuts, edges)) {
            b = distances[mid];
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    std::cout << a << " " << b << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
    std::size_t t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
