//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONSTRAINT_H
#define PATHFINDING_PROJECT_CONSTRAINT_H

#include <memory>
#include "../../Agent.h"

struct Constraint {
    const std::weak_ptr<Agent> agent;
    const Position position;
    const int time_step;

    Constraint(std::shared_ptr<Agent>& agent, const Position &position, const int timeStep) : agent(agent), position(position),
                                                                                           time_step(timeStep) {}
};
#endif //PATHFINDING_PROJECT_CONSTRAINT_H
