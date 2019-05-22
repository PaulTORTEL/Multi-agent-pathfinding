//
// Created by Paul Tortel on 22/05/2019.
//

#ifndef PATHFINDING_PROJECT_CONFLICTBASEDSEARCH_H
#define PATHFINDING_PROJECT_CONFLICTBASEDSEARCH_H


#include <set>
#include "../Solver.h"
#include "Constraint.h"
#include "ConstraintNode.h"

class ConflictBasedSearch : Solver {
private:
    ConstraintNode root;

    void highLevelSolver();

public:
    ConflictBasedSearch(Map &map);

    void solve() override;

    std::map<int, SearchSquare> lowLevelSolver(ConstraintNode &constraint_node, int agent_id = 0);

    SearchSquare computeShortestPathPossible(const Agent &agent, ConstraintNode &constraint_node);

    void populateOpenList(Multimap &open_list, const std::set<std::string> &closed_list, const Agent &agent,
                          std::shared_ptr<SearchSquare> &current_agent_position,
                          ConstraintNode &constraint_node);

    void tryInsertInOpenList(Multimap &open_list, const std::set<std::string> &closed_list,
                             const Agent &agent, std::shared_ptr<SearchSquare> &current_agent_position,
                             Position &analyzed_pos, ConstraintNode &constraint_node);
};


#endif //PATHFINDING_PROJECT_CONFLICTBASEDSEARCH_H
