//
// Created by Paul Tortel on 22/05/2019.
//

#include "../../../include/solver/cbs/ConflictBasedSearch.h"
#include "../../../include/stats/StatsManager.h"
#include <set>
#include <iostream>
#include <ctime>


std::map<int, State> ConflictBasedSearch::solve() {

    if (_status != Status::OK) { return std::map<int, State>(); }

    statsManager.startAnalysis(agents.size());

    // We call the CBS algorithm
    std::map<int, State> solution = highLevelSolver();

    statsManager.stopAnalysis();

    return solution;
}

std::map<int, State> ConflictBasedSearch::highLevelSolver() {

    ConstraintNode root;

    // We set the root solution with a first solution found without any constrained
    root.solution = lowLevelSolver(root);

    if (_status == NO_SOLUTION) {
        std::cout << "No solution found for this problem..." << std::endl;
        return root.solution.dictionary;
    }

    statsManager.getCurrentAnalysis().constraint_nodes_created++;

    // We compute the sic heuristic of the node
    root.computeSicHeuristic();

    MultimapConstraintNode open_list;
    open_list.emplace(root.cost, root);

    while (!open_list.empty()) {

        // We get the less conflicted node by scanning through all the nodes of low cost
        const auto& it_open_list = getIteratorOnLessConflictNode(open_list);
        ConstraintNode current_node = it_open_list->second;
        open_list.erase(it_open_list);

        std::shared_ptr<Conflict> conflict = current_node.first_conflict;

        // If there is no conflict, we have found a valid and admissible solution
        if (conflict == nullptr) {
            for (auto& it : current_node.solution.dictionary) {
                std::cout << "T" << it.first << ": ";

                for (auto &it_search_square : it.second.getSearchSquares()) {
                    std::cout << "A" << it_search_square.first << " = " << it_search_square.second->position << "|" << it_search_square.second->interacting_time_left << "|" << it_search_square.second->agent_status << "; ";
                }
                std::cout << std::endl;
            }

            return current_node.solution.dictionary;
        }

        // There is a conflict between two agents, therefore we will add 2 news constraint nodes
        for (int i = 1; i <= 2; i++) {
            ConstraintNode new_node;
            statsManager.getCurrentAnalysis().constraint_nodes_created++;

            new_node.constraints = current_node.constraints;
            const int& agent_id = i == 1 ? conflict->agent_id2 : conflict->agent_id1;

            if (agent_id == -1) {
                continue;
            }

            // We construct the new constraint node by merging the constraints of the current node with a new constraint coming from the conflict detected
             new_node.constraints[agent_id].emplace_back(conflict->constructConstraint(i));

            if (auto *e = dynamic_cast<EdgeConflict*>(conflict.get())) {
                auto newly_added_constraint = conflict->constructConstraint(i);
                for (auto& agent_constraints : new_node.constraints) {
                    if (agent_constraints.first == newly_added_constraint.agent_id) {
                        continue;
                    } else {
                        std::vector<std::vector<Constraint>::iterator> constraints_to_erase;
                        for (auto it_constraint = agent_constraints.second.begin(); it_constraint < agent_constraints.second.end(); ++it_constraint) {
                            if (newly_added_constraint.agent_id == it_constraint->agent_causing_constraint && newly_added_constraint.time_step == it_constraint->time_step
                                && newly_added_constraint.position == it_constraint->position) {
                                constraints_to_erase.push_back(it_constraint);
                            }
                        }

                        for (auto& it : constraints_to_erase) {
                            agent_constraints.second.erase(it);
                        }
                    }
                }
            }

            // We check if the new node is not already inside the open list to avoid redundant nodes
            if (isNodeAlreadyInOpenList(open_list, new_node)) {
                continue;
            }

            new_node.solution = current_node.solution;
            // We compute the new solution for one of the agent conflicting, with the new node (and new constraints)
            new_node.solution = lowLevelSolver(new_node, agent_id);

            if (_status == NO_SOLUTION) {
                _status = Status::OK;
                continue;
            }

            new_node.computeSicHeuristic();

            // If the sic heuristic is correct
            if (new_node.cost >= 0) {
                // We insert the new constraint node to the open list, so we can analyze its solution later
                open_list.emplace(new_node.cost, new_node);
            }
        }

        if (open_list.empty()) {
            std::cerr << "No solution found for this problem..." << std::endl;
            _status = Status::NO_SOLUTION;

            for (auto& it : current_node.solution.dictionary) {
                std::cout << "B" << it.first << ": ";

                for (auto &it_search_square : it.second.getSearchSquares()) {
                    std::cout << "A" << it_search_square.first << " = " << it_search_square.second->position << "|" << it_search_square.second->interacting_time_left << "|" << it_search_square.second->agent_status << "; ";
                }
                std::cout << std::endl;
            }

        } else {
            _status = Status::OK;
        }
    }

    return root.solution.dictionary;
}
/*
Solver::MultimapConstraintNode::iterator ConflictBasedSearch::getIteratorOnLessConflictNode(Solver::MultimapConstraintNode &open_list) {

    MultimapConstraintNode::iterator it_low, it_up, it_optimized;
    // We set it_low and it_up on the lower and upper bounds of the smallest key of the open list
    it_low = open_list.lower_bound(open_list.begin()->first);
    it_up = open_list.upper_bound(open_list.begin()->first);
    it_optimized = open_list.begin();

    // Number of constraints in the first node
    int num_of_constraints = it_optimized->second.constraints.size();

    for (auto it = it_low; it != it_up; ++it) {
        int new_num_of_constraints = it->second.constraints.size();
        if (new_num_of_constraints < num_of_constraints) {
            it_optimized = it;
            num_of_constraints = new_num_of_constraints;
        }
        else if (new_num_of_constraints == num_of_constraints) {
            if (it->second.constraints.size() < it_optimized->second.constraints.size()) {
                it_optimized = it;
            }
        }
    }

    return it_optimized;
}*/


