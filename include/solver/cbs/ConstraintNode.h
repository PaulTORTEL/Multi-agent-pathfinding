//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONSTRAINTNODE_H
#define PATHFINDING_PROJECT_CONSTRAINTNODE_H

#include <vector>
#include "Constraint.h"

struct ConstraintNode {
    typedef std::map<int, std::vector<std::shared_ptr<Constraint>>> ConstraintMap;
    ConstraintMap constraints;
    std::vector<ConstraintNode> sons;
    std::map<int, SearchSquare> solutions;
    float cost;

    const bool isPositionForbiddenForAgent(const int &agent_id, const Position &position) {

        auto constraints_agent_it = constraints.find(agent_id);

        if (constraints_agent_it == constraints.end()) {
            return false;
        }

        for (auto& constraint : constraints_agent_it->second) {
            if ((*constraint).position == position) {
                return true;
            }
        }

        return false;
    }
};

#endif //PATHFINDING_PROJECT_CONSTRAINTNODE_H
