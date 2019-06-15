//
// Created by Paul Tortel on 28/05/2019.
//

#include "../include/State.h"
#include <map>
#include <memory>
#include <utility>
#include <ostream>

const std::map<int, std::shared_ptr<SearchSquare>>::iterator State::findAgentAtPosition(const Position &position) {
    for (auto it = search_squares.begin(); it != search_squares.end(); ++it) {
        // If the agent is at this position, we return the iterator on this agent
        if (it->second->position == position) {
            return it;
        }
    }
    return search_squares.end();
}

const int State::countAgentsAtPosition(const Position &position) {
    int counter = 0;
    for (auto & search_square : search_squares) {
        // If the agent is at this position, we return the iterator on this agent
        if (search_square.second->position == position) {
            counter++;
        }
    }
    return counter;
}

const int State::getSicHeuristic() {
    int sic = 0;
    for (auto &it : search_squares) {
        sic += it.second->cost();
    }
    return sic;
}

std::unique_ptr<VertexConflict> State::detectVertexConflict(const int& time_step) {

    // We loop through all the search squares (all the agents)
    for (auto& search_squares_it : search_squares) {
        // We get the position of the current agent and its ID
        const Position& agent_1_position = search_squares_it.second->position;
        const int& agent_1_id = search_squares_it.first;

        // We loop through all the search squares again
        for (auto& search_squares_it2 : search_squares) {
            // If the ID are the same, we skip this search square
            if (search_squares_it2.first <= search_squares_it.first) {
                continue;
            }

            // We get the position of the second agent, and its ID
            Position& agent_2_position = search_squares_it2.second->position;
            const int& agent_2_id = search_squares_it2.first;

            // If the position of both agents are the same, it means we have a vertex conflict since they are both at the same time at the same place
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
