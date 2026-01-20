#include <iostream>
#include <limits.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>

//DATA
//vertaices
class City {
private:
    std::string name_;
    double latitude_;
    double longitude_;

public:
    City(const std::string name, double latitude, double longitude) : name_(name), latitude_(latitude), longitude_(longitude) {}
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
    int add_city(const std::string name, double latitude, double longitude) {
        auto it = nameToId_.find(name);
        if (it == nameToId_.end()) {
            City city(name, latitude, longitude);
            cities_.push_back(city);
            nameToId_[name] = cities_.size() - 1;

            return cities_.size() - 1;
        }

        return it->second;
    }
};

//Services/Algorithms
class GeoDistanceCalculator {

public:
    int distance_km(const City& a, const City& b) const {
        const double pi = 3.141592653589793;
        const int R = 6378;
        auto toRad = [&](const double deg) { return deg * pi / 180.0; };

        double lat1 = toRad(a.get_latitude());
        double lon1 = toRad(a.get_longitude());
        double lat2 = toRad(b.get_latitude());
        double lon2 = toRad(b.get_longitude());

        double cosTheta =
            std::sin(lat1) * std::sin(lat2) +
            std::cos(lat1) * std::cos(lat2) * std::cos(lon1 - lon2);


        if (cosTheta >  1.0) cosTheta =  1.0;
        if (cosTheta < -1.0) cosTheta = -1.0;

        double theta = std::acos(cosTheta);
        double dist  = R * theta;

        return static_cast<int>(std::llround(dist));

    }

};


class WeightedGraph {
private:
    std::vector<std::vector<int>> dist_;
    const int INF = 1000000000;

public:
    WeightedGraph() {}

    void init(int n) {
        dist_.assign(n, std::vector<int>(n, INF));
        for (int i = 0; i < n; i++) {
            dist_[i][i] = 0;
        }
    }

    void add_edge(int u, int v, int w) {
        dist_[u][v] = std::min(dist_[u][v], w);
       // dist_[v][u] = std::min(dist_[v][u], w);

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
    int distance(int u, int v) const { return dist_[u][v]; }
    int inf() const { return INF; }
};

//userCase object -> + factory reseni bootstrap problemu inicializace obejktu
struct CaseData {
    int n = 0, m =0, q = 0;
    std::vector<std::pair<int,int>> queries;
    CityRepository repo;
    WeightedGraph graph;


};

//orchastrate/facade
class AirlineNetworkApp {
private:
    static bool read_case(std::istream& in, CaseData*& data) {
        int n, m, q;
        in >> n >> m  >> q;
        if (n == 0 && m == 0 && q == 0) return false;

        CaseData* c = new CaseData();

        //Cities
        for (int i = 0; i < n; i++) {
            std::string s;
            double latitude, longitude;

            in >> s >> latitude >> longitude;
            c->repo.add_city(s, latitude, longitude);
        }

        c->graph.init(n);
        GeoDistanceCalculator calc;
        //Direct flights
        for (int i = 0; i < m; i++) {
            std::string s1, s2;
            in >> s1 >> s2;
            int u = c->repo.id_of(s1), v = c->repo.id_of(s2);
            int w = calc.distance_km(c->repo.city(u), c->repo.city(v));
            c->graph.add_edge(u, v, w);
        }

        //Queries
        for (int i = 0; i < q; i++) {
            std::string s1, s2;
            in >> s1 >> s2;
            int  departure_idx = c->repo.id_of(s1), arrivle_idx = c->repo.id_of(s2);
            c->queries.push_back(std::make_pair(departure_idx, arrivle_idx));
        }

        data = c;
        return true;
    }
    void print_result(std::ostream& out, CaseData& data, int caseNo) {
        out << "Case #" << caseNo << "\n";

        for (int i = 0; i < data.queries.size(); i++) {
            int u = data.queries[i].first;
            int v = data.queries[i].second;

            int d = data.graph.distance(u, v);
            if (d >= data.graph.inf()) { out << "no route exists\n";}
            else out << d << " km\n";
        }
    };


public:
    void run(std::istream& in, std::ostream& out) {
        bool first = true;
        int caseNo = 1;

        CaseData* data;
        while (read_case(in, data)) {
            data->graph.compute_all_pairs();

            if (!first) { out << "\n"; }
            first = false;
            print_result(out, *data, caseNo);
            caseNo++;
            delete data;
        }
    }

};


int main() {
    AirlineNetworkApp app;
    app.run(std::cin, std::cout);

    return 0;
}