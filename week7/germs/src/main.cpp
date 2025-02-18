#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef K::Point_2 P;

long get_answer(K::FT distance_squared) {
    // least integer t such that t*t + 1/2 >= sqrt(distance_squared)
    K::FT distance = CGAL::sqrt(distance_squared);
    double t = std::ceil(CGAL::to_double(CGAL::sqrt(distance - 0.5)));
    return t;
}

void solve(int n) {;
    int lx, ly, ux, uy;
    std::cin >> lx >> ly >> ux >> uy;
    auto dist_to_box = [&](const P& p) {
        return std::min({p.x() - lx, ux - p.x(), p.y() - ly , uy - p.y()});
    };
    typedef std::pair<P, Index> IPoint;
    std::vector<IPoint> points(n);
    std::vector<K::FT> min_dist_sq(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        points[i] = {P(x, y), i};
        auto d = dist_to_box(points[i].first);
        min_dist_sq[i] = d * d;
    }
    Delaunay t;
    t.insert(points.begin(), points.end());
    // Use that the EMST is a subgraph of the Delaunay triangulation
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        Index v1 = e->first->vertex((e->second + 1) % 3)->info();
        Index v2 = e->first->vertex((e->second + 2) % 3)->info();
        K::FT dist = CGAL::squared_distance(points[v1].first, points[v2].first);
        dist /= 4;
        min_dist_sq[v1] = std::min(min_dist_sq[v1], dist);
        min_dist_sq[v2] = std::min(min_dist_sq[v2], dist);
    }
    std::sort(min_dist_sq.begin(), min_dist_sq.end());
    int median_index = n / 2;
    std::cout
        << get_answer(min_dist_sq[0]) << " "
        << get_answer(min_dist_sq[median_index]) << " " 
        << get_answer(min_dist_sq[n - 1]) << std::endl;
}
int main() {
    int n;
    while (std::cin >> n && n > 0) {
        solve(n);
    }
}