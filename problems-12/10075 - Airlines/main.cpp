#include <iostream>
#include <limits.h>
#include <unordered_map>
#include <vector>
#include <algorithm>

//DATA
//vertaices
class City {
private:
    std::string name_;
    double latitude_;
    double longitude_;

public:
    City(std::string name_, double latitude_, double longitude_) : name_(name_), latitude_(latitude_), longitude_(longitude_) {}
    const std::string& get_name() const { return name_; }
    double get_latitude() const { return latitude_; }
    double get_longitude() const  { return longitude_; }
};

//catalog mest
class CityRepository {
private:
    std::vector<City> cities_; // pristup O(1) pres index
    std::unordered_map<std::string, int> nameToId_; // O(1) hledani jmena
public:
    CityRepository() {}
    int id_of(const std::string& name_) const {
        auto it = nameToId_.find(name_);
        if (it == nameToId_.end()) { return -1; }
        return it->second;
    }
    const City& city(int index) const { return cities_[index]; }
    int size() const { return cities_.size(); }
    int add_city(std::string name_, double latitude_, double longitude_) {
        auto it = nameToId_.find(name_);
        if (it == nameToId_.end()) {
            City city(name_, latitude_, longitude_);
            cities_.push_back(city);
            nameToId_[name_] = cities_.size() - 1;

            return cities_.size() - 1;
        }

        return it->second;
    }
};

//Services/Algorithms
class GeoDistanceCalculator {
public:
    int distance_km(const City& a, const City& b) const {
        double pi = 3.141592653589793;
        int earthRadius = 6378;
    }
};


class WeightedGraph {
private:
    std::vector<std::vector<int>> dist_;
    const int INF = 1000000000;

public:
    WeightedGraph(int n) {

        dist_.assign(n, std::vector<int>(n, INF));
        for (int i = 0; i < n; i++) {
            dist_[i][i] = 0;
        }
    }
    void add_edge(int u, int v, int w) {
        dist_[u][v] = std::min(dist_[u][v], w);
        dist_[v][u] = std::min(dist_[v][u], w);

    }
    void compute_all_pairs() {
        int n = dist_.size();
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dist_[i][k] == INF || dist_[k][j] == INF) {continue;}
                    dist_[i][j] = std::min( dist_[i][j] ,dist_[i][k] + dist_[k][j]);
                }
            }
        }
    }
};

//orchastrate/facade
class AirlineNetworkApp {
private:
    static int read_inpout(std::istream& in) {};
    int print_output(std::ostream& out) {};

public:
    void run(std::istream& in, std::ostream& out) {

    }

};


int main() {
    AirlineNetworkApp app;
    app.run(std::cin, std::cout);

    return 0;
}