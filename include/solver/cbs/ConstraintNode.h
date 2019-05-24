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
    typedef std::map<int, std::vector<std::shared_ptr<Constraint>>> ConstraintMap;
    ConstraintMap constraints;
    std::vector<ConstraintNode> sons;
    std::map<int, std::vector<std::shared_ptr<SearchSquare>>> solution;
    int cost;

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

    const void computeSicHeuristic() {
        int sic = 0.;
        for (auto &it : solution) {
            sic += it.second.back()->time_step;
        }
        cost = sic;
    }

    const std::unique_ptr<Conflict> scanForFirstConflict() {
        std::vector<State> known_positions_at_time_steps;
/*
        for (auto &agent_it : solution) {
            int time_step = agent_it.second->time_step;
            auto& current_search_square = agent_it.second;

            while (current_search_square != nullptr)   {

                if (time_step >= 0) {
                    const auto& other_agent_it = known_positions_at_time_steps[time_step].findAgentAtPosition(current_search_square->position);
                    if (other_agent_it != known_positions_at_time_steps[time_step].positions.end()) {

                        //TODO: get all the agents that are at this position at this time step (not only 1)

                        return std::make_shared<Conflict>(std::vector<int>(other_agent_it->first, agent_it.first), current_search_square->position, time_step);
                    } else {
                        known_positions_at_time_steps[time_step].positions[agent_it.first] = current_search_square->position;
                    }
                } else {
                    State state;
                    state.positions[agent_it.first] = current_search_square->position;
                    known_positions_at_time_steps[time_step] = state;
                }

                current_search_square = current_search_square->parent;
                time_step--;
            }
        }*/

        return nullptr;
    }

    void setAndMergeConstraints(ConstraintMap& former_constraints, const Constraint& new_constraint) {
        this->constraints = former_constraints;
       // this->constraints[new_constraint.agent_id].push_back(new_constraint);
    }
};

#endif //PATHFINDING_PROJECT_CONSTRAINTNODE_H
