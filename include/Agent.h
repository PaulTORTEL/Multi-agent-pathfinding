//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_AGENT_H
#define PATHFINDING_PROJECT_AGENT_H


#include <utility>

class Agent {
private:
    int id;

    //speed ? special skills (e.g., climbing?)
    std::pair<int, int> start_coord;

    std::pair<int, int> goal_coord;
public:

    Agent(int id, int start_x, int start_y, int goal_x, int goal_y) : id(id) {
        start_coord = std::make_pair(start_x, start_y);
        goal_coord = std::make_pair(goal_x, goal_y);
    }

    Agent() {}

    int getId() const {
        return id;
    }

    const std::pair<int, int> &getStartCoord() const;

    const std::pair<int, int> &getGoalCoord() const;
};


#endif //PATHFINDING_PROJECT_AGENT_H
