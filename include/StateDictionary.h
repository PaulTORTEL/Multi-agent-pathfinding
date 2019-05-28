//
// Created by Paul Tortel on 20/05/2019.
//

#ifndef PATHFINDING_PROJECT_STATEDICTIONARY_H
#define PATHFINDING_PROJECT_STATEDICTIONARY_H

#include <ostream>
#include "State.h"

struct StateDictionary {
    /**
     * Time_step (starting from 0 with the initial state) => State
     */
    std::map<int, State> dictionary;

    /**
     * Add or update the position (in the proper state) of an agent at a given time step
     * @param init_state : the initial state to initialize a new state if necessary
     * @param time_step : the current time step
     * @param agent_id : the agent id that we want to change/update its position
     * @param new_search_square : the new position of the agent in the state
     */
    void addOrUpdateState(const State &init_state, const int time_step, const int agent_id,
                          const std::shared_ptr<SearchSquare> &new_search_square) {
        if (dictionary.find(time_step) != dictionary.end()) {
            // Not new
            dictionary[time_step].search_squares[agent_id] = new_search_square;

        } else {
            // New
            State new_state;

            if (dictionary.empty()) {
                new_state = init_state;
            } else {
                new_state = dictionary.rbegin()->second;
            }

            new_state.search_squares[agent_id] = new_search_square;
            dictionary[time_step] = new_state;
        }
    }

    State * getStateFromTimeStep(const int &time_step) {

        if (dictionary.find(time_step) != dictionary.end()) {
            return &dictionary[time_step];
        }

        return dictionary.empty() ? nullptr : &dictionary.rbegin()->second;
    }

    void deleteStatesForAgentFromTimeStep(int time_step, const int& agent_id) {
        auto it = dictionary.find(time_step);
        while (it != dictionary.end()) {
            it->second.search_squares.erase(agent_id);
            it = dictionary.find(++time_step);
        }
    }

    friend std::ostream &operator<<(std::ostream &os, const StateDictionary &dictionary) {
        os << "Sol: " << std::endl;
        for (auto& it : dictionary.dictionary) {
            os << "T:" << it.first << std::endl << it.second << std::endl;
        }
        os << std::endl;
        return os;
    }
};

#endif //PATHFINDING_PROJECT_STATEDICTIONARY_H
