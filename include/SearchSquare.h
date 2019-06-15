//
// Created by Paul Tortel on 17/05/2019.
//

#ifndef PATHFINDING_PROJECT_SEARCHSQUARE_H
#define PATHFINDING_PROJECT_SEARCHSQUARE_H

#include "utility.h"
#include <memory>

struct SearchSquare {
    /**
     * The position related to the search square
     */
    Position position;

    /**
     * The cost of the movement and of the heuristic
     */
    int cost_movement, cost_heuristic;

    /**
     * The time step from which the search square has been created
     */
    int time_step;

    /**
     * A shared pointer on the parent (which is a search square as well)
     */
    std::shared_ptr<SearchSquare> parent;

    explicit SearchSquare(Position position, std::shared_ptr<SearchSquare> parent = nullptr,
                          int cost_movement = 0, int cost_heuristic = 0) :
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

    int cost() const {
        return cost_movement + cost_heuristic;
    }
};
#endif //PATHFINDING_PROJECT_SEARCHSQUARE_H
