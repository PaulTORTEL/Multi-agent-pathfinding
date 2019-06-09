//
// Created by Paul Tortel on 15/05/2019.
//

#include <iostream>
#include <set>
#include <ctime>
#include <numeric>
#include "../../include/solver/SimpleSequentialSolver.h"
#include "../../include/solver/Solver.h"

SimpleSequentialSolver::SimpleSequentialSolver(Map &map) : Solver(map) {}

void SimpleSequentialSolver::solve() {

    if (_status != OK) { return; }

    std::clock_t start;
    double duration;

    start = std::clock();

    bool should_retry = false;
    int gap = 0;

    std::map<int, std::shared_ptr<SearchSquare>> result;

    do {
        _status = OK;
        // We get the list of agent ID ordered with the gap
        // if the gap == 0 => 1,2,3,4... if the gap == 2 => 3,4,1,2
        std::vector<int> agent_ids_list = getAgentsOrderWithGap(gap);

        // We loop through all the agents
        for (int& agent_id : agent_ids_list) {

            std::shared_ptr<SearchSquare> search_square = computeShortestPathPossible(map.getAgents().at(agent_id));
            // If there is no solution for one agent
            if (_status == NO_SOLUTION) {

                // We increment gap to try to solve the problem by starting with the next agent first
                gap++;

                // If every agent has been treated first but we still have no solution
                if (gap == map.getAgents().size()) {
                    std::cout << "NO SOLUTION FOUND" << std::endl;
                    should_retry = false;
                } else {
                    // We still have agent(s) to treat first
                    should_retry = true;
                }

                break;
            }

            result[agent_id] = search_square;
        }
    } while (should_retry);

    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    std::cout << "Time elapsed: " << duration <<'\n';

    if (_status == OK) {
        for (const auto& agent : result) {
            std::cout << "Agent " << agent.first << ": ";
            std::shared_ptr<SearchSquare> search_square = agent.second;

            while (search_square != nullptr) {
                std::cout << search_square->position << " <= ";
                search_square = search_square->parent;
            }
            std::cout << std::endl;
        }
    }
}

const std::shared_ptr<SearchSquare> SimpleSequentialSolver::computeShortestPathPossible(const Agent &agent) {
    //TODO: wait action possible ??

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
        populateOpenList(open_list, closed_list, agent, current_search_square);

        if (open_list.empty() && current_search_square->position != agent.getGoalCoord()) {
            _status = NO_SOLUTION;
        }

        // We loop while we didn't detect that there is no solution or that we didn't reach the goal position of the agent
    } while (current_search_square->position != agent.getGoalCoord() && _status == OK);

    if (_status == NO_SOLUTION) {
        return nullptr;
    }

    recordStatesFromPath(agent_id, current_search_square, state_dictionary);

    return current_search_square;
}

void SimpleSequentialSolver::populateOpenList(MultimapSearchSquare &open_list, const std::set<std::string> &closed_list,
                                              const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_position) {

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
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, left_pos, WEST);
    } if (right) {
        Position right_pos = Position(x+1, y);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, right_pos, EAST);
    } if (up) {
        Position up_pos = Position(x, y+1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, up_pos, NORTH);
    } if (down) {
        Position down_pos = Position(x, y-1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, down_pos, SOUTH);
    } if (left && up) {
        Position left_up_pos = Position(x-1, y+1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, left_up_pos, NW);
    } if (right && up) {
        Position right_up_pos = Position(x+1, y+1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, right_up_pos, NE);
    } if (left && down) {
        Position left_down_pos = Position(x-1, y-1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, left_down_pos, SW);
    } if (right && down) {
        Position right_down_pos = Position(x+1, y-1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, right_down_pos, SE);
    }
}

void SimpleSequentialSolver::tryInsertInOpenList(MultimapSearchSquare &open_list, const std::set<std::string> &closed_list,
                                                 const Agent &agent,
                                                 std::shared_ptr<SearchSquare> &current_agent_position,
                                                 Position &analyzed_pos, const Direction &direction) {

    const int& time_step = current_agent_position->time_step;
    State *state_to_evaluate = getStateToEvaluateFromTimeStep(time_step);

    // If the position (left, right, up, down, top-right etc.) from our current search square is a walkable square
    if (!canAgentAccessPosition(agent, current_agent_position, analyzed_pos)) {
        return;
    }

    auto& it_agent_searched = state_to_evaluate->findAgentAtPosition(analyzed_pos);
    const bool isAgentThere = it_agent_searched != state_to_evaluate->getSearchSquares().end();

    // If there is no agent at the position or if the agent that is there is the agent we are currently processing
    if (!isAgentThere || it_agent_searched->first == agent.getId()) {

        // If the agent will collide with other agents if it does this move
        if (state_dictionary.getEdgeConflictWithOtherAgents(current_agent_position->position, analyzed_pos,
                                                            direction,
                                                            time_step,
                                                            state_dictionary.getStateFromTimeStep(time_step),
                                                            agent.getId()) != nullptr)  {
            return;
        }

        std::string pos_coord = std::to_string(analyzed_pos.x) + ";" + std::to_string(analyzed_pos.y);
        // We check that the position has not already been processed (i.e., not in the closed list)
        if (closed_list.find(pos_coord) != closed_list.end()) {
            // Already processed, we stop here for this position
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

State *SimpleSequentialSolver::getStateToEvaluateFromTimeStep(const int &time_step) {

    std::map<int, State> &states = state_dictionary.dictionary;
    // We select the state to evaluate from the current time step

    // If we have already recorded a state for this time step
    if (states.find(time_step + 1) != states.end()) {
        // We select the state recorded at this time step
        return &states[time_step + 1];
    } else {
        // Here, we have reached a further step (not yet defined)
        // if it is due to the fact that no state has been recorded
        if (states.empty()) {
            // We select the init state as the state to evaluate
            return &init_state;
        } else {
            // The state to evaluate is the last we have recorded because it means that the other agents
            // will not move while we finish to deal with the current agent
            return &states[states.size() - 1];
        }
    }
}

std::vector<int> SimpleSequentialSolver::getAgentsOrderWithGap(const int gap) {
    int max_id = map.getAgents().size();
    std::vector<int> result(max_id-gap);

    std::iota(result.begin(), result.end(), gap+1);

    for (int i = 1; i < gap+1; i++) {
        result.push_back(i);
    }
    return result;
}
