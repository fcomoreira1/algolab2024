/// 1
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_root_of.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_root_of K;
typedef K::Point_2 P;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

double convert_to_ceil(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) 
        a += 1;
    while (a-1 >= x) 
        a -= 1;
    return a;
}

void solve(long n) {
    long x, y;
    std::vector<P> points;
    for (int i = 0; i < n; i++) {
        std::cin >> x >> y;
        points.push_back(P(x, y));
    }
    Min_circle mc(points.begin(), points.end(), true);
    Traits::Circle c = mc.circle();
    std::cout << (long)convert_to_ceil(CGAL::sqrt(c.squared_radius())) << std::endl;
}

int main() {
    long n;
    while (std::cin >> n && n > 0) {
        solve(n);
    }
}


