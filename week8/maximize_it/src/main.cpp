#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

#include <iostream>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
    int p, a, b;
    while (std::cin >> p && p != 0) {
        std::cin >> a >> b;
        if (p == 1) {
            const int X = 0;
            const int Y = 1;
            Program lp(CGAL::SMALLER, true, 0, false, 0);
            lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
            lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a * b);
            lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);
            lp.set_c(X, a); lp.set_c(Y, -b);

            Solution s = CGAL::solve_linear_program(lp, ET());
            if (s.is_infeasible()) {
                std::cout << "no" << std::endl;
            } else if(s.is_unbounded()) {
                std::cout << "unbounded" << std::endl;
            } else {
                auto res = -s.objective_value();
                std::cout << std::floor(CGAL::to_double(res)) << std::endl;
            }
        } else if (p == 2) {
            const int X = 0;
            const int Y = 1;
            const int Z = 2;
            Program lp(CGAL::LARGER, false, 0, true, 0);

            lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);
            lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, -4);
            lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_a(Z, 1, 1); lp.set_b(1, -a * b);
            lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, -1);
            
            Solution s = CGAL::solve_linear_program(lp, ET());
            if (s.is_infeasible()) {
                std::cout << "no" << std::endl;
            } else if(s.is_unbounded()) {
                std::cout << "unbounded" << std::endl;
            } else {
                auto res = s.objective_value();
                std::cout << std::ceil(CGAL::to_double(res)) << std::endl;
            }
        }
    }
}