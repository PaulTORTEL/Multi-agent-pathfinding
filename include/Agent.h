//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_AGENT_H
#define PATHFINDING_PROJECT_AGENT_H

#include "utility.h"

class Agent {
private:
    int id;

    //speed ? special skills (e.g., climbing?)


    Position start_coord;
    Position goal_coord;

public:

    Agent(int id, int start_x, int start_y, int goal_x, int goal_y) : id(id) {
        start_coord = Position(start_x, start_y);
        goal_coord = Position(goal_x, goal_y);
    }

    Agent() {}

    int getId() const {
        return id;
    }

    const Position &getStartCoord() const {
        return start_coord;
    }

    const Position &getGoalCoord() const {
        return goal_coord;
    }

};


#endif //PATHFINDING_PROJECT_AGENT_H
