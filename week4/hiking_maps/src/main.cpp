#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/enum.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

bool is_inside(const P &p, const std::vector<P> &t) {
    for (int i = 0; i < 3; i++) {
        auto x = CGAL::orientation(t[2 * i], t[2 * i + 1], p);
        auto y = CGAL::orientation(t[2 * i], t[2 * i + 1], t[(2 * i + 2) % 6]);
        if ((x != CGAL::COLLINEAR) && (x != y))
            return false;
    }
    return true;
}

void solve() {
    int m, n;
    std::cin >> m >> n;
    std::vector<P> paths(m);
    for (int i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        paths[i] = P(x, y);
    }
    std::vector<std::vector<int>> edge_in_triangle(m - 1,
                                                    std::vector<int>(n));
    std::vector<P> triangle(6);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 6; j++) {
            int x, y;
            std::cin >> x >> y;
            triangle[j] = P(x, y);
        }
        bool prev_point_inside = is_inside(paths[0], triangle);
        for (int j = 1; j < m; j++) {
            bool point_inside = is_inside(paths[j], triangle);
            edge_in_triangle[j - 1][i] = point_inside && prev_point_inside;
            prev_point_inside = point_inside;
        }
    }
    int l = 0, r = 0, best = n;
    std::vector<int> cur_state(m - 1, 0);
    while (r < n) {
        bool flag = true;
        for (int i = 0; i < m - 1; i++) {
            cur_state[i] += edge_in_triangle[i][r];
            flag = flag && (cur_state[i] > 0);
        }
        r++;
        while (flag && l < r) {
            best = std::min(best, r - l);
            for (int i = 0; i < m - 1; i++) {
                cur_state[i] -= edge_in_triangle[i][l];
                flag = flag && (cur_state[i] > 0);
            }
            l++;
        }
    }
    std::cout << best << "\n";
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
