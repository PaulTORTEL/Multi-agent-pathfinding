//
// Created by Paul Tortel on 15/05/2019.
//

#ifndef PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
#define PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H


#include "Solver.h"
#include <set>

class SimpleSequentialSolver : Solver {

private:

    /**
     * Compute the shortest path possible for a given agent
     * @param agent : the agent
     */
    void computeShortestPathPossible(const Agent &agent);

    /**
     * Populate the open list
     * @param open_list : the open list
     * @param closed_list : the closed list
     * @param agent : the agent we are dealing with
     * @param current_agent_position : the currently analyzed position for the agent (last element taken from the open list)
     */
    void populateOpenList(Multimap &open_list, const std::set<std::string> &closed_list,
                          const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_position);


    /**
     * Try to insert an element in the open list depending on the agents coordinates at the corresponding time step, the environment topology (walls...) etc.
     * @param open_list : the open list
     * @param closed_list : the closed list
     * @param agent : the currently processed agent
     * @param current_agent_position : the currently analyzed position for the agent
     * @param analyzed_pos : the new position that we can reach from the currently analyzed position
     */
    void tryInsertInOpenList(Multimap &open_list, const std::set<std::string> &closed_list,
                             const Agent &agent,
                             std::shared_ptr<SearchSquare> &current_agent_position,
                             Position &analyzed_pos, const Direction &direction);

    /**
     * Return the state to evaluate from the current time step
     * @param time_step : the time step
     * @return a pointer to the state
     */
    State *getStateToEvaluateFromTimeStep(const int &time_step);

    /**
     * Record the states in the state dictionary from the path we have just computed
     * @param agent_id : the agent id
     * @param current_search_square : the final search square embedding the goal position
     */
    void recordStatesFromPath(const int &agent_id, const std::shared_ptr<SearchSquare> &current_search_square);

    /**
     * Return whether the agent will collide with other agents if it moves in a given direction
     * @param current_position : the current agent position
     * @param next_position : the next agent position that we are evaluating
     * @param direction : the direction such movement would do
     * @param time_step : the current time step
     * @return true if a collision with another agent is detected
     */
    const bool willCollideWithOtherAgents(const Position &current_position, Position &next_position,
                                          const Direction &direction,
                                          const int &time_step);

    /**
     * Return whether the agent is colliding with a neighbour agent or not
     * @param current_position : the current agent position
     * @param next_position : the next agent position that we are evaluating
     * @param time_step : the current time step
     * @param current_state : the current state
     * @param current_position_neighbour : the current position of the neighbour agent
     * @return true if a collision is detected
     */
    const bool
    isCollidingWithNeighbour(const Position &current_position, const Position &next_position,
                             const int &time_step, State &current_state,
                             const Position &current_position_neighbour);

public:

    SimpleSequentialSolver(Map &map);

    void solve() override;
};


#endif //PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
