//
// Created by Paul Tortel on 13/05/2019.
//

#include "../include/Agent.h"

const std::pair<int, int> &Agent::getStartCoord() const {
    return start_coord;
}

const std::pair<int, int> &Agent::getGoalCoord() const {
    return goal_coord;
}
