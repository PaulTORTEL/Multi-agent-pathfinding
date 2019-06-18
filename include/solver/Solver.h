//
// Created by Paul Tortel on 15/05/2019.
//

#ifndef PATHFINDING_PROJECT_SOLVER_H
#define PATHFINDING_PROJECT_SOLVER_H

#include "../State.h"
#include "../Map.h"
#include "../StateDictionary.h"
#include "../SearchSquare.h"
#include "cbs/ConstraintNode.h"


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

    static int manhattanDistance(const Position &p1, const Position &p2);

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
     * Keeps a const. ref. of the map it is working on
     */
    const Map map;

    // Cost - SearchSquare (Position embedded)
    typedef std::multimap<int, std::shared_ptr<SearchSquare>> MultimapSearchSquare;
    // Cost - Constraint node (solution embedded)
    typedef std::multimap<int, ConstraintNode> MultimapConstraintNode;


    /**
     * Returns the movement cost
     * @param current : the current search square
     * @param next : the position that we are computing
     * @return the movement cost
     */
    virtual int movementCost(const SearchSquare &current, const Position &next, const int &agent_id);

    /**
     * Returns the value of the heuristic function
     * @param current : the current agent's position
     * @param goal : the agent's goal
     * @return the computed value of the estimated distance between the current position and the goal position
     */
    static int heuristicCost(const Position &current, const Position &goal);

    /**
    * Finds a position in the open list
    * @param pos : the position we look for
    * @param open_list : the open list
    * @return an iterator on the position
    */
    static Solver::MultimapSearchSquare::iterator findPositionInOpenList(const Position &pos, MultimapSearchSquare &open_list);

    /**
    * Records the states in the state dictionary from the path we have just computed
    * @param agent_id : the agent id
    * @param current_search_square : the final search square embedding the goal position
    */
    void recordStatesFromPath(const int &agent_id, const std::shared_ptr<SearchSquare> &current_search_square,
                              StateDictionary &state_dictionary);

    /**
     * Indicates whether the agent can access to the analyzed position or not
     * @param agent : the agent moving
     * @param current_agent_pos : its current position
     * @param analyzed_pos : the analyzed position, where the agent wants to go
     * @return true if the agent can move to this position
     */
    bool canAgentAccessPosition(const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_pos,
                                Position &analyzed_pos);

public:

    Solver(Map &map);

    /**
     * Checks if a state is valid or not
     * @param state : the state to evaluate
     * @param grid : the world
     * @return True if the state is valid
     */
    static bool checkStateValidity(const State &state, const std::vector<std::vector<MapSquare>> &grid);

    /**
     * Sets the status of the solver
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
