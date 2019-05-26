//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONSTRAINTNODE_H
#define PATHFINDING_PROJECT_CONSTRAINTNODE_H

#include <vector>
#include <set>
#include "Constraint.h"
#include "Conflict.h"

struct ConstraintNode {
    typedef std::map<int, std::vector<Constraint>> ConstraintMap;
    ConstraintMap constraints;
    std::vector<ConstraintNode> sons;
    StateDictionary solution;
    int cost = 0;

    const bool isPositionForbiddenForAgent(const int &agent_id, const Position &position, const int& time_step) {

        auto constraints_agent_it = constraints.find(agent_id);

        if (constraints_agent_it == constraints.end()) {
            return false;
        }

        for (auto& constraint : constraints_agent_it->second) {
            if (constraint.time_step == time_step && constraint.position == position) {
                return true;
            }
        }

        return false;
    }

    const void computeSicHeuristic() {
        cost = solution.dictionary.rbegin()->second.getSicHeuristic();
    }

    const std::unique_ptr<Conflict> scanForFirstConflict() {

        for (auto &state_it : solution.dictionary) {
            //TODO: detect collision if agents exchange positions or cross each other path
            std::multimap<int, std::pair<int, Position>> agents_conflicting = state_it.second.getAgentsInFirstConflict(solution.dictionary, state_it.first);

            if (!agents_conflicting.empty()) {
                return std::make_unique<Conflict>(agents_conflicting);
            }
        }

        return nullptr;
    }

    void setAndMergeConstraints(ConstraintMap& former_constraints, const std::vector<Constraint>& new_constraints) {
        this->constraints = former_constraints;

        for (auto& constraint : new_constraints) {
            this->constraints[constraint.agent_id].push_back(constraint);
        }
    }
};

#endif //PATHFINDING_PROJECT_CONSTRAINTNODE_H
