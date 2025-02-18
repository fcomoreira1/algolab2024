#include <iostream>
// BGL includes
#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>

typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using std::pair;
using std::vector;
typedef pair<long, long> pll;

long squared_distance(const pll &a, const pll &b) {
    long dx = a.first - b.first;
    long dy = a.second - b.second;
    return dx * dx + dy * dy;
}

long diff_vec(const vector<int> &a, const vector<int> &b) {
    int i = 0, j = 0;
    int count = 0;
    while (i < a.size() && j < b.size()) {
        if (a[i] < b[j]) {
            count++;
            i++;
        } else if (a[i] > b[j]) {
            count++;
            j++;
        } else {
            i++;
            j++;
        }
    }
    return count + (a.size() - i) + (b.size() - j);
}

void solve() {
    int n, m, c;
    std::cin >> n >> m >> c;
    vector<pll> loc_w(n);
    vector<int> alcohol(n);
    vector<pll> loc_s(m);

    vector<vector<int>> est_revenue(n, vector<int>(m));

    Program lp(CGAL::SMALLER, true, 0, false, 0);

    // Vars m * i + j is how much from supply i to stad j
    int CUR_EQ = 0;
    for (int i = 0; i < n; i++) {
        int x, y, s;
        std::cin >> x >> y >> s >> alcohol[i];
        loc_w[i] = {x, y};
        for (int j = 0; j < m; j++) {
            lp.set_a(m * i + j, CUR_EQ, 1);
        }
        lp.set_b(CUR_EQ, s);
        CUR_EQ++;
    }

    for (int j = 0; j < m; j++) {
        int x, y, d, u;
        std::cin >> x >> y >> d >> u;
        loc_s[j] = {x, y};
        for (int i = 0; i < n; i++) {
            lp.set_a(m * i + j, CUR_EQ, 1);
            lp.set_a(m * i + j, CUR_EQ + 1, -1);
            lp.set_a(m * i + j, CUR_EQ + 2, alcohol[i]);
        }
        lp.set_b(CUR_EQ, d);
        lp.set_b(CUR_EQ + 1, -d);
        lp.set_b(CUR_EQ + 2, u * 100);
        CUR_EQ += 3;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cin >> est_revenue[i][j];
        }
    }

    vector<vector<int>> w_in_circ(n);
    vector<vector<int>> s_in_circ(m);
    for (int k = 0; k < c; k++) {
        long x, y, r;
        std::cin >> x >> y >> r;
        r = r * r;
        for (int i = 0; i < n; i++) {
            if (squared_distance({x, y}, loc_w[i]) <= r) {
                w_in_circ[i].push_back(k);
            }
        }
        for (int j = 0; j < m; j++) {
            if (squared_distance({x, y}, loc_s[j]) <= r) {
                s_in_circ[j].push_back(k);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            long r = 100 * est_revenue[i][j] 
                    - diff_vec(w_in_circ[i], s_in_circ[j]);
            lp.set_c(m * i + j, -r);
        }
    }

    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
        std::cout << "no" << std::endl;
    } else {
        auto res = s.objective_value();
        std::cout << (long)-std::ceil(CGAL::to_double(res) / 100.0)
                  << std::endl;
    }
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