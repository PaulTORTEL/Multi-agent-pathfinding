//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_STATE_H
#define PATHFINDING_PROJECT_STATE_H

#include "utility.h"
#include "SearchSquare.h"
#include <map>

struct State {
    /**
     * Map the position of all the agents at a given state
     * The key is the ID of the agent
     */
    std::map<int, Position> positions;

    bool operator==(const State &rhs) const {

        // We check the size of the maps first
        if (this->positions.size() != rhs.positions.size()) {
            return false;
        }

        for (auto& it : this->positions) {
            const auto& rhs_it = rhs.positions.find(it.first);

            // If rhs doesn't contain the agent with ID = it.first
            if (rhs_it == rhs.positions.end()) {
                return false;
            }

            // If rhs has a different Position stored for the same agent
            if (it.second != rhs_it->second) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const State &rhs) const {
        return !(rhs == *this);
    }

    const std::map<int, Position>::iterator findAgentAtPosition(const Position &position) {
        for (auto it = positions.begin(); it != positions.end(); ++it) {
            if (it->second == position) {
                return it;
            }
        }
        return positions.end();
    }
};

struct StateDictionary {
    std::map<int, State> dictionary;

    void addOrUpdateState(const State& current_state, const int time_step, const int agent_id, const Position& new_position) {
        if (dictionary.find(time_step) != dictionary.end()) {
            // Not new
            dictionary[time_step].positions[agent_id] = new_position;

        } else {
            // New
            State new_state = current_state;
            new_state.positions[agent_id] = new_position;
            dictionary[time_step] = new_state;
        }
    }

};
#endif //PATHFINDING_PROJECT_STATE_H