Solver::MultimapConstraintNode::iterator ConflictBasedSearch::getIteratorOnLessConflictNode(Solver::MultimapConstraintNode &open_list) {

    MultimapConstraintNode::iterator it_low, it_up, it_optimized;
    // We set it_low and it_up on the lower and upper bounds of the smallest key of the open list
    it_low = open_list.lower_bound(open_list.begin()->first);
    it_up = open_list.upper_bound(open_list.begin()->first);

    bool first_to_assign = true;

    // Number of constraints in the first node
    int num_of_conflicts = 0;

    for (auto it = it_low; it != it_up; ++it) {

        it->second.scanSolution(num_of_conflicts, getAgentsGoal());
        int new_num_of_conflicts = it->second.conflicts_detected;

        if (first_to_assign || new_num_of_conflicts < num_of_conflicts) {
            first_to_assign = false;
            it_optimized = it;
            num_of_conflicts = new_num_of_conflicts;
        } else if (new_num_of_conflicts == num_of_conflicts) {
            if (it->second.constraints.size() < it_optimized->second.constraints.size()) {
                it_optimized = it;
            }
        }
    }

    return it_optimized;
}

ConflictBasedSearch::ConflictBasedSearch(Map &map, const std::map<int, Agent> &agents, StatsManager &statsManager)
        : Solver(map, agents, statsManager) {}

StateDictionary ConflictBasedSearch::lowLevelSolver(ConstraintNode &constraint_node,
                                                    int agent_id) {
    StateDictionary state_dictionary = constraint_node.solution;

    // If we want to compute the whole paths
    if (agent_id == 0) {
        for (auto& it_agent : agents) {
            statsManager.startMeasuringDuration(StatsManager::LOW_LEVEL);
            auto final_search_square = computeShortestPathPossible(it_agent.second, constraint_node);
            statsManager.stopMeasuringDuration(StatsManager::LOW_LEVEL);
            if (_status == NO_SOLUTION) {
                return state_dictionary;
            }
            recordStatesFromPath(it_agent.first, final_search_square, state_dictionary);
        }
    } else {
        // If we want to compute only the path for an agent
        const auto& it_agent = agents.find(agent_id);
        statsManager.startMeasuringDuration(StatsManager::LOW_LEVEL);
        auto final_search_square = computeShortestPathPossible(it_agent->second, constraint_node);
        statsManager.stopMeasuringDuration(StatsManager::LOW_LEVEL);
        if (_status == NO_SOLUTION) {
            return state_dictionary;
        }
        recordStatesFromPath(agent_id, final_search_square, state_dictionary);
    }

    return state_dictionary;
}

