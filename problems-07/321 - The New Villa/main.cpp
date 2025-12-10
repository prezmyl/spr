#include <chrono>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>

// topologie: Staticky Graf domu z mistnostma

class Room {
private:
    int room_id;
    //bool light; -> do abstratni vrstavy state, duplicitni tady
    std::vector<int> neighbors_of;
    std::vector<int> switch_target;

public:
    Room(int room_id,
        const std::vector<int>& neighbors_of,
        const std::vector<int>& switch_target )
    : room_id(room_id),
    neighbors_of(neighbors_of),
    switch_target(switch_target) {}

    int get_id() const { return room_id; }

    const std::vector<int>& get_neighbors() const { return neighbors_of; }
    const std::vector<int>& get_switchable() const { return switch_target; }

};


class House {
private:
    std::vector<Room> rooms;

public:
    House() = default;

    void add_room(const Room& room) { rooms.push_back(room); }

    const Room& get_room(int room_id) const  {
        return rooms.at(room_id);

    }
    const std::vector<int>& neighbors_of(int room_id) const {
        return rooms.at(room_id).get_neighbors();
    }
    const std::vector<int>& switchable_target_from(int room_id) const {
        return rooms.at(room_id).get_switchable();
    }

    int room_count() const { return static_cast<int>(rooms.size()); }


};

// abstract layer - state and BFS -> Graf stavu (zmena stavu jedna hrana, bud move nebo light toggle)

struct Action {
    enum Type {MOVE, SWITCH} type;
    int room;
    bool turned_on;
};


class State {
private:
    int current_room_id;
    std::vector<bool> lights_state;
public:
    State(int current_room_id,
       const std::vector<bool>& lights_state)
    : current_room_id(current_room_id),
    lights_state(lights_state) {}

    int get_current_room() const { return current_room_id; }
    void set_current_room(int room_id) { current_room_id = room_id; }

    bool is_light_on(int light_id) const { return lights_state.at(light_id); }
    void set_light_state(int light_id, bool state) { lights_state.at(light_id) = state; }

    const std::vector<bool>& get_light() const { return lights_state; }

    int to_mask() const {
        int mask = 0;
        for (std::size_t i = 0; i < lights_state.size(); i++) {
            if (lights_state.at(i)) {
                mask |= (1 << i);
            }
        }
        return mask;

    }

    //konfigurace stavoveho prostoru, hranove krokz, immutable
    State with_moved_to(int room_id) const {
        State next = *this;
        next.set_current_room(room_id);
        return next;
    }
    State with_toggled_lighit_in(int room_id) const {
        State next = *this;
        bool new_light_state = !is_light_on(room_id);
        next.set_light_state(room_id, new_light_state);
        return next;
    }

    // genereovani vsech vrcholu na ktere vede hrana stavoveho prostoru z aktualniho vrcholu + pravidla
    std::vector<std::pair<State, Action>> expand_neighbors( const House& h) const {
        std::vector<std::pair<State, Action>> result;
        int here = get_current_room();

        //Move hrana - dvere
        const std::vector<int>& neigh = h.neighbors_of(here);
        for (std::size_t i = 0; i < neigh.size(); ++i) {
            int nextRoom = neigh[i];

            if (!is_light_on(nextRoom)) {
                continue;
            }

            State moved = with_moved_to(nextRoom);

            Action act;
            act.type = Action::MOVE;
            act.room = nextRoom;
            act.turned_on = false;  // ignore pro move

            result.push_back(std::make_pair(moved, act));
        }

        // Light hrana
        const std::vector<int>& sw = h.switchable_target_from(here);
        for (std::size_t i = 0; i < sw.size(); ++i) {
            int lightRoom = sw[i];
            bool was_on = is_light_on(lightRoom);

            if (lightRoom == here && was_on) {
                continue;
            }

            State toggled = with_toggled_lighit_in(lightRoom);

            Action act;
            act.type = Action::SWITCH;
            act.room = lightRoom;
            act.turned_on = !was_on;

            result.push_back(std::make_pair(toggled, act));
        }

        return result;
    }
};



class App {
private:
    std::vector<std::vector<int>> doors, switches;
    House house;
    int room_count = 0;

