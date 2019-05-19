//
// Created by Paul Tortel on 17/05/2019.
//

#ifndef PATHFINDING_PROJECT_SEARCHSQUARE_H
#define PATHFINDING_PROJECT_SEARCHSQUARE_H

#include "utility.h"
#include "State.h"
#include <memory>


struct State;

struct SearchSquare {
    Position position;
    float cost_movement, cost_heuristic;
    int time_step;

    std::shared_ptr<SearchSquare> parent;

    explicit SearchSquare(Position position, std::shared_ptr<SearchSquare> parent = nullptr,
            float cost_movement = 0, float cost_heuristic = 0) :
                position(position), parent(parent), cost_movement(cost_movement), cost_heuristic(cost_heuristic) {

        if (parent != nullptr) {
            time_step = parent.get()->time_step + 1;
        } else {
            time_step = 0;
        }
    }


    explicit SearchSquare() {
        position = Position(0,0);
        cost_heuristic = 0;
        cost_movement = 0;
        time_step = 0;
        parent = nullptr;
    }

    float cost() const {
        return cost_movement + cost_heuristic;
    }
};
#endif //PATHFINDING_PROJECT_SEARCHSQUARE_H