std::shared_ptr<SearchSquare> ConflictBasedSearch::computeShortestPathPossible(Agent &agent,
                                                                               ConstraintNode &constraint_node) {

    const int& agent_id = agent.getId();
    MultimapSearchSquare open_list;
    std::set<std::string> closed_list;

    // We populate the open list with the initial search square, wrapping the initial position of the agent
    std::shared_ptr<SearchSquare> current_search_square = init_state.getSearchSquares().at(agent_id);
    open_list.emplace(current_search_square->cost(), current_search_square);

    do {
        // We select the search square having the cheapest cost: it becomes our current search square
        const auto& it_open_list = getIteratorOnStateWithLessConflict(open_list, constraint_node);
        current_search_square = it_open_list->second;

        // We insert the coordinates in the closed list, so we won't deal with the position ever again
        std::string pos_coord = std::to_string(current_search_square->position.x) + ";" + std::to_string(current_search_square->position.y)
                + ";" ;//+ std::to_string(current_search_square->time_step);
        closed_list.emplace(pos_coord);

        // We remove the search square from the open list
        open_list.erase(it_open_list);

        // We populate the open list with the surroundings of the current search square
        populateOpenList(open_list, closed_list, agent, current_search_square, constraint_node);

        if (open_list.empty() && current_search_square->position != getAgentGoalPosition(agent)) {
            _status = NO_SOLUTION;
        }

        if (current_search_square->position == getAgentGoalPosition(agent)) {

            PointOfInterest point_of_interest = map.getInterestOfPosition(current_search_square->position);
            std::shared_ptr<SearchSquare> new_current_search_square = current_search_square;

            if (current_search_square->interacting_time_left == 0) {
                new_current_search_square = std::make_shared<SearchSquare>(current_search_square->position,
                                                                           current_search_square,
                                                                           current_search_square->cost_movement,
                                                                           current_search_square->cost_heuristic);
                switch (point_of_interest) {

                    case NA:
                        new_current_search_square = current_search_square;
                        current_search_square->setCurrentStatus(SearchSquare::AgentStatus::FINISHED,
                                                                PointOfInterest::NA);
                        break;
                    case PRODUCT:
                        new_current_search_square->setCurrentStatus(SearchSquare::AgentStatus::PICKING,
                                                                    point_of_interest);
                        break;
                    case REPAIR_POINT:
                        new_current_search_square->setCurrentStatus(SearchSquare::AgentStatus::BEING_FIXED,
                                                                    point_of_interest);
                        break;
                    case DROP_OFF_POINT:
                        new_current_search_square->setCurrentStatus(SearchSquare::AgentStatus::DROPPING,
                                                                    point_of_interest);
                        break;
                }
            }

            while (new_current_search_square->interacting_time_left > 0) {
                int new_interacting_time = new_current_search_square->interacting_time_left - 1;
                SearchSquare::AgentStatus agent_status = new_current_search_square->agent_status;
                new_current_search_square = std::make_shared<SearchSquare>(new_current_search_square->position, new_current_search_square,
                                                                           new_current_search_square->cost_movement, new_current_search_square->cost_heuristic);
                new_current_search_square->agent_status = agent_status;
                new_current_search_square->setInteractingTimeLeft(new_interacting_time);
            }
            current_search_square = new_current_search_square;
        }

        // We loop while we didn't detect that there is no solution or that we didn't reach the goal position of the agent
    } while ((current_search_square->position != getAgentGoalPosition(agent) ||
                constraint_node.doesAgentStillHaveFutureConstraints(agent_id, current_search_square->time_step - 1))
                && _status == Status::OK);

    return current_search_square;
}

Solver::MultimapSearchSquare::iterator ConflictBasedSearch::getIteratorOnStateWithLessConflict(Solver::MultimapSearchSquare &open_list, ConstraintNode& constraint_node) {
    MultimapSearchSquare::iterator it_low, it_up, it_optimized;
    // We set it_low and it_up on the lower and upper bounds of the smallest key of the open list
    it_low = open_list.lower_bound(open_list.begin()->first);
    it_up = open_list.upper_bound(open_list.begin()->first);

    bool first_to_assign = true;
    int smallest_num_conflicts = 0;

    for (auto it = it_low; it != it_up; ++it) {

        auto state_it = constraint_node.solution.dictionary.find(it->second->time_step);

        if (state_it != constraint_node.solution.dictionary.end()) {
            const int conflicts = state_it->second.countAgentsAtPosition(it->second->position);

            if (first_to_assign || conflicts < smallest_num_conflicts) {
                it_optimized = it;
                first_to_assign = false;
                smallest_num_conflicts = conflicts;
            }
        } else {
            return it;
        }
    }

    return it_optimized;
}

