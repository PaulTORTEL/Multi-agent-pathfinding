//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONSTRAINTNODE_H
#define PATHFINDING_PROJECT_CONSTRAINTNODE_H

#include <vector>
#include <set>
#include <ostream>
#include "Constraint.h"
#include "Conflict.h"

struct ConstraintNode {
    typedef std::map<int, std::vector<Constraint>> ConstraintMap;
    ConstraintMap constraints;
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

        for (auto it_state = solution.dictionary.begin(); it_state != solution.dictionary.end(); ++it_state) {
            //TODO: detect collision if agents exchange positions or cross each other path

            std::unique_ptr<VertexConflict> vertex_conflict = it_state->second.detectVertexConflict(it_state->first);

            if (vertex_conflict != nullptr) {
                return vertex_conflict;
            }

            auto it_next_state = it_state;
            it_next_state++;

            if (it_next_state != solution.dictionary.end()) {
                std::unique_ptr<EdgeConflict> edge_conflict = solution.detectFirstEdgeConflictFromTwoStates(it_state->first, it_state, it_next_state);

                if (edge_conflict != nullptr) {
                    return edge_conflict;
                }
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

    friend std::ostream &operator<<(std::ostream &os, const ConstraintNode &node) {
        os << "ConstraintNode constraints: " << std::endl;
        for (auto&it : node.constraints) {
            os << "Agent " << it.first << ":" << std::endl;
            for (auto& constraint : it.second) {
                os << constraint << std::endl;
            }
            os << std::endl;
        }
        os << std::endl;
        return os;
    }
};

#endif //PATHFINDING_PROJECT_CONSTRAINTNODE_H
