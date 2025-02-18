#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;

struct dir {
    int i;
    long y;
    R ray;
};

void solve() {
    int n;
    std::cin >> n;
    std::vector<dir> v(n);
    for (int i = 0; i < n; i++) {
        long x, y;
        v[i].i = i;
        std::cin >> v[i].y >> x >> y;
        v[i].ray = R(P((long) 0, v[i].y), P(x, y));
    }
    std::sort(v.begin(), v.end(), [](dir &a, dir &b) { return a.y < b.y; });

    std::vector<dir> possible_ans;
    possible_ans.push_back(v[0]);
    for (int i = 1; i < n; i++) {
        bool flag = true;
        while (possible_ans.size()) {
            if (CGAL::do_intersect(v[i].ray, possible_ans.back().ray)) {
                auto o = CGAL::intersection(v[i].ray, possible_ans.back().ray);
                if (const P *op = boost::get<P>(&*o)) {
                    if (CGAL::has_smaller_distance_to_point(
                            *op, v[i].ray.source(),
                            possible_ans.back().ray.source())) {
                        possible_ans.pop_back();
                    } else {
                        flag = false;
                        break;
                    }
                } // Cannot have an else case
            } else {
                break;
            }
        }
        if (flag) {
            possible_ans.push_back(v[i]);
        }
    }
    std::sort(possible_ans.begin(), possible_ans.end(),
              [](dir &a, dir &b) { return a.i < b.i; });
    for (auto &v : possible_ans) {
        std::cout << v.i << " ";
    }
    std::cout << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
