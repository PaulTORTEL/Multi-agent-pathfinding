//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONFLICT_H
#define PATHFINDING_PROJECT_CONFLICT_H

#include "../../utility.h"
#include <vector>

struct Conflict {
    std::vector<int> agents_conflicting;
    Position position;
    int time_step;

    Conflict(std::vector<int> agents_conflicting, const Position &position, int timeStep) : agents_conflicting(agents_conflicting), position(position),
                                                                                   time_step(timeStep) {}
};
#endif //PATHFINDING_PROJECT_CONFLICT_H
