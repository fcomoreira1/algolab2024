#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <limits>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;

double floor_to_double(const K::FT &x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

void solve(int n) {
    long px, py, qx, qy;
    std::cin >> px >> py >> qx >> qy;
    const P p(px, py), q(qx, qy);
    const R ray(p, q);
    bool flag = false;
    S best_seg;
    std::vector<S> segments(n);
    for(int i = 0; i < n; i++) {
        long ux, uy, vx, vy;
        std::cin >> ux >> uy >> vx >> vy;
        segments[i] = S(P(ux, uy), P(vx, vy));
    }
    std::random_shuffle(segments.begin(), segments.end());
    for (int i = 0; i < n; i++) {
       if (!flag && CGAL::do_intersect(ray, segments[i])) {
            flag = true;
            auto o = CGAL::intersection(ray, segments[i]);
            if (const P *op = boost::get<P>(&*o)) {
                best_seg = S(p, *op);
            } else if (const S *os = boost::get<S>(&*o)) {
                if (CGAL::compare_distance_to_point(p, os->source(), os->target()) == CGAL::SMALLER) {
                    best_seg = S(p, os->source());
                } else {
                    best_seg = S(p, os->target());
                }
            }
       } else if (CGAL::do_intersect(best_seg, segments[i])) {
            auto o = CGAL::intersection(best_seg, segments[i]);
            if (const P *op = boost::get<P>(&*o)) {
                best_seg = S(p, *op);
            } else if (const S *os = boost::get<S>(&*o)) {
                if (CGAL::compare_distance_to_point(p, os->source(), os->target()) == CGAL::SMALLER) {
                    best_seg = S(p, os->source());
                } else {
                    best_seg = S(p, os->target());
                }
            }
       }
    }
    if (flag) {
        std::cout << (long)floor_to_double(best_seg.target().x()) << " ";
        std::cout << (long)floor_to_double(best_seg.target().y()) << std::endl;
    } else {
        std::cout << "no" << std::endl;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int n;
    while (std::cin >> n) {
        if (n == 0)
            break;
        solve(n);
    }
}



