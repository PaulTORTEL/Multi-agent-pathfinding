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

    /**
    * Constructs and returns the constraints according to the agent number:
     * if we are dealing with the first agent (==1), we want to build constraint for the second agent
     * if we are dealing with the second agent (==2), we want to build constraint for the first agent
    * @param agent_no : the first or second agent
    * @return the new constraint
    */
    virtual Constraint constructConstraint(const int& agent_no) = 0;
};

struct VertexConflict : Conflict {
    Position position;

    VertexConflict(int agentId1, int agentId2, int timeStep, const Position &position)
            : Conflict(agentId1, agentId2, timeStep), position(position) {}


    Constraint constructConstraint(const int &agent_no) override {
        return Constraint(agent_no == 1 ? agent_id2 : agent_id1, position, time_step, agent_no == 1 ? agent_id1 : agent_id2);
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
        if (agent_no == 1) {
            return Constraint(agent_id2, next_pos_agent2, time_step, agent_id1, extractDirection(next_pos_agent2, init_pos_agent2));
        } else {
            return Constraint(agent_id1, next_pos_agent1, time_step, agent_id2, extractDirection(next_pos_agent1, init_pos_agent1));
        }
    }

    bool isTheSameThan(EdgeConflict* edge_conflict) {
        if (this->agent_id1 != edge_conflict->agent_id2) {
            return false;
        }

        if (this->agent_id2 != edge_conflict->agent_id1) {
            return false;
        }

        if (this->time_step != edge_conflict->time_step) {
            return false;
        }

        return this->init_pos_agent1 == edge_conflict->init_pos_agent2 &&
               this->init_pos_agent2 == edge_conflict->init_pos_agent1 &&
               this->next_pos_agent1 == edge_conflict->next_pos_agent2 &&
               this->next_pos_agent2 == edge_conflict->next_pos_agent1;

    }
};


#endif //PATHFINDING_PROJECT_CONFLICT_H
