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
     * @param new_position : the new position of the agent in the state
     */
    void addOrUpdateState(const State& init_state, const int time_step, const int agent_id, const Position& new_position) {
        if (dictionary.find(time_step) != dictionary.end()) {
            // Not new
            dictionary[time_step].positions[agent_id] = new_position;

        } else {
            // New
            State new_state = init_state;
            new_state.positions[agent_id] = new_position;
            dictionary[time_step] = new_state;
        }
    }

};

#endif //PATHFINDING_PROJECT_STATEDICTIONARY_H
