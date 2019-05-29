//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONFLICT_H
#define PATHFINDING_PROJECT_CONFLICT_H

#include "../../utility.h"
#include "Constraint.h"
#include <map>
#include <utility>

struct Conflict {
    int agent_id1, agent_id2;
    int time_step;

    Conflict(int agentId1, int agentId2, int timeStep) : agent_id1(agentId1), agent_id2(agentId2),
                                                         time_step(timeStep) {}

    virtual Constraint constructConstraint(const int& agent_no) = 0;
};

struct VertexConflict : Conflict {
    Position position;

    VertexConflict(int agentId1, int agentId2, int timeStep, const Position &position)
            : Conflict(agentId1, agentId2, timeStep), position(position) {}

    Constraint constructConstraint(const int &agent_no) override {
        return Constraint(agent_no == 1 ? agent_id2 : agent_id1, position, time_step);
    }
};

struct EdgeConflict : Conflict {
    Position init_pos_agent1, init_pos_agent2;
    Position next_pos_agent1, next_pos_agent2;

    EdgeConflict(int agentId1, int agentId2, int timeStep, const Position &init_pos_agent1, const Position &init_pos_agent2,
            const Position &next_pos_agent1, const Position &next_pos_agent2)
            : Conflict(agentId1, agentId2, timeStep), init_pos_agent1(init_pos_agent1), init_pos_agent2(init_pos_agent2),
              next_pos_agent1(next_pos_agent1), next_pos_agent2(next_pos_agent2) {}

    Constraint constructConstraint(const int &agent_no) override {
        return Constraint(agent_no == 1 ? agent_id2 : agent_id1, agent_no == 1 ? next_pos_agent2 : next_pos_agent1, time_step);
    }
};


#endif //PATHFINDING_PROJECT_CONFLICT_H
