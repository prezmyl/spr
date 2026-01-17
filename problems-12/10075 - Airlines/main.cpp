#include <iostream>
#include <unordered_map>
#include <vector>

//DATA
//vertaices
class City {
private:
    std::string name_;
    double latitude_;
    double longitude_;

public:
    City(std::string name_, double latitude_, double longitude_) : name_(name_), latitude_(latitude_), longitude_(longitude_) {}
    const std::string get_name() { return name_; }
    double get_latitude() { return latitude_; }
    double get_longitude() { return longitude_; }
};

//catalog mest
class CityRepository {
private:
    std::vector<City> cities_;
    std::unordered_map<std::string, int> nameToId_;
public:
    CityRepository() {}
    int id_of(const std::string& name_) const {}
    const City& city(int index) const { return cities_[index]; }
    int sizeCity() const { return cities_.size(); }
    void add_city(std::string name_, double latitude_, double longitude_) {
        City city(name_, latitude_, longitude_);
        cities_.push_back(city);

    }
};

//Algorithms/services
class WeightedGraph {
private:
    std::vector<std::vector<int>> adj_;
    int calculate_weight_() { //ortodromas

    }

public:
    WeightedGraph() {}
    void add_edge(int u, int v) {
        adj_[u].push_back(v);
        adj_[v].push_back(u);
    }
    int dijkstra(int source, int target) {}
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