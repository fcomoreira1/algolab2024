#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;
typedef K::Segment_2 S;
int main() {
    int n;
    while (std::cin >> n) {
        if (n == 0) {
            break;
        }
        bool hit = false;
        long px, py, dirx, diry;
        std::cin >> px >> py >> dirx >> diry;
        R ray(P(px, py), P(dirx, diry));
        while (n--) {
            long ux, uy, vx, vy;
            std::cin >> ux >> uy >> vx >> vy;
            S seg(P(ux, uy), P(vx, vy));
            if (CGAL::do_intersect(ray, seg)) {
                hit = true;
            }
        }
        std::cout << (hit ? "yes" : "no") << std::endl;
    }
}
