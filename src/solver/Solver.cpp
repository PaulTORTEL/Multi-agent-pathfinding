//
// Created by Paul Tortel on 15/05/2019.
//

#include "../../include/solver/Solver.h"
#include <set>
#include <iostream>

Solver::Solver(Map &map) : map(map) {

    std::map<int, Agent> agents = map.getAgents();

    for (auto agent_id = 1; agent_id <= agents.size(); agent_id++) {
        const auto& agent_it = agents.find(agent_id);

        // If we didn't find the corresponding agent (with id = agent_id)
        if (agent_it == agents.end()) {
            setStatus(ERROR, "In Solver.cpp (constructor), unknown agent ID");
        }

        // We save the initial and goal positions of the agent
        init_state.positions[agent_id] = Position(agent_it->second.getStartCoord().x, agent_it->second.getStartCoord().y);
        goal_state.positions[agent_id] = Position(agent_it->second.getGoalCoord().x, agent_it->second.getGoalCoord().y);
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

bool Solver::checkStateValidity(const State &state, const std::vector<std::vector<Square>> &grid) {

    std::set<std::string> known_pos;

    for (auto& it : state.positions) {
        const int x = it.second.x;
        const int y = it.second.y;

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
