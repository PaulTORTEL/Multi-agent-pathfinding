//
// Created by Paul Tortel on 15/05/2019.
//

#ifndef PATHFINDING_PROJECT_SOLVER_H
#define PATHFINDING_PROJECT_SOLVER_H


#include "../State.h"
#include "../Map.h"
#include "../StateDictionary.h"
#include "../SearchSquare.h"
#include <vector>

class Solver {

private:

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

    /**
     * The Chebyshev distance is used instead of the Manhattan distance since we allow diagonal moves
     * @param p1 : a position
     * @param p2 : another position
     * @return the value of the Chebyshev distance
     */
    static float chebyshevDistance(const Position &p1, const Position &p2);

protected:

    /**
     * Possible status of a solver
     */
    enum Status {
        OK,
        ERROR,
        NO_SOLUTION
    };

    Status _status = OK;

    State init_state;

    State goal_state;

    /**
     * The dictionary gathering all the known states in the problem solving
     */
    StateDictionary state_dictionary;

    /**
     * Keep a const. ref. of the map it is working on
     */
    const Map map;

    /**
     * Return the total movement cost (heuristic + move)
     * @param current : the current search square
     * @param next : the position that we are computing
     * @param goal : the position of the agent's goal
     * @return the total movement cost
     */
    const float total_movement_cost(const SearchSquare &current, const Position &next, const Position &goal);

    /**
     * Return the movement cost
     * @param current : the current search square
     * @param next : the position that we are computing
     * @return the movement cost
     */
    virtual float movement_cost(const SearchSquare &current, const Position &next);

    /**
     * Returns the value of the heuristic function
     * @param current : the current agent's position
     * @param goal : the agent's goal
     * @return the computed value of the estimated distance between the current position and the goal position
     */
    static float heuristic_cost(const Position &current, const Position &goal);

public:
    Solver(Map &map);

    /**
     * Check if a state is valid or not
     * @param state : the state to evaluate
     * @param grid : the world
     * @return True if the state is valid
     */
    static bool checkStateValidity(const State &state, const std::vector<std::vector<MapSquare>> &grid);

    /**
     * Set the status of the solver
     * @param status : the status to be set
     * @param msg : the optional message
     */
    void setStatus(Status status, const std::string& msg);

    /**
     * Virtual pure function that child classes have to override in order to start solving the problem
     * @return
     */
    virtual void solve() = 0;
};


#endif //PATHFINDING_PROJECT_SOLVER_H
