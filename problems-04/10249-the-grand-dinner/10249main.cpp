#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

struct TeamInfo {
    int size;
    int originalIndex;
};

struct TableInfo {
    int remainingCapacity;
    int id1Based;
};

//comperators
static bool by_teamSize_esc(const TeamInfo& a, const TeamInfo& b) {
    return a.size > b.size;
}

static bool by_tableCapacity_desc(const TableInfo& a, const TableInfo& b) {
    return a.remainingCapacity > b.remainingCapacity;
}

std::vector<TeamInfo> build_teams(const std::vector<int>& teamSizes) {
    std::vector<TeamInfo> teams;
    teams.reserve(teamSizes.size());
    for (std::size_t i = 0; i < teamSizes.size(); ++i) {
        TeamInfo t; t.size = teamSizes[i]; t.originalIndex = static_cast<int>(i);
        teams.push_back(t);
    }
    return teams;
}

std::vector<TableInfo> build_tables(const std::vector<int>& tableCapacities) {
    std::vector<TableInfo> tables;
    tables.reserve(tableCapacities.size());
    for (std::size_t j = 0; j < tableCapacities.size(); ++j) {
        TableInfo tb; tb.remainingCapacity = tableCapacities[j]; tb.id1Based = static_cast<int>(j) + 1;
        tables.push_back(tb);
    }
    return tables;
}

// po sortu zceknout
bool has_enough_distinct_tables(const std::vector<TableInfo>& tables, int need) {
    if (need == 0) return true;
    if (need > static_cast<int>(tables.size())) return false;
    return tables[need - 1].remainingCapacity > 0;
}

bool seat_all_teams_Greedy(const std::vector<int>& teamSizes,
                        const std::vector<int>& tableCapacities,
                        std::vector< std::vector<int> >& assignment)
{

    std::vector<TeamInfo>  teams  = build_teams(teamSizes);
    std::vector<TableInfo> tables = build_tables(tableCapacities);

    //serazeni stolu podle velikost
    std::sort(teams.begin(), teams.end(), by_teamSize_esc);


    assignment.assign(teamSizes.size(), std::vector<int>());


    for (std::size_t t = 0; t < teams.size(); ++t) {
        const int need = teams[t].size; //pocet lidi v aktualn tymu -> tolik mist potrebuju priradit && kolik stolu potrebuju -> kazdy z nich potrebuje jiny stul
        const int originalIndex = teams[t].originalIndex;


        if (need == 0) {
            continue;
        }

        // prerovnani kapacity stolu
        std::sort(tables.begin(), tables.end(), by_tableCapacity_desc);


        if (!has_enough_distinct_tables(tables, need)) {
            return false; // reseni neni
        }

        // prirazeni kapacit
        for (int k = 0; k < need; ++k) {
            assignment[originalIndex].push_back(tables[k].id1Based);
            tables[k].remainingCapacity -= 1;
        }
    }

    // proslo vse
    return true;
}


void printResult(bool ok, const std::vector< std::vector<int> >& assignment) {
    if (!ok) {
        std::cout << 0 << "\n";
        return;
    }

    std::cout << 1 << "\n";
    for (std::size_t i = 0; i < assignment.size(); ++i) {
        for (std::size_t k = 0; k < assignment[i].size(); ++k) {
            if (k) std::cout << ' ';
            std::cout << assignment[i][k];
        }
        std::cout << "\n";
    }
}

int main() {
    int teamCount, tableCount;

    while ( (std::cin >> teamCount >> tableCount) && (teamCount || tableCount) ) {


        std::vector<int> teamSizes(teamCount);
        std::vector<int> tableCapacities(tableCount);

        for (int i = 0; i < teamCount; ++i) std::cin >> teamSizes[i];
        for (int j = 0; j < tableCount; ++j) std::cin >> tableCapacities[j];


        // Greedy
        std::vector< std::vector<int> > assignment;
        bool ok = seat_all_teams_Greedy(teamSizes, tableCapacities, assignment);

        printResult(ok, assignment);
    }

    return 0;
}