//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONFLICTBASEDSEARCH_H
#define PATHFINDING_PROJECT_CONFLICTBASEDSEARCH_H


#include "../Solver.h"
#include "Constraint.h"
#include "ConstraintNode.h"
#include "../../stats/StatsManager.h"

class ConflictBasedSearch : public Solver {
private:

    enum InsertionOpenListResult {
        FAIL_ENVIRONMENT,
        FAIL_CONSTRAINT,
        FAIL_CLOSED_LIST,
        OK
    };



    /**
     * Performs the high level of the CBS algorithm
     */
    std::map<int, State> highLevelSolver();

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
    std::shared_ptr<SearchSquare> computeShortestPathPossible(Agent &agent,
                                                              ConstraintNode &constraint_node);

    /**
     * Populates the open list
     * @param open_list : the open list
     * @param closed_list : the closed list
     * @param agent : the agent we are dealing with
     * @param current_agent_position : the position of the agent
     * @param constraint_node : the constraint node we are using currently
     */
    void populateOpenList(MultimapSearchSquare &open_list, std::set<std::string> &closed_list, const Agent &agent,
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
     * @return the result of the insertion attempt
     */
    InsertionOpenListResult tryInsertInOpenList(MultimapSearchSquare &open_list,
                                                std::set<std::string> &closed_list,
                                                const Agent &agent,
                                                std::shared_ptr<SearchSquare> &current_agent_position,
                                                Position &analyzed_pos, ConstraintNode &constraint_node);

    /**
     * Returns an iterator on the constraint node having the less constraints among the less expensive nodes
     * @param open_list : the open list
     * @return an iterator
     */
    Solver::MultimapConstraintNode::iterator getIteratorOnLessConflictNode(MultimapConstraintNode &open_list);

    /**
     * Returns an iterator on the search square having the less conflicts with the current solution
     * @param open_list : the open list
     * @param constraint_node : the constraint node
     * @return an iterator
     */
    static Solver::MultimapSearchSquare::iterator getIteratorOnStateWithLessConflict(Solver::MultimapSearchSquare &open_list, ConstraintNode& constraint_node);

    /**
     * Returns whether the constraint node is already added in the open list or not
     * @param open_list : the open list
     * @param constraint_node : the constraint node
     * @return a boolean, true if the node is already added
     */
    static bool isNodeAlreadyInOpenList(const Solver::MultimapConstraintNode &open_list,
                                        ConstraintNode &constraint_node);

public:

    ConflictBasedSearch(Map &map, const std::map<int, Agent> &agents, StatsManager &statsManager);

    std::map<int, State> solve() override;

    bool tryForceMovementForAgent(std::shared_ptr<SearchSquare> &current_agent_position, const Agent &agent,
                                  ConstraintNode &constraint_node);

    void setStatsManager(StatsManager &manager);
};


#endif //PATHFINDING_PROJECT_CONFLICTBASEDSEARCH_H
