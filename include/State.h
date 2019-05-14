//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_STATE_H
#define PATHFINDING_PROJECT_STATE_H

#include "utility.h"
#include <map>

struct State {
    std::map<int, Position> positions;
};
#endif //PATHFINDING_PROJECT_STATE_H
