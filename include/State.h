//
// Created by Paul Tortel on 28/05/2019.
//

#ifndef PATHFINDING_PROJECT_STATE_H
#define PATHFINDING_PROJECT_STATE_H

#include "utility.h"
#include "solver/cbs/Conflict.h"
#include "SearchSquare.h"

class State {

private:
    /**
     * Map the position of all the agents at a given state
     * The key is the ID of the agent
     */
    std::map<int, std::shared_ptr<SearchSquare>> search_squares;

public:

    const std::map<int, std::shared_ptr<SearchSquare>> &getSearchSquares() const;

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

    friend std::ostream &operator<<(std::ostream &os, const State &state) {
        for (auto& it : state.search_squares) {
            os << "A:" << it.first << " " << it.second->position;
        }
        return os;
    }

    const std::map<int, std::shared_ptr<SearchSquare>>::iterator findAgentAtPosition(const Position &position);

    const float getSicHeuristic();

    std::unique_ptr<VertexConflict> detectVertexConflict(const int &time_step);

    std::multimap<int, std::pair<int, Position>>
    getAgentsInFirstConflict(std::map<int, State> &state_dictionary, const int &time_step);

    void setSearchSquareForAgent(const int& agent_id, const std::shared_ptr<SearchSquare>& search_square);
};


#endif //PATHFINDING_PROJECT_STATE_H
