#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef K::Point_2 P;

double floor_to_double(const K::FT &x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

int main() {
    // read number of points
    size_t n;
    while (std::cin >> n && n) {
        // read points
        std::vector<P> pts;
        pts.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            int x, y;
            std::cin >> x >> y;
            pts.push_back(P(x, y));
        }
        // construct triangulation
        Triangulation t;
        t.insert(pts.begin(), pts.end());
        size_t m;
        std::cin >> m;
        for (size_t i = 0; i < m; ++i) {
            int x, y;
            std::cin >> x >> y;
            P p(x, y);
            P nearest = t.nearest_vertex(p)->point();
            std::cout << std::setprecision(0) << std::fixed
                      << CGAL::squared_distance(p, nearest) << "\n";
        }
    }
}
