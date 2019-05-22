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
    std::map<int, std::shared_ptr<SearchSquare>> solution;
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
            sic += it.second->time_step;
        }
        cost = sic;
    }

    const std::shared_ptr<Conflict> getFirstConflict() {
        //TODO: store not only the positions, but also the id of the agent that is occupying the position
        std::vector<std::set<Position>> known_positions_at_time_steps;

        for (auto &agent_it : solution) {
            int time_step = 0;
            auto& current_search_square = agent_it.second;

            while (current_search_square != nullptr)   {
                std::set<Position> positions_at_this_time_step = known_positions_at_time_steps[time_step];
                if (positions_at_this_time_step.find(current_search_square->position) != positions_at_this_time_step.end()) {
                    //return std::make_shared<Conflict>()
                }
                current_search_square = current_search_square->parent;
                time_step++;
            }
           // paths.push_back(path);
        }
    }
};

#endif //PATHFINDING_PROJECT_CONSTRAINTNODE_H