    //cilovy stav
    bool is_goal_state(const State& s, int goal_room) {
        if (s.get_current_room() != goal_room) { return false;}

        const auto& lights = s.get_light();
        for (std::size_t i = 0; i < lights.size(); i++) {
            bool on = lights.at(i);
            if (static_cast<int>(i) == goal_room) {
                if (!on) { return false; }
            } else {
                if (on) { return false; }
            }
        }
        return true;
    }



    //orchestrace
    bool solve_bfs_shortest_path(const House& h, const State& s, std::vector<Action>& path) {
        using Key = std::pair<int, int>; //room, mask(lights)
        std::map<Key, Key> parent;
        std::map<Key, Action> how;

        std::queue<State> q;
        std::set<Key> visited;

        int start_room = s.get_current_room();
        int start_mask = s.to_mask();
        int goal_room = h.room_count() - 1; // posledni v poli je cil

        Key start_key(start_room, start_mask);
        Key goal_key(-1, -1);

        q.push(s);
        visited.insert(start_key);

        bool found = false;

        while (!q.empty()) {
            State cur = q.front();
            q.pop();

            int cur_room = cur.get_current_room();
            int cur_mask = cur.to_mask();
            Key curKey(cur_room, cur_mask);

            if (is_goal_state(cur, goal_room)) {
                found = true;
                goal_key = curKey;
                break;
            }

            //pro me: rebulid: state action dostanu ted rovnou -> nemusim zpetne dohledavat co bylo za akci
            std::vector<std::pair<State, Action> > neighbors = cur.expand_neighbors(h);

            for (std::size_t i = 0; i < neighbors.size(); ++i) {
                const State&  nxt = neighbors[i].first;
                const Action& act = neighbors[i].second;

                int nxt_room = nxt.get_current_room();
                int nxt_mask = nxt.to_mask();
                Key nxtKey(nxt_room, nxt_mask);

                if (visited.count(nxtKey)) continue;
                visited.insert(nxtKey);

                parent[nxtKey] = curKey;
                how[nxtKey]    = act;

                q.push(nxt);
            }
        }

        if (!found) return false;

        // Rekonstrukce cesty z parent + how: goal_key -> start_key
        std::vector<Action> reversed;
        Key k = goal_key;

        while (k != start_key) {
            Action a = how[k];
            reversed.push_back(a);
            k = parent[k];
        }
        //otoceni poradi
        path.assign(reversed.rbegin(), reversed.rend());
        return true;
    }


    bool read_input(std::istream& in) {
        int r, d, s;
        if (!(in >> r >> d >> s)) {
            return false;
        }

        if (r == 0 && d == 0 && s == 0) return false;

        room_count = r;
        doors.assign(r, {});
        switches.assign(r, {});
        house = House();

        //doors
        for (int i = 0; i < d; ++i) {
            int a, b;
            in >> a >> b;
            --a; --b; // 0-based
            doors.at(a).push_back(b);
            doors.at(b).push_back(a); //neorientovany
        }

        //switches
        for (int i = 0; i < s; ++i) {
            int k, l;
            in >> k >> l;
            --k; --l;
            switches.at(k).push_back(l); // orientovany
        }

        //House build
        for (int i = 0; i < r; ++i) {
            this->house.add_room(Room(i,doors.at(i),switches.at(i)));
        }
        return true;
    }


    void write_output(std::ostream& out, int case_number, const std::vector<Action>& path, bool solvable) {
        out << "Villa #" << case_number << "\n";

        if (!solvable) {
            out << "The problem cannot be solved.\n\n";
            return;
        }

        int steps = static_cast<int>(path.size());
        out << "The problem can be solved in " << steps << " step";
        if (steps != 1) out << "s";
        out << ":\n";

        for (const auto& a : path) {
            if (a.type == Action::MOVE) {
                out << "- Move to room " << (a.room + 1) << ".\n";
            } else { // SWITCH
                out << "- Switch "
                    << (a.turned_on ? "on" : "off")
                    << " light in room " << (a.room + 1) << ".\n";
            }
        }
        out << "\n";
    }



public:

    void run(std::istream& in, std::ostream& out) {
        int case_number = 1;
        while (read_input(in)) {
            std::vector<bool> start_light(room_count, false);
            start_light[0] = true;
            State start_state(0, start_light);

            std::vector<Action> path;
            bool solvable = solve_bfs_shortest_path(house, start_state, path);

            write_output(out, case_number++, path, solvable);
        }

    };

};




int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    App app;
    app.run(std::cin, std::cout);


    return 0;
}