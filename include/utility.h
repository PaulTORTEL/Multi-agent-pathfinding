//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_UTILITY_H
#define PATHFINDING_PROJECT_UTILITY_H

#include <ostream>

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

    friend std::ostream &operator<<(std::ostream &os, const Position &position) {
        os << "Pos[x:" << position.x << " y:" << position.y << "]";
        return os;
    }
};

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,

    NE,
    NW,
    SE,
    SW,

    NO_DIRECTION
};

Direction inline extractDirection(const Position& p1, const Position& p2) {
    int diff_x = p1.x - p2.x;
    int diff_y = p1.y - p2.y;

    if (diff_x < 0 && diff_y < 0) {
        return NE;
    } else if (diff_x < 0 && diff_y == 0) {
        return EAST;
    } else if (diff_x < 0 && diff_y > 0) {
        return SE;
    } else if (diff_x == 0 && diff_y < 0) {
        return NORTH;
    } else if (diff_x == 0 && diff_y == 0) {
        return NO_DIRECTION;
    } else if (diff_x == 0 && diff_y > 0) {
        return SOUTH;
    } else if (diff_x > 0 && diff_y < 0) {
        return NW;
    } else if (diff_x > 0 && diff_y == 0) {
        return WEST;
    } else if (diff_x > 0 && diff_y > 0) {
        return SW;
    }

    return NO_DIRECTION;
}
#endif //PATHFINDING_PROJECT_UTILITY_H
