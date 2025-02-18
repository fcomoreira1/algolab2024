#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Delaunay;
typedef Delaunay::Edge_iterator Edge_iterator;
typedef Delaunay::Face_handle face_handle;
typedef K::Point_2 P;
typedef K::Triangle_2 T;

double floor_to_double(const K::FT &x) {
    double a = std::floor(CGAL::to_double(x));
    while (a > x)
        a -= 1;
    while (a + 1 <= x)
        a += 1;
    return a;
}

void solve() {
    int n, h;
    K::FT s;
    std::cin >> n >> s >> h;
    std::vector<P> points(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        points[i] = P(x, y);
    }
    Delaunay t;
    t.insert(points.begin(), points.end());
    long a2 = 0, a3 = 0;
    K::FT min_dist = std::numeric_limits<double>::max();
    for (auto e : t.finite_edges()) {
        P p1 = e.first->vertex((e.second + 1) % 3)->point();
        P p2 = e.first->vertex((e.second + 2) % 3)->point();
        K::FT dist = CGAL::to_double(CGAL::squared_distance(p1, p2));
        if (dist < min_dist) {
            a2 = 1;
            min_dist = dist;
        } else if (dist == min_dist) {
            a2++;
        }
    }
    min_dist = std::numeric_limits<double>::max();
    std::unordered_map<face_handle, P> vonoroi;
    for (auto f : t.finite_face_handles()) {
        P p = t.dual(f);
        vonoroi[f] = p;
        P v = f->vertex(0)->point();
        K::FT dist = CGAL::squared_distance(p, v);
        if (dist < min_dist) {
            a3 = 1;
            min_dist = dist;
        } else if (dist == min_dist) {
            a3++;
        }
    }
    long as = 0;
    std::vector<std::pair<K::FT, bool>> intervals;
    for (auto f : t.finite_face_handles()) {
        K::FT radius =
            CGAL::squared_distance(vonoroi[f], f->vertex(0)->point());
        for (int i = 0; i < 3; i++) {
            face_handle f_neigh = f->neighbor(i);
            P p1 = f->vertex((i + 1) % 3)->point();
            P p2 = f->vertex((i + 2) % 3)->point();
            auto side = K::Segment_2(p1, p2);
            K::FT len_side = CGAL::squared_distance(p1, p2) / 4.0;
            if (t.is_infinite(f_neigh)) {
                auto ray = K::Segment_2(f->vertex(i)->point(), vonoroi[f]);
                if (CGAL::do_intersect(ray, side)) {
                    intervals.push_back({radius, 0});
                    intervals.push_back({radius, 0});
                    if (s >= radius)
                        as += 2;
                } else {
                    intervals.push_back({len_side, 0});
                    intervals.push_back({len_side, 0});
                    if (s >= len_side)
                        as += 2;
                }
            } else {
                K::FT radius_neigh =
                    CGAL::squared_distance(vonoroi[f_neigh], p1);
                K::FT max_center_dists = CGAL::max(radius, radius_neigh);
                K::FT min_center_dists = CGAL::min(radius, radius_neigh);
                auto bisec = K::Segment_2(vonoroi[f_neigh], vonoroi[f]);
                if (CGAL::do_intersect(side, bisec)) {
                    intervals.push_back({len_side, 0});
                    intervals.push_back({max_center_dists, 1});
                    if (s >= len_side && s <= max_center_dists)
                        as += 1;
                } else {
                    intervals.push_back({min_center_dists, 0});
                    intervals.push_back({max_center_dists, 1});
                    if (s >= min_center_dists && s <= max_center_dists)
                        as += 1;
                }
            }
        }
    }
    std::sort(intervals.begin(), intervals.end());
    long amax = 0, cur_counter = 0;
    for (int i = 0; i < intervals.size(); i++) {
        K::FT val = intervals[i].first;
        bool is_end = intervals[i].second;
        if (is_end) {
            cur_counter--;
        } else{
            cur_counter++;
            amax = std::max(amax, cur_counter);
        }
    }
    std::cout << a2 << " " << a3 << " " << as / 2 << " " << amax / 2 << std::endl;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}