//
// Created by Paul Tortel on 22/05/2019.
//

#include "../../../include/solver/cbs/ConflictBasedSearch.h"
#include <set>
#include <iostream>


void ConflictBasedSearch::solve() {

    highLevelSolver();
}

void ConflictBasedSearch::highLevelSolver() {

    root.constraints.clear();
    root.solution = lowLevelSolver(root);

    if (_status == NO_SOLUTION) {
        std::cout << "No solution found for this problem..." << std::endl;
        return;
    }

    root.computeSicHeuristic();

    MultimapConstraintNode open_list;
    open_list.insert({root.cost, root});

    while (!open_list.empty()) {

        const auto& it_open_list = open_list.begin();
        ConstraintNode current_node = it_open_list->second;
        open_list.erase(it_open_list);
       // std::cout << current_node << std::endl;
       // std::cout << current_node.solution << std::endl;

        //TODO: detecting exisiting constraint node to avoid duplicating
        //TODO: optimization with shared ptr/unique ptr for conflicts, constraintNode, map.rbegin()->first rather than .size()-1 etc...
        //TODO: wait action ??

        std::unique_ptr<Conflict> conflict = current_node.scanForFirstConflict();

        if (conflict == nullptr) {
            std::cout << "solution found" << std::endl;

            for (auto& it : current_node.solution.dictionary) {
                std::cout << "TIMESTEP " << it.first << ": ";

                for (auto &it_search_square : it.second.getSearchSquares()) {
                    std::cout << "Agent " << it_search_square.first << " = " << it_search_square.second->position << "; ";
                }
                std::cout << std::endl;
            }

            return;
        }

        for (int i = 1; i <= 2; i++) {
            ConstraintNode new_node;
            new_node.solution = current_node.solution;
            new_node.constraints = current_node.constraints;
            const int& agent_id = i == 1 ? conflict->agent_id2 : conflict->agent_id1;

            new_node.constraints[agent_id].emplace_back(conflict->constructConstraint(i));
            new_node.solution = lowLevelSolver(new_node, agent_id);

            if (_status == NO_SOLUTION) {
                break;
            }

            new_node.computeSicHeuristic();

            if (new_node.cost >= 0) {
                open_list.insert({new_node.cost, new_node});
            }
        }

        if (open_list.empty()) {
            std::cout << "No solution found for this problem..." << std::endl;
        } else {
            _status = OK;
        }
    }
}

ConflictBasedSearch::ConflictBasedSearch(Map &map) : Solver(map){}

StateDictionary ConflictBasedSearch::lowLevelSolver(ConstraintNode &constraint_node,
                                                    int agent_id) {
    StateDictionary state_dictionary = constraint_node.solution;

    // If we want to compute the whole paths
    if (agent_id == 0) {
        for (auto& it_agent : map.getAgents()) {
            auto final_search_square = computeShortestPathPossible(it_agent.second, constraint_node);
            if (_status == NO_SOLUTION) {
                return state_dictionary;
            }
            recordStatesFromPath(it_agent.first, final_search_square, state_dictionary);
        }
    } else {
        // If we want to compute only the path for an agent
        const auto& it_agent = map.getAgents().find(agent_id);
        auto final_search_square = computeShortestPathPossible(it_agent->second, constraint_node);
        if (_status == NO_SOLUTION) {
            return state_dictionary;
        }
        recordStatesFromPath(agent_id, final_search_square, state_dictionary);
    }

    return state_dictionary;
}

std::shared_ptr<SearchSquare> ConflictBasedSearch::computeShortestPathPossible(const Agent &agent,
                                                                               ConstraintNode &constraint_node) {

    const int& agent_id = agent.getId();
    MultimapSearchSquare open_list;
    std::set<std::string> closed_list;

    // We populate the open list with the initial search square, wrapping the initial position of the agent
    std::shared_ptr<SearchSquare> current_search_square = init_state.getSearchSquares().at(agent_id);
    open_list.insert({current_search_square->cost(), current_search_square});

    do {
        // We select the search square having the cheapest cost: it becomes our current search square
        const auto& it_open_list = open_list.begin();
        current_search_square = it_open_list->second;

        // We insert the coordinates in the closed list, so we won't deal with the position ever again
        std::string pos_coord = std::to_string(current_search_square->position.x) + ";" + std::to_string(current_search_square->position.y);
        closed_list.insert(pos_coord);

        // We remove the search square from the open list
        open_list.erase(it_open_list);

        // We populate the open list with the surroundings of the current search square
        populateOpenList(open_list, closed_list, agent, current_search_square, constraint_node);

        if (open_list.empty()) {
            _status = NO_SOLUTION;
        }

        // We loop while we didn't detect that there is no solution or that we didn't reach the goal position of the agent
    } while (current_search_square->position != agent.getGoalCoord() && _status == OK);

    return current_search_square;
}

void
ConflictBasedSearch::populateOpenList(MultimapSearchSquare &open_list, const std::set<std::string> &closed_list, const Agent &agent,
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
    } if (left && up) {
        Position left_up_pos = Position(x-1, y+1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, left_up_pos, constraint_node);
    } if (right && up) {
        Position right_up_pos = Position(x+1, y+1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, right_up_pos, constraint_node);
    } if (left && down) {
        Position left_down_pos = Position(x-1, y-1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, left_down_pos, constraint_node);
    } if (right && down) {
        Position right_down_pos = Position(x+1, y-1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, right_down_pos, constraint_node);
    }
}

void ConflictBasedSearch::tryInsertInOpenList(MultimapSearchSquare &open_list, const std::set<std::string> &closed_list,
                                              const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_position,
                                              Position &analyzed_pos, ConstraintNode &constraint_node) {

    const int& time_step = current_agent_position->time_step;

    //TODO: if water..., add it here
    //TODO: check if the agent and the square are on the same level or that the agent can climb/use "staircases"

    // If the position (left, right, up, down, top-right etc.) from our current search square is a walkable square
    if (map.getMapSquare(analyzed_pos).type != WALL) {

        std::string pos_coord = std::to_string(analyzed_pos.x) + ";" + std::to_string(analyzed_pos.y);
        // We check that the position has not already been processed (i.e., not in the closed list)
        if (closed_list.find(pos_coord) != closed_list.end()) {
            // Already processed, we stop here for this position
            return;
        }

        if (constraint_node.isPositionForbiddenForAgent(agent.getId(), analyzed_pos, time_step+1)) {
            return;
        }

        const float move_cost = movement_cost(*current_agent_position, analyzed_pos);
        const float heuristic = heuristic_cost(analyzed_pos, agent.getGoalCoord());

        const float cost = move_cost + heuristic;

        const auto& it_analyzed_pos = findPositionInOpenList(analyzed_pos, open_list);

        // If we realize that the position is already in the open list
        if (it_analyzed_pos != open_list.end()) {
            // If the cost is cheaper with the current path (current search square and its parent)
            if (it_analyzed_pos->second->cost() > cost) {
                // We change only the movement cost since the heuristic cost can't change
                it_analyzed_pos->second->cost_movement = move_cost;
                // We change the parent
                it_analyzed_pos->second->parent = current_agent_position;
                // We change its time step
                it_analyzed_pos->second->time_step = current_agent_position->time_step + 1;
            }
        } else {
            // This is a new position (not yet processed), we create a search square to represent it and we set its parent with the current search square
            // since we reach this new search square thanks to the current search square
            std::shared_ptr<SearchSquare> new_search_square = std::make_shared<SearchSquare>(analyzed_pos, current_agent_position, move_cost, heuristic);
            // We insert it in the open list
            open_list.insert({cost, new_search_square});
        }
    }
}
