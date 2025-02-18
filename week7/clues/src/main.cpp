#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/pending/disjoint_sets.hpp>
#include <iostream>
#include <tuple>
#include <vector>

using std::vector;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
    graph;
typedef boost::graph_traits<graph>::edge_iterator edge_it;
typedef std::vector<boost::default_color_type> partition_t;
typedef boost::vec_adj_list_vertex_id_map<boost::no_property, std::size_t>
    index_map_t;
typedef boost::iterator_property_map<partition_t::iterator, index_map_t>
    partition_map_t;
typedef boost::graph_traits<graph> traits;
typename traits::vertex_iterator vertex_iter, vertex_end;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef K::Point_2 P;
typedef std::size_t Index;
typedef std::pair<P, Index> IPoint;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;

void solve() {
    long n, m, r;
    std::cin >> n >> m >> r;
    r = r * r;
    vector<IPoint> stations(n);
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        stations[i] = std::make_pair(P(x, y), i);
    }
    Delaunay t;
    boost::disjoint_sets_with_storage<> uf(n);
    t.insert(stations.begin(), stations.end());
    graph G(n);

    for (auto &e : t.finite_edges()) {
        auto v1 = e.first->vertex((e.second + 1) % 3);
        auto v2 = e.first->vertex((e.second + 2) % 3);
        if (CGAL::squared_distance(v1->point(), v2->point()) <= r) {
            boost::add_edge(v1->info(), v2->info(), G);
            Index c1 = uf.find_set(v1->info());
            Index c2 = uf.find_set(v2->info());
            if (c1 != c2) {
                uf.link(c1, c2);
            }
        }
    }
    // Need to check if bipartite
    bool is_bipartite = boost::is_bipartite(G);
    if (is_bipartite) {
        partition_t partition(boost::num_vertices(G));
        partition_map_t partition_map(partition.begin(),
                                      boost::get(boost::vertex_index, G));

        boost::is_bipartite(G, boost::get(boost::vertex_index, G),
                            partition_map);
        vector<IPoint> white, black;
        for (boost::tie(vertex_iter, vertex_end) = boost::vertices(G);
             vertex_iter != vertex_end; ++vertex_iter) {
            if (boost::get(partition_map, *vertex_iter) ==
                boost::color_traits<boost::default_color_type>::white()) {
                white.push_back(stations[*vertex_iter]);
            } else {
                black.push_back(stations[*vertex_iter]);
            }
        }

        Delaunay t1, t2;
        t1.insert(white.begin(), white.end());
        t2.insert(black.begin(), black.end());
        for (auto &e : t1.finite_edges()) {
            auto v1 = e.first->vertex((e.second + 1) % 3);
            auto v2 = e.first->vertex((e.second + 2) % 3);
            if (CGAL::squared_distance(v1->point(), v2->point()) <= r) {
                is_bipartite = false;
                break;
            }
        }
        if (is_bipartite) {
            for (auto &e : t2.finite_edges()) {
                auto v1 = e.first->vertex((e.second + 1) % 3);
                auto v2 = e.first->vertex((e.second + 2) % 3);
                if (CGAL::squared_distance(v1->point(), v2->point()) <= r) {
                    is_bipartite = false;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < m; i++) {
        int ax, ay, bx, by;
        std::cin >> ax >> ay >> bx >> by;
        P a(ax, ay);
        P b(bx, by);
        if (is_bipartite) {
            if (CGAL::squared_distance(a, b) <= r) {
                std::cout << "y";
                continue;
            }
            auto n_a = t.nearest_vertex(a);
            auto n_b = t.nearest_vertex(b);

            if (CGAL::squared_distance(n_a->point(), a) <= r &&
                CGAL::squared_distance(n_b->point(), b) <= r &&
                uf.find_set(n_a->info()) == uf.find_set(n_b->info())) {
                std::cout << "y";
            } else {
                std::cout << "n";
            }
        } else{
            std::cout << "n";
        }
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