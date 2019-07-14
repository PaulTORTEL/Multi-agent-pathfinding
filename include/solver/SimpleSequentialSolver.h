//
// Created by Paul Tortel on 15/05/2019.
//

#ifndef PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
#define PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H

#include "Solver.h"

class SimpleSequentialSolver : Solver {

private:

    /**
     * Compute the shortest path possible for a given agent
     * @param agent : the agent
     */
    const std::shared_ptr<SearchSquare> computeShortestPathPossible(const Agent &agent);

    /**
     * Populate the open list
     * @param open_list : the open list
     * @param closed_list : the closed list
     * @param agent : the agent we are dealing with
     * @param current_agent_position : the currently analyzed position for the agent (last element taken from the open list)
     */
    void populateOpenList(MultimapSearchSquare &open_list, const std::set<std::string> &closed_list,
                          const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_position);


    /**
     * Try to insert an element in the open list depending on the agents coordinates at the corresponding time step, the environment topology (walls...) etc.
     * @param open_list : the open list
     * @param closed_list : the closed list
     * @param agent : the currently processed agent
     * @param current_agent_position : the currently analyzed position for the agent
     * @param analyzed_pos : the new position that we can reach from the currently analyzed position
     */
    void tryInsertInOpenList(MultimapSearchSquare &open_list, const std::set<std::string> &closed_list,
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
     * Returns a vector of agent IDs, putting at the front the ID found with the gap
     * @param gap : the number of IDs that we ignore
     * @return a vector of agents ID
     */
    std::vector<int> getAgentsOrderWithGap(const int gap);

public:

    SimpleSequentialSolver(Map &map, const std::map<int, Agent> &agents, StatsManager &manager);

    std::map<int, State> solve() override;


    /**
    * The dictionary gathering all the known states in the problem solving
    */
    StateDictionary state_dictionary;
};


#endif //PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
