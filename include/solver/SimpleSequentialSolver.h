//
// Created by Paul Tortel on 15/05/2019.
//

#ifndef PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
#define PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H


#include "Solver.h"

class SimpleSequentialSolver : Solver {

private:

    // Cost - Position
    typedef std::multimap<float, Position> Multimap;

    void computeShortestPathPossible(State& current_state, const Agent &agent);

    void populateOpenList(Multimap &open_list, State &current_state, const Agent &agent,
                          const Position &current_agent_position);

public:

    SimpleSequentialSolver(Map &map);

    const std::vector<State> &solve() override;

    void tryInsertInOpenList(Multimap &open_list, State &current_state, const Agent &agent,
                             const Position &current_agent_position, const Position &analyzed_pos);
};


#endif //PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
