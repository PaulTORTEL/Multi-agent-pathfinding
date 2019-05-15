//
// Created by Paul Tortel on 15/05/2019.
//

#include <iostream>
#include "../../include/solver/SimpleSequentialSolver.h"

SimpleSequentialSolver::SimpleSequentialSolver(Map &map) : Solver(map) {}

const std::vector<State> &SimpleSequentialSolver::solve() {

    //TODO: data structure for keeping track of the agents paths? or can we just update the state 0, then 1, then 2 for the agent (1 by 1)?

    std::cout << "solv: " << heuristic(Position(0,0), Position(5,6)) << std::endl;
    return solution;
}
