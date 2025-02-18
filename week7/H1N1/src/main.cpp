#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef Delaunay::Edge_iterator Edge_iterator;
typedef Delaunay::Face_handle face_handle;
typedef Delaunay::Vertex_handle vertex_handle;
typedef K::Point_2 P;

using std::vector;

void solve(int n) {
    vector<P> points(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        points[i] = P(x, y);
    }
    Delaunay t;
    t.insert(points.begin(), points.end());
    std::map<face_handle, double> max_radius_exit;
    std::priority_queue<std::pair<double, face_handle>> q;
    for (face_handle f : t.finite_face_handles()) {
        double best = 0;
        for (int i = 0; i < 3; i++) {
            if (t.is_infinite(f->neighbor(i))) {
                P p0 = f->vertex((i + 1) % 3)->point();
                P p1 = f->vertex((i + 2) % 3)->point();
                best = std::max(best, CGAL::squared_distance(p0, p1));
            }
        }
        if (best) {
            q.push({best, f});
            max_radius_exit[f] = best;
        }
    }
    while (!q.empty()) {
        auto [cap, f] = q.top();
        q.pop();
        for (int i = 0; i < 3; i++) {
            face_handle f_neigh = f->neighbor(i);
            if (!t.is_infinite(f_neigh)) {
                P p0 = f->vertex((i + 1) % 3)->point();
                P p1 = f->vertex((i + 2) % 3)->point();
                double cap_neigh =
                    std::min(cap, CGAL::squared_distance(p0, p1));
                if (max_radius_exit.find(f_neigh) == max_radius_exit.end() ||
                    max_radius_exit[f_neigh] < cap_neigh) {
                    max_radius_exit[f_neigh] = cap_neigh;
                    q.push({cap_neigh, f_neigh});
                }
            }
        }
    }
    int m;
    std::cin >> m;
    for (int i = 0; i < m; i++) {
        int x, y;
        double d;
        std::cin >> x >> y >> d;
        P p(x, y);
        vertex_handle v = t.nearest_vertex(p);
        if (CGAL::squared_distance(p, v->point()) >= d) {
            face_handle f = t.locate(p);
            if (t.is_infinite(f) || max_radius_exit[f] >= 4 * d) {
                std::cout << "y";
            } else {
                std::cout << "n";
            }
        } else {
            std::cout << "n";
        }
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n;
    while (std::cin >> n && n > 0) {
        solve(n);
    }
}