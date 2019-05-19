//
// Created by Paul Tortel on 15/05/2019.
//

#ifndef PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
#define PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H


#include "Solver.h"
#include <set>

class SimpleSequentialSolver : Solver {

private:

    // Cost - Position
    typedef std::multimap<float, SearchSquare> Multimap;

    void computeShortestPathPossible(const Agent &agent);

    void populateOpenList(Multimap &open_list, const std::set<std::string> &closed_list,
                          const Agent &agent, SearchSquare current_agent_position);

    static SimpleSequentialSolver::Multimap::iterator findPositionInOpenList(const Position &pos, Multimap &open_list);


public:

    SimpleSequentialSolver(Map &map);

    const std::vector<State> &solve() override;

    void tryInsertInOpenList(Multimap &open_list, const std::set<std::string> &closed_list,
                             const Agent &agent, SearchSquare &current_agent_position,
                             Position &analyzed_pos);
};


#endif //PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
