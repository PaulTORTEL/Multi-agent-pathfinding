//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_UTILITY_H
#define PATHFINDING_PROJECT_UTILITY_H

struct Position {
    int x, y;

    explicit Position(int x = 0, int y = 0) {
        this->x = x;
        this->y = y;
    }

    bool operator==(const Position &rhs) const {
        return x == rhs.x &&
               y == rhs.y;
    }

    bool operator!=(const Position &rhs) const {
        return !(rhs == *this);
    }
};

#endif //PATHFINDING_PROJECT_UTILITY_H
