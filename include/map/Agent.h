//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_AGENT_H
#define PATHFINDING_PROJECT_AGENT_H

#include "../utility.h"
#include <vector>

class Agent {

private:
    int id;
    Position parking_coord;

    std::vector<int> pickup_list;

    // Number of items still to be picked up by the agent
    int items_to_pickup = 0;

    // Agent has picked up the items and has dropped them off
    bool has_finished = false;

    Position current_position;
    int interacting_time_left = 0;

public:

    Agent(int id, int start_x, int start_y) : id(id) {
        parking_coord = Position(start_x, start_y);
        current_position = Position(start_x, start_y);
    }

    Agent() {};

    int getId() const {
        return id;
    }

    const Position &getParkingCoord() const {
        return parking_coord;
    }

    const std::vector<int> &getPickupList() const {
        return pickup_list;
    }

    void addProductToPickupList(const int product_id) {
        pickup_list.push_back(product_id);
    }

    void computeItemsToPickup() {
        items_to_pickup = pickup_list.size();
    }

    int getItemsToPickup() const {
        return items_to_pickup;
    }

    const Position &getCurrentPosition() const {
        return current_position;
    }

    void setCurrentPosition(const Position &currentPosition) {
        current_position = currentPosition;
    }

    void removeItemToPickup() {
        if (items_to_pickup > 0) {
            items_to_pickup--;
        } else {
            setHasFinished(true);
        }
    }

    bool hasFinished() const {
        return has_finished;
    }

    void setHasFinished(const bool hasFinished) {
        has_finished = hasFinished;
    }

    void setInteractingTimeLeft(const int time_left) {
        interacting_time_left = time_left;
    }

    int getInteractingTimeLeft() const {
        return interacting_time_left;
    }
};


#endif //PATHFINDING_PROJECT_AGENT_H