void
ConflictBasedSearch::populateOpenList(MultimapSearchSquare &open_list, std::set<std::string> &closed_list,
                                      const Agent &agent,
                                      std::shared_ptr<SearchSquare> &current_agent_position,
                                      ConstraintNode &constraint_node) {

    const int x = current_agent_position->position.x;
    const int y = current_agent_position->position.y;

    const int max_x = map.getGrid().size() - 1;
    const int max_y = map.getGrid()[0].size() - 1;

    // There is a position at the left
    const bool left = x > 0;
    // There is a position at the right
    const bool right = x < max_x;
    // There is a position at the top
    const bool up = y < max_y;
    // There is a position at the bottom
    const bool down = y > 0;

    if (left) {
        Position left_pos = Position(x-1, y);
        // We insert in the open list left_pos only if its coordinates are not in the closed list
        // or it is not already in the open list with a cheapest cost
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, left_pos, constraint_node);

    } if (right) {
        Position right_pos = Position(x+1, y);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, right_pos, constraint_node);

    } if (up) {
        Position up_pos = Position(x, y+1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, up_pos, constraint_node);

    } if (down) {
        Position down_pos = Position(x, y-1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, down_pos, constraint_node);

    }

    // No constraint in the future, the agent is not allowed to wait
    if (!constraint_node.doesAgentStillHaveFutureConstraints(agent.getId(), current_agent_position->time_step - 1)) {
        return;
    }

    tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, current_agent_position->position, constraint_node);
}

ConflictBasedSearch::InsertionOpenListResult
ConflictBasedSearch::tryInsertInOpenList(MultimapSearchSquare &open_list, std::set<std::string> &closed_list,
                                         const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_position,
                                         Position &analyzed_pos, ConstraintNode &constraint_node) {

    const int& time_step = current_agent_position->time_step;



    // If the position (left, right, up, down, top-right etc.) from our current search square is not an accessible square (wall, different level..)
    if (!canAgentAccessPosition(agent, current_agent_position, analyzed_pos)) {
        return FAIL_ENVIRONMENT;
    }

    std::string pos_coord = std::to_string(analyzed_pos.x) + ";" + std::to_string(analyzed_pos.y) + ";";// + std::to_string(time_step);
    // We check that the position has not already been processed (i.e., not in the closed list)
    if (closed_list.find(pos_coord) != closed_list.end() && current_agent_position->position != analyzed_pos) {

        //TODO: backtracking ?
        //int latest_time_step_constraint = constraint_node.getConstraintLatestTimeStepForAgent(agent.getId());
         //&& (time_step < latest_time_step_constraint || latest_time_step_constraint == -1)

        if (constraint_node.doesAgentStillHaveFutureConstraints(agent.getId(), time_step-1)) {
            //return FAIL_CLOSED_LIST;
            closed_list.clear();
        } else {
            //closed_list.clear();
            return FAIL_CLOSED_LIST;
        }
    }

    // If the agent is not allowed to go to this position (constraints are used at this point)
    if (constraint_node.isPositionForbiddenForAgent(agent.getId(), analyzed_pos, time_step + 1, extractDirection(analyzed_pos, current_agent_position->position))) {
        return FAIL_CONSTRAINT;
    }

    const int move_cost = movementCost(*current_agent_position, analyzed_pos, agent.getId());
    const int heuristic = heuristicCost(analyzed_pos, getAgentGoalPosition(agent));

    const float cost = move_cost + heuristic;

    const auto& it_analyzed_pos = findPositionInOpenList(analyzed_pos, open_list);

    // If we realize that the position is already in the open list
    if (it_analyzed_pos != open_list.end()) {
        // If the cost is cheaper with the current path (current search square and its parent)
        if (it_analyzed_pos->second->cost() > cost || (it_analyzed_pos->second->cost() == cost && it_analyzed_pos->second->time_step > time_step + 1)) {
            auto modified_search_square = it_analyzed_pos->second;
            open_list.erase(it_analyzed_pos);

            // We change only the movement cost since the heuristic cost can't change
            modified_search_square->cost_movement = move_cost;
            // We change the parent
            modified_search_square->parent = current_agent_position;
            // We change its time step
            modified_search_square->time_step = current_agent_position->time_step + 1;

            open_list.insert({cost, modified_search_square});
        }
    } else {
        // This is a new position (not yet processed), we create a search square to represent it and we set its parent with the current search square
        // since we reach this new search square thanks to the current search square
        std::shared_ptr<SearchSquare> new_search_square = std::make_shared<SearchSquare>(analyzed_pos, current_agent_position, move_cost, heuristic);
        new_search_square->setCurrentStatus(SearchSquare::AgentStatus::MOVING, NA);
        // We insert it in the open list
        open_list.emplace(cost, new_search_square);
    }

    return OK;
}

bool ConflictBasedSearch::isNodeAlreadyInOpenList(const Solver::MultimapConstraintNode &open_list,
                                                  ConstraintNode &constraint_node) {

    for (const auto& it_nodes : open_list) {
        // We are only interested in nodes that have the same constraints since the solution, computed or not will be similar from the same constraints
        if (it_nodes.second.hasSameConstraintsThan(constraint_node)) {
            return true;
        }
    }
    return false;
}
