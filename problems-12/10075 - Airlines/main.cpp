#include <iostream>
#include <map>
#include <vector>


class City {
private:
    std::string name;
    float latitude;
    float longitude;

public:
    City(std::string name, float latitude, float longitude) : name(name), latitude(latitude), longitude(longitude) {}
    std::string get_name() { return name; }
    float get_latitude() { return latitude; }
    float get_longitude() { return longitude; }
};

class Route {
private:
    std::vector<City> cities;
    std::map<std::pair<City,City>,int> ortodromas;
    bool directRoute = false;

    int ortodroma;
    int count_ortodroma() {
        const float pi = 3.141592653589793;
        int earthRadius = 6378;
    };

public:
    Route(std::vector<City> cities) : cities(cities) {
        if (this->cities.size() == 2) { this->directRoute = true; }

    }
    std::vector<City> get_cities() { return cities; }
    bool is_direct_route() { return directRoute; }
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}