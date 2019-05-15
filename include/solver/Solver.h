//
// Created by Paul Tortel on 15/05/2019.
//

#ifndef PATHFINDING_PROJECT_SOLVER_H
#define PATHFINDING_PROJECT_SOLVER_H


#include "../State.h"
#include "../Map.h"
#include <vector>

class Solver {

private:

    /**
     * Possible status of a solver
     */
    enum Status {
        OK,
        ERROR,
        NO_SOLUTION,
    };

    /**
     * Exception used to explain why the solver' status is now set on ERROR
     */
    struct SolverException : public std::exception
    {
        std::string msg;

        SolverException(const std::string msg) : msg(msg) {}

        const char * what () const throw ()
        {
            return msg.c_str();
        }
    };

    Status _status = OK;

    State init_state;
    State goal_state;

    /**
     * Store the paths that each agent must take to reach its goal
     */
    std::vector<State> solution;

    /**
     * Keep a const. ref. of the map it is working on
     */
    const Map map;


public:
    Solver(Map &map);

    /**
     * Check if a state is valid or not
     * @param state : the state to evaluate
     * @param grid : the world
     * @return True if the state is valid
     */
    static bool checkStateValidity(const State &state, const std::vector<std::vector<Square>> &grid);

    /**
     * Set the status of the solver
     * @param status : the status to be set
     * @param msg : the optional message
     */
    void setStatus(Status status, const std::string& msg);
};


#endif //PATHFINDING_PROJECT_SOLVER_H
