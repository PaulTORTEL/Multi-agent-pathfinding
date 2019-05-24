//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_STATE_H
#define PATHFINDING_PROJECT_STATE_H

#include "utility.h"
#include "solver/cbs/Conflict.h"
#include "SearchSquare.h"
#include <map>
#include <memory>

struct State {
    /**
     * Map the position of all the agents at a given state
     * The key is the ID of the agent
     */
    std::map<int, std::shared_ptr<SearchSquare>> search_squares;

    bool operator==(const State &rhs) const {

        // We check the size of the maps first
        if (this->search_squares.size() != rhs.search_squares.size()) {
            return false;
        }

        for (auto& it : this->search_squares) {
            const auto& rhs_it = rhs.search_squares.find(it.first);

            // If rhs doesn't contain the agent with ID = it.first
            if (rhs_it == rhs.search_squares.end()) {
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
    const std::map<int, std::shared_ptr<SearchSquare>>::iterator findAgentAtPosition(const Position &position) {
        for (auto it = search_squares.begin(); it != search_squares.end(); ++it) {
            if (it->second->position == position) {
                return it;
            }
        }
        return search_squares.end();
    }

    const int getSicHeuristic() {
        int sic = 0.;
        for (auto &it : search_squares) {
            sic += it.second->time_step;
        }
        return sic;
    }

    std::pair<Position, std::vector<int>> getAgentsInFirstConflict() {
        std::vector<int> agents_conflicting;

        for (auto& search_squares_it : search_squares) {
            agents_conflicting.push_back(search_squares_it.first);
            Position& pos = search_squares_it.second->position;

            for (auto& search_squares_it2 : search_squares) {
                if (search_squares_it == search_squares_it2 || search_squares_it.first > search_squares_it2.first) {
                    continue;
                }

                if (search_squares_it.second->position == search_squares_it2.second->position) {
                    agents_conflicting.push_back(search_squares_it2.first);
                }
            }

            if (agents_conflicting.size() > 1) {
                return std::make_pair(pos, agents_conflicting);
            }
            agents_conflicting.clear();
        }

        return std::make_pair(Position(0,0), agents_conflicting);
    }
};

#endif //PATHFINDING_PROJECT_STATE_H
