//
// Created by Paul Tortel on 28/05/2019.
//

#include "../include/State.h"
#include <map>
#include <memory>
#include <utility>
#include <ostream>


/**
 * Find an agent at a given position
 * @param position : the position to look for
 * @return an iterator on the found agent
 */
const std::map<int, std::shared_ptr<SearchSquare>>::iterator State::findAgentAtPosition(const Position &position) {
    for (auto it = search_squares.begin(); it != search_squares.end(); ++it) {
        if (it->second->position == position) {
            return it;
        }
    }
    return search_squares.end();
}

const int State::getSicHeuristic() {
    int sic = 0;
    for (auto &it : search_squares) {
        sic += it.second->time_step;
    }
    return sic;
}

std::unique_ptr<VertexConflict> State::detectVertexConflict(const int& time_step) {

    for (auto& search_squares_it : search_squares) {
        const Position& agent_1_position = search_squares_it.second->position;
        const int& agent_1_id = search_squares_it.first;

        for (auto& search_squares_it2 : search_squares) {
            if (search_squares_it2.first <= search_squares_it.first) {
                continue;
            }

            Position& agent_2_position = search_squares_it2.second->position;
            const int& agent_2_id = search_squares_it2.first;

            if (agent_1_position == agent_2_position) {
                return std::make_unique<VertexConflict>(agent_1_id, agent_2_id, time_step, agent_1_position);
            }
        }
    }
    return nullptr;
}

const std::map<int, std::shared_ptr<SearchSquare>> &State::getSearchSquares() const {
    return search_squares;
}

void State::setSearchSquareForAgent(const int &agent_id, const std::shared_ptr<SearchSquare>& search_square) {
    search_squares[agent_id] = search_square;
}
