#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <iostream>
#include <set>
#include <vector>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

bool maximum_matching(const graph &G) {
    int n = boost::num_vertices(G);
    std::vector<vertex_desc> mate_map(n); // exterior property map

    boost::edmonds_maximum_cardinality_matching(
        G, boost::make_iterator_property_map(
               mate_map.begin(), boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(
        G, boost::make_iterator_property_map(
               mate_map.begin(), boost::get(boost::vertex_index, G)));
    return (2 * matching_size) == n;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        int n, c, f;
        std::cin >> n >> c >> f;
        int cnt = 0;
        std::unordered_map<std::string, int> str_to_int;
        std::vector<std::set<int>> chars(n); // Characteristics
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < c; j++) {
                std::string aux;
                std::cin >> aux;
                if (str_to_int.find(aux) == str_to_int.end()) {
                    str_to_int[aux] = cnt++;
                }
                chars[i].insert(str_to_int[aux]);
            }
        }
        graph G(n);
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                std::set<int> counter;
                std::set_intersection(chars[i].begin(), chars[i].end(),
                                      chars[j].begin(), chars[j].end(),
                                      std::inserter(counter, counter.begin()));
                if ((int)counter.size() > f) {
                    boost::add_edge(i, j, G);
                    boost::add_edge(j, i, G);
                }
            }
        }
        std::cout << (maximum_matching(G) ? "not " : "") << "optimal"
                  << std::endl;
    }
}
