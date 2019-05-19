//
// Created by Paul Tortel on 15/05/2019.
//

#include <iostream>
#include "../../include/solver/SimpleSequentialSolver.h"


SimpleSequentialSolver::SimpleSequentialSolver(Map &map) : Solver(map) {}

const std::vector<State> &SimpleSequentialSolver::solve() {

    if (_status != OK) { return std::vector<State>(); }

    for (auto& agent_it : map.getAgents()) {

        computeShortestPathPossible(agent_it.second);

        if (_status == NO_SOLUTION) {
            std::cout << "NO SOLUTION FOR AGENT " << agent_it.first << std::endl;
            break;
        }
    }

    return std::vector<State>();
}

void SimpleSequentialSolver::computeShortestPathPossible(const Agent &agent) {
    //TODO: level ? type du square? occupée ou pas par un autre agent à ce moment
    //TODO: wait action possible ??

    const int& agent_id = agent.getId();
    Multimap open_list;
    std::set<std::string> closed_list;

    std::shared_ptr<SearchSquare> current_search_square = std::make_shared<SearchSquare>(init_state.positions[agent_id], nullptr);
    open_list.insert({current_search_square->cost(), current_search_square});

    do {
        const auto& it_open_list = open_list.begin();
        current_search_square = it_open_list->second;

        std::string pos_coord = std::to_string(current_search_square->position.x) + ";" + std::to_string(current_search_square->position.y);
        closed_list.insert(pos_coord);

        open_list.erase(it_open_list);

        std::map<int, State> &dictionary = state_dictionary.dictionary;

        if (current_search_square->time_step < dictionary.size()) {
            std::shared_ptr<State> new_current_state = std::make_shared<State>();
            *new_current_state = dictionary[current_search_square->time_step];

        } else if (dictionary.empty()) {
            // prendre init
        } else {
            std::shared_ptr<State> new_current_state = std::make_shared<State>();
            *new_current_state = dictionary[dictionary.size() - 1];
        }

        populateOpenList(open_list, closed_list, agent, current_search_square);

        if (open_list.empty()) {
            _status = NO_SOLUTION;
        }
    } while (current_search_square->position != agent.getGoalCoord() && _status == OK);


    SearchSquare* square = current_search_square.get();

    if (_status == 2) {
        return;
    }
    std::cout << "agent " << agent_id << ": ";
    do {
        state_dictionary.addOrUpdateState(init_state, square->time_step, agent_id, square->position);
        std::cout << square->position << " <= ";
        square = square->parent.get();
    } while (square != nullptr);
    std::cout << std::endl;
}

void SimpleSequentialSolver::populateOpenList(Multimap &open_list, const std::set<std::string> &closed_list,
                                              const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_position) {

    const int x = current_agent_position->position.x;
    const int y = current_agent_position->position.y;

    const int max_xy = map.getGrid().size() - 1;

    const bool left = x > 0;
    const bool right = x < max_xy;
    const bool up = y < max_xy;
    const bool down = y > 0;

    if (left) {
        Position left_pos = Position(x-1, y);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, left_pos);
    } if (right) {
        Position right_pos = Position(x+1, y);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, right_pos);
    } if (up) {
        Position up_pos = Position(x, y+1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, up_pos);
    } if (down) {
        Position down_pos = Position(x, y-1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, down_pos);
    } if (left && up) {
        Position left_up_pos = Position(x-1, y+1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, left_up_pos);
    } if (right && up) {
        Position right_up_pos = Position(x+1, y+1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, right_up_pos);
    } if (left && down) {
        Position left_down_pos = Position(x-1, y-1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, left_down_pos);
    } if (right && down) {
        Position right_down_pos = Position(x+1, y-1);
        tryInsertInOpenList(open_list, closed_list, agent, current_agent_position, right_down_pos);
    }
    std::cout << "shared count :"  << current_agent_position.use_count() << std::endl;
}

void SimpleSequentialSolver::tryInsertInOpenList(Multimap &open_list, const std::set<std::string> &closed_list,
                                                 const Agent &agent,
                                                 std::shared_ptr<SearchSquare> &current_agent_position,
                                                 Position &analyzed_pos) {

    std::map<int, State> &dictionary = state_dictionary.dictionary;
    const int& time_step = current_agent_position->time_step;
    State *state_to_evaluate = nullptr;

    if (time_step + 1 < dictionary.size()) {
        state_to_evaluate = &dictionary[time_step+1];
    } else {
        if (dictionary.empty()) {
            state_to_evaluate = &init_state;
        } else {
            state_to_evaluate = &dictionary[dictionary.size() - 1];
        }
    }

    //TODO: if water..., add it here
    //TODO: check if the agent and the square are on the same level or that the agent can climb/use "staircases"
    //TODO: if we do not allow agents to exchange their position (cross the same edge simultaneously in both direction, add to check if next state.pos = current agent pos
    if (map.getMapSquare(analyzed_pos).type != WALL) {
        if (state_to_evaluate->findAgentAtPosition(analyzed_pos) == state_to_evaluate->positions.end()) {

            std::string pos_coord = std::to_string(analyzed_pos.x) + ";" + std::to_string(analyzed_pos.y);
            if (closed_list.find(pos_coord) != closed_list.end()) {
                return;
            }

            const float move_cost = movement_cost(*current_agent_position, analyzed_pos);
            const float heuristic = heuristic_cost(analyzed_pos, agent.getGoalCoord());

            const float cost = move_cost + heuristic;

            const auto& it = findPositionInOpenList(analyzed_pos, open_list);

            if (it != open_list.end()) {
                if (it->second->cost() > cost) {
                    it->second->cost_movement = move_cost;
                    it->second->parent = current_agent_position->parent;
                }
            } else {
                std::shared_ptr<SearchSquare> new_search_square = std::make_shared<SearchSquare>(analyzed_pos, current_agent_position, move_cost, heuristic);
                open_list.insert({cost, new_search_square});
            }
        }
    }
}

SimpleSequentialSolver::Multimap::iterator SimpleSequentialSolver::findPositionInOpenList(
        const Position &pos,
        Multimap &open_list) {

    for (auto it = open_list.begin(); it != open_list.end(); ++it) {
        if (it->second->position == pos) {
            return it;
        }
    }
    return open_list.end();
}
