#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

typedef std::pair<long, long> pll;

int main() {
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    while (t--) {
        int n;
        std::cin >> n;
        std::vector<pll> boats(n);
        for (int i = 0; i < n; i++) {
            std::cin >> boats[i].first >> boats[i].second;
        }
        std::sort(boats.begin(), boats.end(),
                  [](pll a, pll b) { return a.second < b.second; });
        long last_boat = boats[0].second;
        int res = 1;
        for (int i = 1; i < n; i++) {
            if (last_boat <= boats[i].second) {
                res++;
                long last_tmp =
                    std::max(boats[i].second, boats[i].first + last_boat);
                int j = i + 1;
                while (j < n && boats[i].second < last_tmp) {
                    long aux = std::max(boats[j].second, boats[j].first + last_boat);
                    if (aux < last_tmp){
                        i = j;
                        last_tmp = aux;
                    }
                    j++;
                }
                last_boat = last_tmp;
            }
        }
        std::cout << res << std::endl;
    }
}
