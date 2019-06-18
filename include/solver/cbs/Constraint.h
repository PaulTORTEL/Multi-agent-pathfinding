//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONSTRAINT_H
#define PATHFINDING_PROJECT_CONSTRAINT_H

#include <memory>
#include <ostream>
#include "../../map/Agent.h"

struct Constraint {
    const int agent_id;
    const Position position;
    const int time_step;
    const Direction edge;

    Constraint(const int& agent_id, const Position &position, const int timeStep, const Direction edge = NO_DIRECTION) : agent_id(agent_id), position(position),
                                                                                           time_step(timeStep), edge(edge) {}

    friend std::ostream &operator<<(std::ostream &os, const Constraint &constraint) {
        os << "Constraint = A:" << constraint.agent_id << " " << constraint.position << " T:"
           << constraint.time_step << " Edge? " << constraint.edge;
        return os;
    }

    bool operator==(const Constraint &rhs) const {
        return agent_id == rhs.agent_id &&
               position == rhs.position &&
               time_step == rhs.time_step &&
               edge == rhs.edge;
    }

    bool operator!=(const Constraint &rhs) const {
        return !(rhs == *this);
    }
};
#endif //PATHFINDING_PROJECT_CONSTRAINT_H
