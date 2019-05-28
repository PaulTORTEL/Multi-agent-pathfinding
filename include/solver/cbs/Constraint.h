//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONSTRAINT_H
#define PATHFINDING_PROJECT_CONSTRAINT_H

#include <memory>
#include <ostream>
#include "../../Agent.h"

struct Constraint {
    const int agent_id;
    const Position position;
    const int time_step;

    Constraint(const int& agent_id, const Position &position, const int timeStep) : agent_id(agent_id), position(position),
                                                                                           time_step(timeStep) {}

    friend std::ostream &operator<<(std::ostream &os, const Constraint &constraint) {
        os << "Constraint = A:" << constraint.agent_id << " " << constraint.position << " T:"
           << constraint.time_step;
        return os;
    }
};
#endif //PATHFINDING_PROJECT_CONSTRAINT_H
