//
// Created by Paul Tortel on 15/05/2019.
//

#include <iostream>
#include "../../include/solver/SimpleSequentialSolver.h"
#include <queue>

SimpleSequentialSolver::SimpleSequentialSolver(Map &map) : Solver(map) {}

const std::vector<State> &SimpleSequentialSolver::solve() {

    //TODO: data structure for keeping track of the agents paths? or can we just update the state 0, then 1, then 2 for the agent (1 by 1)?

    //solution.push_back(init_state);
    State current_state = init_state;

    for (auto& agent_it : map.getAgents()) {
        if (_status == OK && current_state != goal_state) {
            computeShortestPathPossible(current_state, agent_it.second);
        }
    }

    return solution;
}

void SimpleSequentialSolver::computeShortestPathPossible(State& current_state, const Agent &agent) {
    const int& agent_id = agent.getId();
    //TODO: level ? type du square? occupée ou pas par un autre agent à ce moment
    //TODO: wait action possible ??

    Multimap open_list;
    std::queue<Position> closed_list;

    const Position* current_agent_position = &(current_state.positions[agent_id]);

    open_list.insert(Multimap::value_type(0, *current_agent_position));

    populateOpenList(open_list, current_state, agent, *current_agent_position);
}

void SimpleSequentialSolver::populateOpenList(Multimap &open_list, State &current_state, const Agent &agent,
                                              const Position &current_agent_position) {

    const int& x = current_agent_position.x;
    const int& y = current_agent_position.y;

    const int& max_xy = map.getGrid().size() - 1;

    const bool left = x > 0;
    const bool right = x < max_xy;
    const bool up = y < max_xy;
    const bool down = y > 0;

    if (left) {
        Position left_pos = Position(x-1, y);
        tryInsertInOpenList(open_list, current_state, agent, current_agent_position, left_pos);
    } if (right) {
        Position right_pos = Position(x+1, y);
        tryInsertInOpenList(open_list, current_state, agent, current_agent_position, right_pos);
    } if (up) {
        Position up_pos = Position(x, y+1);
        tryInsertInOpenList(open_list, current_state, agent, current_agent_position, up_pos);
    } if (down) {
        Position down_pos = Position(x, y-1);
        tryInsertInOpenList(open_list, current_state, agent, current_agent_position, down_pos);
    } if (left && up) {
        Position left_up_pos = Position(x-1, y+1);
        tryInsertInOpenList(open_list, current_state, agent, current_agent_position, left_up_pos);
    } if (right && up) {
        Position right_up_pos = Position(x+1, y+1);
        tryInsertInOpenList(open_list, current_state, agent, current_agent_position, right_up_pos);
    } if (left && down) {
        Position left_down_pos = Position(x-1, y-1);
        tryInsertInOpenList(open_list, current_state, agent, current_agent_position, left_down_pos);
    } if (right && down) {
        Position right_down_pos = Position(x+1, y-1);
        tryInsertInOpenList(open_list, current_state, agent, current_agent_position, right_down_pos);
    }
}

void SimpleSequentialSolver::tryInsertInOpenList(Multimap &open_list, State &current_state, const Agent &agent,
                                                 const Position &current_agent_position, const Position &analyzed_pos) {
    //TODO: if water..., add it here
    //TODO: check if the agent and the square are on the same level or that the agent can climb/use "staircases"
    if (map.getGrid()[analyzed_pos.x][analyzed_pos.y].type != WALL) {
        if (current_state.findAgentAtPosition(analyzed_pos) == current_state.positions.end()) {
            const float cost = total_movement_cost(current_agent_position, analyzed_pos, agent.getGoalCoord());
            open_list.insert(Multimap::value_type(cost, analyzed_pos));
        }
    }
}
