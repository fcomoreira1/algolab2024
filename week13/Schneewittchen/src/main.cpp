#include <CGAL/Gmpz.h>
#include <CGAL/QP_functions.h>
#include <CGAL/QP_models.h>
#include <iostream>
#include <vector>
// choose input type (input coefficients must fit)
//
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using std::vector;
typedef vector<int> vi;
typedef vector<vector<int>> vvi;
using std::pair;

void dfs(int node, const vvi &adj, vvi &minerals, vvi &reduced_adj,
         int prev_dang, const vi &dangerous_index) {
    if (dangerous_index[node] > 0) {
        reduced_adj[prev_dang].push_back(node);
        prev_dang = node;
    }
    for (auto &v : adj[node]) {
        dfs(v, adj, minerals, reduced_adj, prev_dang, dangerous_index);
        if (dangerous_index[v] == -1) {
            for (int j = 0; j < minerals[v].size(); j++) {
                minerals[node][j] += minerals[v][j];
            }
        }
    }
}

void solve() {
    int n, m;
    std::cin >> n >> m;
    vi d(n, -1), c(n), s(n), p(n);
    vvi adj(n);
    vvi minerals(n, vi(m));
    vi dangerous_index(n, -1);
    dangerous_index[0] = 0;
    int num_dang = 1;
    for (int i = 0; i < n; i++) {
        std::cin >> d[i];
        if (d[i] >= 0) {
            dangerous_index[i] = num_dang++;
        }
        for (int j = 0; j < m; j++) {
            std::cin >> minerals[i][j];
        }
    }
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        std::cin >> u >> v;
        adj[v].push_back(u);
    }
    for (int i = 0; i < m; i++) {
        std::cin >> c[i] >> s[i] >> p[i];
    }
    vvi reduced_adj(n);
    dfs(0, adj, minerals, reduced_adj, 0, dangerous_index);
    
    Program lp(CGAL::SMALLER, true, 0, false, 0);
    int cur_eq = 0;
    for (int j = 0; j < m; j++) {
        lp.set_a(j, j, 1);
        lp.set_b(j, minerals[0][j]);
        for (auto &v : reduced_adj[0]) {
            int v_ind = dangerous_index[v];
            lp.set_a(v_ind * m + j, j, -1);
        }
    }
    cur_eq = m;
    for (int i = 1; i < n; i++) {
        int i_ind = dangerous_index[i];
        if (i_ind == -1)
            continue;
        for (int j = 0; j < m; j++) {
            lp.set_a(i_ind * m + j, cur_eq + j, 2);
            lp.set_b(cur_eq + j, minerals[i][j]);
            lp.set_a(i_ind * m + j, cur_eq + m, 2);
        }
        lp.set_b(cur_eq + m, d[i]);
        for (auto &v : reduced_adj[i]) {
            int v_ind = dangerous_index[v];
            for (int j = 0; j < m; j++) {
                lp.set_a(v_ind * m + j, cur_eq + j, -1);
            }
        }
        cur_eq += (m + 1);
    }
    for (int j = 0; j < m; j++) {
        lp.set_a(num_dang * m + j, cur_eq, 1);
        lp.set_a(j, cur_eq, 1);
        lp.set_b(cur_eq, c[j]);
        cur_eq++;

        lp.set_a(num_dang * m + j, cur_eq, -1);
        lp.set_a(j, cur_eq, -1);
        lp.set_b(cur_eq, -c[j]);
        cur_eq++;
        
        lp.set_a(num_dang * m + j, cur_eq, 1);
        lp.set_b(cur_eq, s[j]);
        cur_eq++;

        lp.set_c(num_dang * m + j, p[j]);
    }

    Solution sol = CGAL::solve_linear_program(lp, ET());
    if (sol.is_infeasible() || !sol.solves_linear_program(lp) ||
        !sol.is_optimal())
        std::cout << "Impossible!\n";
    else {
        CGAL::Quotient<ET> res = sol.objective_value();
        int result = (int)std::floor(CGAL::to_double(res));
        std::cout << result << "\n";
    }
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}