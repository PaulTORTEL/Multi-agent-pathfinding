//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONFLICT_H
#define PATHFINDING_PROJECT_CONFLICT_H

#include "../../utility.h"
#include <map>

struct Conflict {
    // Agent ID => time step, position
    std::multimap<int, std::pair<int, Position>> agents_conflicting;

    Conflict(std::multimap<int, std::pair<int, Position>> agents_conflicting) : agents_conflicting(agents_conflicting){}
};
#endif //PATHFINDING_PROJECT_CONFLICT_H
