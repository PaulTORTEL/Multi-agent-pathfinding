//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONFLICTBASEDSEARCH_H
#define PATHFINDING_PROJECT_CONFLICTBASEDSEARCH_H


#include "../Solver.h"
#include "Constraint.h"
#include "ConstraintNode.h"

class ConflictBasedSearch : Solver {
private:
    /**
     * The root constraint node, no constraints and a first solution
     */
    ConstraintNode root;

    /**
     * Performs the high level of the CBS algorithm
     */
    void highLevelSolver();

    /**
     * Performs the low level of the CBS algorithm
     * @param constraint_node : the constraint node we have to use to find a new solution
     * @param agent_id : the agent id (optional)
     * @return a state dictionary gathering the solution computed
     */
    StateDictionary lowLevelSolver(ConstraintNode &constraint_node, int agent_id = 0);

    /**
     * Computes the shortest path possible for the given agent, according to the constraint node we are dealing with
     * @param agent : the agent
     * @param constraint_node : the current constraint node
     * @return a shared pointer on the final search square, having a link to its parents
     */
    std::shared_ptr<SearchSquare> computeShortestPathPossible(const Agent &agent, ConstraintNode &constraint_node);

    /**
     * Populates the open list
     * @param open_list : the open list
     * @param closed_list : the closed list
     * @param agent : the agent we are dealing with
     * @param current_agent_position : the position of the agent
     * @param constraint_node : the constraint node we are using currently
     */
    void populateOpenList(MultimapSearchSquare &open_list, const std::set<std::string> &closed_list, const Agent &agent,
                          std::shared_ptr<SearchSquare> &current_agent_position,
                          ConstraintNode &constraint_node);

    /**
     * Try to insert new search square in the open list
     * @param open_list : the open list
     * @param closed_list : the closed list
     * @param agent : the agent
     * @param current_agent_position : the current position of the agent
     * @param analyzed_pos : the position where the agent could be at the next time step
     * @param constraint_node : the constraint node we are using currently
     */
    void tryInsertInOpenList(MultimapSearchSquare &open_list, const std::set<std::string> &closed_list,
                             const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_position,
                             Position &analyzed_pos, ConstraintNode &constraint_node);

    /**
     * Returns an iterator on the constraint node having the less constraints among the less expensive nodes
     * @param open_list : the open list
     * @return an iterator
     */
    static Solver::MultimapConstraintNode::iterator getIteratorOnLessConstrainedNode(MultimapConstraintNode &open_list);

    /**
     * Returns whether the constraint node is already added in the open list or not
     * @param open_list : the open list
     * @param constraint_node : the constraint node
     * @return a boolean, true if the node is already added
     */
    static bool isNodeAlreadyInOpenList(const Solver::MultimapConstraintNode& open_list, const ConstraintNode& constraint_node);

public:

    ConflictBasedSearch(Map &map);

    void solve() override;
};


#endif //PATHFINDING_PROJECT_CONFLICTBASEDSEARCH_H
