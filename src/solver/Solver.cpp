//
// Created by Paul Tortel on 15/05/2019.
//

#include "../../include/solver/SimpleSequentialSolver.h"
#include "../../include/solver/Solver.h"
#include <set>
#include <iostream>
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
        init_state.search_squares[agent_id] = std::move(init_state_search_square);
        goal_state.search_squares[agent_id] = std::move(goal_state_search_square);
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

    for (auto& it : state.search_squares) {
        const int x = it.second->position.x;
        const int y = it.second->position.y;

        std::string key = std::to_string(x) + ";" + std::to_string(y);

        // If the position has already been evaluated, it means that an agent is already occupying it
        if (known_pos.find(key) == known_pos.end()) {
            // We insert the position because it has just been evaluated
            known_pos.insert(key);

            if (x < grid.size() && y < grid.size()) {
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

float Solver::heuristic_cost(const Position &current, const Position &goal) {
    return chebyshevDistance(current, goal);
}

float Solver::chebyshevDistance(const Position &p1, const Position &p2) {

    int dist_x = abs(p1.x - p2.x);
    int dist_y = abs(p1.y - p2.y);

    return std::max(dist_x, dist_y);
}

float Solver::movement_cost(const SearchSquare &current, const Position &next) {

    // We get the direction (North, South, etc.)
    Direction move_direction = extractDirection(current.position, next);
    const float straight = 1.;
    const float diagonal = 1.414;

    switch (move_direction) {

        case NORTH: case SOUTH: case WEST: case EAST:
            return straight + current.cost_movement;

        case NE: case NW: case SE: case SW:
            return diagonal + current.cost_movement;

        case NO_DIRECTION:
            return 0.;

        default:
            return 0.;
    }
}

const float Solver::total_movement_cost(const SearchSquare &current, const Position &next, const Position &goal) {
    return movement_cost(current, next) + heuristic_cost(next, goal);
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

void Solver::recordStatesFromPath(const int &agent_id,
                                                  const std::shared_ptr<SearchSquare> &current_search_square) {
    // We get the search square wrapping the goal position and the total cost
    std::shared_ptr<SearchSquare> square = current_search_square;

    std::cout << "agent " << agent_id << ": ";
    while (square != nullptr) {
        // We set up the states according to the position of the agent along its path towards its goal
        state_dictionary.addOrUpdateState(init_state, square->time_step, agent_id, square);
        std::cout << square->position << " <= ";
        square = square->parent;
    }
    std::cout << std::endl;
}

