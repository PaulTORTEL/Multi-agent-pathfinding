//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONFLICT_H
#define PATHFINDING_PROJECT_CONFLICT_H

#include "../../utility.h"

struct Conflict {
    int agent_id_1, agent_id_2;
    Position position;
    int time_step;

    Conflict(int agentId1, int agentId2, const Position &position, int timeStep) : agent_id_1(agentId1),
                                                                                   agent_id_2(agentId2),
                                                                                   position(position),
                                                                                   time_step(timeStep) {}
};
#endif //PATHFINDING_PROJECT_CONFLICT_H
