#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

#include <iostream>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
    int n, m;
    while (std::cin >> n >> m && n && m) {
        Program lp(CGAL::LARGER, true, 0, false, 0);
        for (int i = 0; i < n; i++) {
            int a, b;
            std::cin >> a >> b;
            lp.set_b(2 * i, a);
            lp.set_b(2 * i + 1, -b);
        }
        for (int i = 0; i < m; i++) {
            // Minimize price
            int p;
            std::cin >> p;
            lp.set_c(i, p);
            for (int j = 0; j < n; j++) {
                int c;
                std::cin >> c;
                lp.set_a(i, 2 * j, c);
                lp.set_a(i, 2 * j + 1, -c);
            }
        }
        Solution s = CGAL::solve_linear_program(lp, ET());
        if (s.is_infeasible()) {
            std::cout << "No such diet.\n";
        } else {
            std::cout << std::floor(CGAL::to_double(s.objective_value()))
                      << std::endl;
        }
    }
}