//
// Created by Paul Tortel on 15/05/2019.
//

#ifndef PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
#define PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H


#include "Solver.h"
#include <set>

class SimpleSequentialSolver : Solver {

private:

    // Cost - SearchSquare (Position embedded)
    typedef std::multimap<float, std::shared_ptr<SearchSquare>> Multimap;

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
     * Find a position in the open list
     * @param pos : the position we look for
     * @param open_list : the open list
     * @return an iterator on the position
     */
    static SimpleSequentialSolver::Multimap::iterator findPositionInOpenList(const Position &pos, Multimap &open_list);


public:

    SimpleSequentialSolver(Map &map);

    void solve() override;

    /**
     * Try to insert an element in the open list depending on the agents coordinates at the corresponding time step, the environment topology (walls...) etc.
     * @param open_list : the open list
     * @param closed_list : the closed list
     * @param agent : the currently processed agent
     * @param current_agent_position : the currently analyzed position for the agent
     * @param analyzed_pos : the new position that we can reach from the currently analyzed position
     */
    void tryInsertInOpenList(Multimap &open_list, const std::set<std::string> &closed_list,
                             const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_position,
                             Position &analyzed_pos);
};


#endif //PATHFINDING_PROJECT_SIMPLESEQUENTIALSOLVER_H
