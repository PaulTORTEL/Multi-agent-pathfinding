//
// Created by Paul Tortel on 15/05/2019.
//

#include "../../include/solver/SimpleSequentialSolver.h"
#include "../../include/solver/Solver.h"
#include <set>
#include <iostream>
#include <vector>
#include <algorithm>

Solver::Solver(Map &map) : map(map) {

    std::map<int, Agent> agents = map.getAgents();

    for (auto agent_id = 1; agent_id <= agents.size(); agent_id++) {
        const auto& agent_it = agents.find(agent_id);

        // If we didn't find the corresponding agent (with id = agent_id)
        if (agent_it == agents.end()) {
            setStatus(ERROR, "In Solver.cpp (constructor), unknown agent ID");
        }

        // We save the initial and goal positions of the agent
        auto init_state_search_square = std::make_unique<SearchSquare>(Position(agent_it->second.getStartCoord().x, agent_it->second.getStartCoord().y),
                                                                       nullptr);
        auto goal_state_search_square = std::make_unique<SearchSquare>(Position(agent_it->second.getGoalCoord().x, agent_it->second.getGoalCoord().y),
                                                                       nullptr);
        init_state.setSearchSquareForAgent(agent_id, std::move(init_state_search_square));
        goal_state.setSearchSquareForAgent(agent_id, std::move(goal_state_search_square));
    }

    bool validity = true;
    std::string err;

    // We check if the init state is valid (no agents overlapped, or at incorrect positions such as squares with walls)
    if (!checkStateValidity(init_state, map.getGrid())) {
        validity = false;
        err = "[init_state]";
    }

    // We check if the goal state is valid (no agents overlapped, or at incorrect positions such as squares with
    if (!checkStateValidity(goal_state, map.getGrid())) {
        validity = false;
        err += "[goal_state]";
    }

    // We throw an exception by setting the status of the solver to ERROR and we specify the error message
    if (!validity) {
        setStatus(ERROR, err);
    }
}

bool Solver::checkStateValidity(const State &state, const std::vector<std::vector<MapSquare>> &grid) {

    std::set<std::string> known_pos;

    for (auto& it : state.getSearchSquares()) {
        const int x = it.second->position.x;
        const int y = it.second->position.y;

        std::string key = std::to_string(x) + ";" + std::to_string(y);

        // If the position has already been evaluated, it means that an agent is already occupying it
        if (known_pos.find(key) == known_pos.end()) {
            // We insert the position because it has just been evaluated
            known_pos.insert(key);

            if (x < grid.size() && y < grid[0].size()) {
                // If the square type is WALL
                if (grid[x][y].type == WALL) {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    return true;
}

void Solver::setStatus(Status status, const std::string& msg) {
    Solver::_status = status;

    if (Solver::_status == ERROR) {
        std::string error = "A Solver Error has occured (" + msg + ")";
        throw SolverException(error);
    }
}

int Solver::heuristicCost(const Position &current, const Position &goal) {
    //return chebyshevDistance(current, goal);
    return manhattanDistance(current, goal);
}

float Solver::chebyshevDistance(const Position &p1, const Position &p2) {

    int dist_x = abs(p1.x - p2.x);
    int dist_y = abs(p1.y - p2.y);

    return std::max(dist_x, dist_y);
}

int Solver::manhattanDistance(const Position &p1, const Position &p2) {
    return abs(p2.x - p1.x) + abs(p2.y - p1.y);
}


int Solver::movementCost(const SearchSquare &current, const Position &next, const int &agent_id) {

    // We get the direction (North, South, etc.)
    Direction move_direction = extractDirection(current.position, next);
    const int extra_cost = map.getExtraCostFromMapSquareType(current.position);

    const int straight = 1;
    const float diagonal = 1.414;
    const int wait = getWaitCost(agent_id, current.position);

    switch (move_direction) {

        case NORTH: case SOUTH: case WEST: case EAST:
            return straight + current.cost_movement + extra_cost;

      /*  case NE: case NW: case SE: case SW:
            return diagonal + current.cost_movement + extra_cost;*/

        case NO_DIRECTION:
            return wait + current.cost_movement;

        default:
            return 0;
    }
}

Solver::MultimapSearchSquare::iterator Solver::findPositionInOpenList(
        const Position &pos,
        MultimapSearchSquare &open_list) {

    for (auto it = open_list.begin(); it != open_list.end(); ++it) {
        if (it->second->position == pos) {
            return it;
        }
    }
    return open_list.end();
}

void Solver::recordStatesFromPath(const int &agent_id, const std::shared_ptr<SearchSquare> &current_search_square,
                                  StateDictionary &state_dictionary) {
    // We get the search square wrapping the goal position and the total cost
    std::shared_ptr<SearchSquare> square = current_search_square;

    if (square != nullptr) {
        state_dictionary.setAgentPositionFromTimeStep(square->time_step, agent_id, square);
    }

    while (square != nullptr) {
        // We set up the states according to the position of the agent along its path towards its goal
        state_dictionary.addOrUpdateState(init_state, square->time_step, agent_id, square);
        square = square->parent;
    }
}

bool
Solver::canAgentAccessPosition(const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_pos,
                               Position &analyzed_pos) {

    //TODO: skills of the agent to test here
    //TODO: if water..., add it here
    //TODO: check if the agent can climb

    const MapSquare& analyzed_map_square = map.getMapSquare(analyzed_pos);

    // If the position is a non-walkable square
    if (analyzed_map_square.type == WALL) {
        return false;
    }

    const MapSquare& current_map_square = map.getMapSquare(current_agent_pos->position);

    // If both square are on the same level, the agent can move to this position
    if (current_map_square.level == analyzed_map_square.level) {
        return true;
    }

    Direction direction;

    // We have to check if the high ground square has stairs
    if (current_map_square.level > analyzed_map_square.level) {
        direction = extractDirection(current_agent_pos->position, analyzed_pos);
        return !(current_map_square.stairs.find(direction) == current_map_square.stairs.end());
    } else {
        direction = extractDirection(analyzed_pos, current_agent_pos->position);
        return !(analyzed_map_square.stairs.find(direction) == analyzed_map_square.stairs.end());
    }
}

const int Solver::getWaitCost(const int &agent_id, const Position &current_pos) const {
    return map.getAgents().at(agent_id).getGoalCoord() == current_pos ? 0 : 1;
}




