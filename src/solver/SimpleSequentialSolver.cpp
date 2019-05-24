//
// Created by Paul Tortel on 15/05/2019.
//

#include <iostream>
#include "../../include/solver/SimpleSequentialSolver.h"


SimpleSequentialSolver::SimpleSequentialSolver(Map &map) : Solver(map) {}

void SimpleSequentialSolver::solve() {

    if (_status != OK) { return; }

    // We loop through all the agents
    for (auto& agent_it : map.getAgents()) {

        computeShortestPathPossible(agent_it.second);
        // If there is no solution for one agent, we stop
        if (_status == NO_SOLUTION) {
            std::cout << "NO SOLUTION FOR AGENT " << agent_it.first << std::endl;
            break;
        }
    }
}

void SimpleSequentialSolver::computeShortestPathPossible(const Agent &agent) {
    //TODO: level
    //TODO: wait action possible ??

    const int& agent_id = agent.getId();
    MultimapSearchSquare open_list;
    std::set<std::string> closed_list;

    // We populate the open list with the initial search square, wrapping the initial position of the agent
    std::shared_ptr<SearchSquare> current_search_square = std::make_shared<SearchSquare>(init_state.positions[agent_id], nullptr);
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

        if (open_list.empty()) {
            _status = NO_SOLUTION;
        }
        // We loop while we didn't detect that there is no solution or that we didn't reach the goal position of the agent
    } while (current_search_square->position != agent.getGoalCoord() && _status == OK);

    if (_status == NO_SOLUTION) {
        return;
    }

    recordStatesFromPath(agent_id, current_search_square);
}

void SimpleSequentialSolver::recordStatesFromPath(const int &agent_id,
                                                  const std::shared_ptr<SearchSquare> &current_search_square) {
    // We get the search square wrapping the goal position and the total cost
    std::shared_ptr<SearchSquare> square = current_search_square;

    std::cout << "agent " << agent_id << ": ";
    do {
        // We set up the states according to the position of the agent along its path towards its goal
        state_dictionary.addOrUpdateState(init_state, square->time_step, agent_id, square->position);
        std::cout << square->position << " <= ";
        square = square->parent;
    } while (square != nullptr);
    std::cout << std::endl;
}

void SimpleSequentialSolver::populateOpenList(MultimapSearchSquare &open_list, const std::set<std::string> &closed_list,
                                              const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_position) {

    const int x = current_agent_position->position.x;
    const int y = current_agent_position->position.y;

    const int max_xy = map.getGrid().size() - 1;

    // There is a position at the left
    const bool left = x > 0;
    // There is a position at the right
    const bool right = x < max_xy;
    // There is a position at the top
    const bool up = y < max_xy;
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

    //TODO: if water..., add it here
    //TODO: check if the agent and the square are on the same level or that the agent can climb/use "staircases"

    // If the position (left, right, up, down, top-right etc.) from our current search square is a walkable square
    if (map.getMapSquare(analyzed_pos).type != WALL) {
        auto& it_agent_searched = state_to_evaluate->findAgentAtPosition(analyzed_pos);
        const bool isAgentThere = it_agent_searched != state_to_evaluate->positions.end();

        // If there is no agent at the position or if the agent that is there is the agent we are currently processing
        if (!isAgentThere || it_agent_searched->first == agent.getId()) {

            // If the agent will collide with other agents if it does this move
            if (willCollideWithOtherAgents(current_agent_position->position, analyzed_pos, direction, time_step)) {
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
}

const bool SimpleSequentialSolver::willCollideWithOtherAgents(const Position &current_position, Position &next_position,
                                                              const Direction &direction, const int &time_step) {

    State* current_state = state_dictionary.getStateFromTimeStep(time_step);
    if (current_state == nullptr) {
        return false;
    }

    Position pos_right = Position(current_position.x+1, current_position.y);
    Position pos_up = Position(current_position.x, current_position.y+1);
    Position pos_left = Position(current_position.x-1, current_position.y);
    Position pos_down = Position(current_position.x, current_position.y-1);
    Position pos_down_left = Position(current_position.x-1, current_position.y-1);
    Position pos_down_right = Position(current_position.x+1, current_position.y-1);
    Position pos_up_left = Position(current_position.x-1, current_position.y+1);
    Position pos_up_right = Position(current_position.x+1, current_position.y+1);

    switch(direction) {

        case NORTH:break;
        case SOUTH:break;
        case EAST:break;
        case WEST:break;
        case NO_DIRECTION:break;

        case NE:
            // In the case our current agent plans to go north-east, the only positions that could have other agents and that could result to a collision
            // are the position at the right, top right, and top
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_right)) {
                return true;
            }
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_up)) {
                return true;
            }
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_up_right)) {
                return true;
            }
            break;

        case NW:
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_left)) {
                return true;
            }
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_up)) {
                return true;
            }
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_up_left)) {
                return true;
            }
            break;
        case SE:
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_right)) {
                return true;
            }
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_down)) {
                return true;
            }
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_down_right)) {
                return true;
            }
            break;
        case SW:
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_left)) {
                return true;
            }
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_down)) {
                return true;
            }
            if (isCollidingWithNeighbour(current_position, next_position, time_step, *current_state, pos_down_left)) {
                return true;
            }
            break;
    }

    return false;
}

const bool
SimpleSequentialSolver::isCollidingWithNeighbour(const Position &current_position, const Position &next_position,
                                                 const int &time_step, State &current_state,
                                                 const Position &current_position_neighbour) {

    // We check if there is an agent at the position that could lead to a collision with our agent
    // if this agent moves in a particular direction
    auto &it_agent = current_state.findAgentAtPosition(current_position_neighbour);

    if (it_agent != current_state.positions.end()) {
        // We fetch the next state to be able to see where this agent will go at the next time step
        State* next_state = state_dictionary.getStateFromTimeStep(time_step + 1);

        // There is no next state yet, then it means that the other agent will not move at this point
        if (next_state == nullptr) {
            // There is no collision
            return false;
        }

        // If the movements will result in a collision
        if (areMovementsColliding(current_position, next_position, it_agent->second,
                                  next_state->positions[it_agent->first])) {
            return true;
        }
    }
    // There is no agent
    return false;
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
