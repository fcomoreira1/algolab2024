#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

#include <iostream>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, d;
    while (std::cin >> n && n) {
        std::cin >> d;
        Program lp(CGAL::SMALLER, false, 0, false, 0);
        const int R = n;
        lp.set_l(R, true, 0);
        lp.set_c(R, -1);
        int a, b;
        for (int i = 0; i < n; i++) {
            int acc_norm = 0;
            for (int j = 0; j < d; j++) {
                std::cin >> a;
                acc_norm += a * a;
                lp.set_a(j, i, a);
            }
            std::cin >> b;
            lp.set_b(i, b);
            int norm = std::floor(std::sqrt(acc_norm + 1));
            lp.set_a(R, i, norm);
        }

        Solution s = CGAL::solve_linear_program(lp, ET());
        // std::cout << s;
        if (s.is_infeasible()) {
            std::cout << "none\n";
        } else if (s.is_unbounded()) {
            std::cout << "inf\n";
        } else {
            std::cout << std::floor(CGAL::to_double(-s.objective_value()))
                      << std::endl;
        }
    }
}