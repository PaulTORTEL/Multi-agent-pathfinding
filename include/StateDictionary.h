//
// Created by Paul Tortel on 20/05/2019.
//

#ifndef PATHFINDING_PROJECT_STATEDICTIONARY_H
#define PATHFINDING_PROJECT_STATEDICTIONARY_H

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
            if (time_step == 0) {
                new_state = init_state;
            } else {
                new_state = dictionary[time_step-1];
            }

            new_state.search_squares[agent_id] = new_search_square;
            dictionary[time_step] = new_state;
        }
    }

    State * getStateFromTimeStep(const int &time_step) {

        if (dictionary.find(time_step) != dictionary.end()) {
            return &dictionary[time_step];
        }

        return dictionary.empty() ? nullptr : &dictionary[dictionary.size() - 1];
    }

};

#endif //PATHFINDING_PROJECT_STATEDICTIONARY_H
