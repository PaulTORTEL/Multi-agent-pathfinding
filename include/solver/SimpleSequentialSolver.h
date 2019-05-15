//
// Created by Paul Tortel on 15/05/2019.
//

#ifndef PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
#define PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H


#include "Solver.h"

class SimpleSequentialSolver : Solver {

private:

public:
    SimpleSequentialSolver(Map &map);

    const std::vector<State> &solve() override;
};


#endif //PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
