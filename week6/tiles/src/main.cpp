#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>
    graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

void solve() {
    int w, h;
    std::cin >> w >> h;
    std::vector<std::vector<int>> v(w, std::vector<int>(h));
    int count_x = 0;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            char c;
            std::cin >> c;
            v[i][j] = (c == '.');
            count_x += (c != '.');
        }
    }
    graph G(w * h);
    // Bulding Graph
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            if (v[i][j]) {
                if (i > 0 && v[i - 1][j]) {
                    boost::add_edge(i * h + j, (i - 1) * h + j, G);
                }
                if (j > 0 && v[i][j - 1]) {
                    boost::add_edge(i * h + j, i * h + (j - 1), G);
                }
            }
        }
    }

    // Maximum Matching
    std::vector<vertex_desc> mate_map(w * h);
    const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
    bool success = boost::checked_edmonds_maximum_cardinality_matching(
        G, boost::make_iterator_property_map(
               mate_map.begin(), boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(
        G, boost::make_iterator_property_map(
               mate_map.begin(), boost::get(boost::vertex_index, G)));

    if (2 * matching_size + count_x == w * h) {
        std::cout << "yes\n";
    } else {
        std::cout << "no\n";
    }
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}
