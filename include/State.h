//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_STATE_H
#define PATHFINDING_PROJECT_STATE_H

#include "utility.h"
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

    /**
     * Find an agent at a given position
     * @param position : the position to look for
     * @return an iterator on the found agent
     */
    const std::map<int, Position>::iterator findAgentAtPosition(const Position &position) {
        for (auto it = positions.begin(); it != positions.end(); ++it) {
            if (it->second == position) {
                return it;
            }
        }
        return positions.end();
    }
};

#endif //PATHFINDING_PROJECT_STATE_H
