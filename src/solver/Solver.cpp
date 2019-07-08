//
// Created by Paul Tortel on 15/05/2019.
//

#include "../../include/solver/SimpleSequentialSolver.h"
#include "../../include/solver/Solver.h"
#include <set>
#include <iostream>
#include <vector>
#include <algorithm>

Solver::Solver(Map &map, const std::map<int, Agent>& agents) : map(map), agents(agents) {

    for (auto& agent : agents) {

        // We save the initial and goal positions of the agent
        auto init_state_search_square = std::make_unique<SearchSquare>(agent.second.getCurrentPosition(), nullptr);
        init_state.setSearchSquareForAgent(agent.first, std::move(init_state_search_square));
    }

    bool validity = true;
    std::string err;

    // We check if the init state is valid (no agents overlapped, or at incorrect positions such as squares with walls)
    if (!checkStateValidity(init_state, map.getGrid())) {
        validity = false;
        err = "[init_state]";
    }

    // We check if the goal state is valid (no agents overlapped, or at incorrect positions such as squares with
  /*  if (!checkStateValidity(goal_state, map.getGrid())) {
        validity = false;
        err += "[goal_state]";
    }*/

    // We throw an exception by setting the status of the solver to ERROR and we specify the error message
    if (!validity) {
        setStatus(ERROR, err);
    }
}

//TODO: to remove?
Position Solver::getAgentStartPosition(const Agent& agent) {
    int items_to_pickup = agent.getItemsToPickup();
    int size_of_list = agent.getPickupList().size();
    if (items_to_pickup == size_of_list)  {
        return agent.getParkingCoord();
    }

    int product_id = agent.getPickupList()[size_of_list - (items_to_pickup+1)];
    return map.getProducts().at(product_id);
}

Position Solver::getAgentGoalPosition(const Agent& agent) {
    int items_to_pickup = agent.getItemsToPickup();
    int size_of_list = agent.getPickupList().size();

    if (items_to_pickup > 0) {
        int product_id = agent.getPickupList()[size_of_list - items_to_pickup];
        return map.getProducts().at(product_id);
    }

    if (agent.hasFinished()) {
        return agent.getParkingCoord();
    }

    const auto& drop_off_points = map.getDropOffPoints();

    const int& drop_off_1_heuristic = heuristicCost(agent.getCurrentPosition(), drop_off_points.at(1));
    const int& drop_off_2_heuristic = heuristicCost(agent.getCurrentPosition(), drop_off_points.at(2));

    if (drop_off_1_heuristic <= drop_off_2_heuristic) {
        return drop_off_points.at(1);
    } else {
        return drop_off_points.at(2);
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

int Solver::computeTotalHeuristicCost(Position current, Agent agent) {
    int total_heuristic_cost = 0;
    Position goal;
    do {
        goal = getAgentGoalPosition(agent);
        agent.removeItemToPickup();
        total_heuristic_cost += heuristicCost(current, goal);
        current = goal;
    } while (goal != agent.getParkingCoord());

    return total_heuristic_cost;
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

    switch (move_direction) {

        case NORTH: case SOUTH: case WEST: case EAST:
            return straight + current.cost_movement + extra_cost;

        case NO_DIRECTION:
            return straight + current.cost_movement;

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

    const MapSquare& analyzed_map_square = map.getMapSquare(analyzed_pos);
    const MapSquare& current_map_square = map.getMapSquare(current_agent_pos->position);

    // If the position is a non-walkable square
    switch (analyzed_map_square.type) {
        case NONE:break;
        case WALL:
            return false;
        case HUMAN_POINT:
            return false;
    }

    Direction direction_out = extractDirection(current_agent_pos->position, analyzed_pos);
    Direction direction_in = extractDirection(analyzed_pos, current_agent_pos->position);

    if (analyzed_map_square.forbidden_accesses.find(direction_in) != analyzed_map_square.forbidden_accesses.end()) {
        return false;
    }

    if (current_map_square.forbidden_exits.find(direction_out) != current_map_square.forbidden_exits.end()) {
        return false;
    }

    return true;
}

const std::map<int, Position> & Solver::getAgentsGoal() const {
    return agents_goal;
}



