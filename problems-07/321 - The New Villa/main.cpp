#include <iostream>
#include <vector>

// topologie: House of (rooms)
class Room {
private:
    int room_id;
    //bool light; -> do abstratni vrstavy state, duplicitni tady
    std::vector<int> neighbors_of;
    std::vector<int> switchable_from;

public:
    Room(int room_id,
        const std::vector<int>& neighbors_of,
        const std::vector<int>& switchable_from )
    : room_id(room_id),
    neighbors_of(neighbors_of),
    switchable_from(switchable_from) {}

    int get_id() const { return room_id; }

    const std::vector<int>& get_neighbors() const { return neighbors_of; }
    const std::vector<int>& get_switchable_from() const { return switchable_from; }

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
    const std::vector<int>& switchable_from(int room_id) const {
        return rooms.at(room_id).get_switchable_from();
    }


};

// abstract layer - state and BFS

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
};


int main() {

    return 0;
}