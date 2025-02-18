#include <iostream>
#include <vector>
// example: how to solve a simple explicit LP
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Point {
    int x, y;
};

void solve() {
    int n, m;
    long s;
    std::cin >> n >> m >> s;
    Program lp(CGAL::SMALLER, false, 0, false, 0);
    const int a = 0, b = 1, c = 2, l = 3;
    lp.set_l(l, true, 0);
    std::vector<Point> nobles(n), com(m);
    int cur_eq = 0;
    for (int i = 0; i < n; i++) {
        std::cin >> nobles[i].x >> nobles[i].y;
        lp.set_a(a, cur_eq, nobles[i].y);
        lp.set_a(c, cur_eq, -1);
        lp.set_b(cur_eq, -nobles[i].x);
        cur_eq++;
    }
    for (int i = 0; i < m; i++) {
        std::cin >> com[i].x >> com[i].y;
        lp.set_a(a, cur_eq, -com[i].y);
        lp.set_a(c, cur_eq, 1);
        lp.set_b(cur_eq, com[i].x);
        cur_eq++;
    }
    Solution sol = CGAL::solve_linear_program(lp, ET());
    if (!sol.solves_linear_program(lp) || sol.is_infeasible()) {
        std::cout << "Y\n";
        return;
    }
    long coef_a = 0;
    int coef_c = n - m;
    long coef_sum = s;
    for (int i = 0; i < n; i++) {
        coef_a -= (long) nobles[i].y;
        coef_sum += (long) nobles[i].x;
        lp.set_a(a, cur_eq, nobles[i].x);
        lp.set_a(b, cur_eq, 1);
        lp.set_a(l, cur_eq, -1);
        lp.set_b(cur_eq, nobles[i].y);
        cur_eq++;
        lp.set_a(a, cur_eq, -nobles[i].x);
        lp.set_a(b, cur_eq, -1);
        lp.set_a(l, cur_eq, -1);
        lp.set_b(cur_eq, -nobles[i].y);
        cur_eq++;
    }
    for (int i = 0; i < m; i++) {
        coef_a += (long) com[i].y;
        coef_sum -= (long) com[i].x;
        lp.set_a(a, cur_eq, com[i].x);
        lp.set_a(b, cur_eq, 1);
        lp.set_a(l, cur_eq, -1);
        lp.set_b(cur_eq, com[i].y);
        cur_eq++;
        lp.set_a(a, cur_eq, -com[i].x);
        lp.set_a(b, cur_eq, -1);
        lp.set_a(l, cur_eq, -1);
        lp.set_b(cur_eq, -com[i].y);
        cur_eq++;
    }
    if (s != -1) {
        lp.set_a(a, cur_eq, coef_a);
        lp.set_a(c, cur_eq, coef_c);
        lp.set_b(cur_eq, coef_sum);
    }
    lp.set_c(l, 1);
    Solution new_sol = CGAL::solve_linear_program(lp, ET());
    if (new_sol.is_infeasible() || !new_sol.solves_linear_program(lp)) {
        std::cout << "B\n";
    } else {
        // std::cout << "Feasible\n";
        auto obj = new_sol.objective_value();
        std::cout << (obj.numerator() + obj.denominator() - 1) /
                         obj.denominator()
                  << std::endl;
    }
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}