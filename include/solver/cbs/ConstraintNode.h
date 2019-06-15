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
    int cost = -1;
    std::shared_ptr<Conflict> first_conflict = nullptr;
    int conflicts_detected = 0;

    /**
     * Returns whether the position is allowed for the given agent at this time step
     * @param agent_id : the agent id
     * @param position : the position we evaluate
     * @param time_step : the time step
     * @return true if the position is forbidden, false if the agent can go to this position
     */
    const bool isPositionForbiddenForAgent(const int &agent_id, const Position &position, const int &time_step,
                                           const Direction &edge) {

        auto constraints_agent_it = constraints.find(agent_id);

        if (constraints_agent_it == constraints.end()) {
            return false;
        }

        for (auto& constraint : constraints_agent_it->second) {
            // If there is a constraint at this time step for this position and for this agent
            if (constraint.time_step == time_step && constraint.position == position) {
                if (constraint.edge == NO_DIRECTION) {
                    return true;
                }

                if (constraint.edge == edge) {
                    return true;
                }
            }
        }

        return false;
    }

    const void computeSicHeuristic() {
        cost = solution.dictionary.rbegin()->second.getSicHeuristic();
    }

   /**
     * Scans for conflicts: count and set the first conflict found
     */
    void scanSolution(const int& stop_value) {
        // For each states in the solution
        for (auto it_state = solution.dictionary.begin(); it_state != solution.dictionary.end(); ++it_state) {
            // We try to detect a vertex collision in the given state
            std::unique_ptr<VertexConflict> vertex_conflict = it_state->second.detectVertexConflict(it_state->first);

            if (vertex_conflict != nullptr) {
                if (first_conflict == nullptr) {
                    first_conflict = std::move(vertex_conflict);
                }
                conflicts_detected++;
                if (stop_value != 0 && conflicts_detected >= stop_value) {
                    return;
                }
            }

            // We get the next state
            auto it_next_state = it_state;
            it_next_state++;

            // If the next state exists (it_state was not pointing the last state)
            if (it_next_state != solution.dictionary.end()) {
                // We try to detect an edge collision between the two states
                std::unique_ptr<EdgeConflict> edge_conflict = solution.detectFirstEdgeConflictFromTwoStates(
                        it_state->first, it_state, it_next_state);

                if (edge_conflict != nullptr) {
                    if (first_conflict == nullptr) {
                        first_conflict = std::move(edge_conflict);
                    }
                    conflicts_detected++;
                    if (stop_value != 0 && conflicts_detected >= stop_value) {
                        return;
                    }
                }
            }
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

    /**
     * Returns whether the constraint node has the same constraints that the given one
     * @param rhs : the given constraint node we want to compare
     * @return true if they have the same constraints
     */
    bool hasSameConstraintsThan(const ConstraintNode& rhs) const {
        if (constraints.size() != rhs.constraints.size()) {
            return false;
        }

        // for each agent
        for (const auto& it_agent : constraints) {
            auto it_elem_rhs = rhs.constraints.find(it_agent.first);
            // If there is no constraints for this agent in the rhs, it means they are not similar
            if (it_elem_rhs == rhs.constraints.end()) {
                return false;
            }

            if (it_agent.second.size() != it_elem_rhs->second.size()) {
                return false;
            }

            // For each constraint for the agent
            for (const auto& constraint : it_agent.second) {
                bool found = false;
                // For each constraint for the agent in rhs
                for (const auto& constraint_rhs : it_elem_rhs->second) {
                    // If we have found the same constraint, we can continue
                    if (constraint == constraint_rhs) {
                        found = true;
                        break;
                    }
                }

                // The constraint has not been found in rhs, it means the constraint nodes have not the same constraints
                if (!found) {
                    return false;
                }
            }
        }
        return true;
    }

    bool doesAgentStillHaveFutureConstraints(const int& agent_id, const int& time_step) {
        for (auto& constraint : constraints[agent_id]) {
            if (constraint.time_step > time_step) {
                return true;
            }
        }
        return false;
    }

     int getConstraintLatestTimeStepForAgent(const int& agent_id) {
        int latest = -1;
        for (auto& constraint : constraints[agent_id]) {
            if (latest < constraint.time_step) {
                latest = constraint.time_step;
            }
        }
        return latest;
    }
};

#endif //PATHFINDING_PROJECT_CONSTRAINTNODE_H