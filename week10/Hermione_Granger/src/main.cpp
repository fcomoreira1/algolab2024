#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <iostream>

typedef int IT;
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef K::Point_2 P;

void solve() {
    int f, m, c, g;
    std::cin >> f >> m >> c >> g;
    size_t d, s;
    Delaunay t;
    std::cin >> d >> s;
    typedef std::pair<P, Index> IPoint;
    std::vector<IPoint> pts(s);
    std::vector<std::tuple<int, int, int>> info(s);
    for (size_t i = 0; i < s; i++) {
        int x, y, info_m, info_c, info_g;
        std::cin >> x >> y >> info_m >> info_c >> info_g;
        pts[i] = {P(x, y), i};
        info[i] = {info_m, info_c, info_g};
    }
    t.insert(pts.begin(), pts.end());
    // best_to_s[i] == d means no point in D has S[i] as the closest point.
    std::vector<Index> best_to_s(s, d);
    std::vector<int> cost_potion(d);
    for (size_t i = 0; i < d; i++) {
        int x, y;
        std::cin >> x >> y >> cost_potion[i];
        Index aux = t.nearest_vertex(P(x, y))->info();
        if (best_to_s[aux] == d || cost_potion[best_to_s[aux]] > cost_potion[i]){
            best_to_s[aux] = i;
        }
    }

    // Equation 0 is for m, 1 for c, 2 for g
    // 3 for cost
    Program lp(CGAL::LARGER, true, 0, true, 24);
    lp.set_b(0, m);
    lp.set_b(1, c);
    lp.set_b(2, g);
    lp.set_b(3, -f);
     
    // Optimization using that not all DA's are used.
    size_t cur_var = 0;
    for (size_t i = 0; i < s; i++) {
        Index cur = best_to_s[i];
        if (cur != d) {
            lp.set_a(cur_var, 0, std::get<0>(info[i]));
            lp.set_a(cur_var, 1, std::get<1>(info[i]));
            lp.set_a(cur_var, 2, std::get<2>(info[i]));
            lp.set_a(cur_var, 3, -cost_potion[cur]);
            cur_var++;
        }
    }
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    std::cout << (sol.is_infeasible() ? "H" : "L") << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}

