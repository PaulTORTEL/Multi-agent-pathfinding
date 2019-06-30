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

    bool isNeighbourWith(const Position& rhs) const {

        if (*this == rhs) {
            return false;
        }

        int diff_x = abs(this->x - rhs.x);
        int diff_y = abs(this->y - rhs.y);

        return diff_x <= 1 && diff_y <= 1;
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

/**
* WARNING: ONLY CALL THIS FUNCTION WITH ADJACENT POSITIONS (pos1 & pos2)
* Return whether the directions of the movements would result to an edge collision
* @param start_pos1 : the starting position 1
* @param end_pos1 : the ending position 1
* @param start_pos2 : the starting position 2
* @param end_pos2 : the ending position 2
* @return true if the movements should collide
*/
const bool inline areMovementsEdgeColliding(const Position &start_pos1, const Position &end_pos1,
                                            const Position &start_pos2, const Position &end_pos2) {

    if (!start_pos1.isNeighbourWith(start_pos2)) {
        return false;
    }

    // Pos1 & pos2 are adjacent, we are only interested in determining whether their movements will result to an edge collision
    Direction d1 = extractDirection(start_pos1, end_pos1);
    Direction d2 = extractDirection(start_pos2, end_pos2);

    // Which side the agent 2 is on from the agent 1
    // If the agent 2 is at the left hand side of agent 1, side = WEST
    Direction side_agent_2 = extractDirection(start_pos1, start_pos2);

    if (d1 == NW && d2 == NE && side_agent_2 == WEST) {
        return true;
    } else if (d1 == NE && d2 == NW && side_agent_2 == EAST) {
        return true;
    } else if (d1 == SW && d2 == SE && side_agent_2 == WEST) {
        return true;
    } else if (d1 == SE && d2 == SW && side_agent_2 == EAST) {
        return true;
    } else if (d1 == NE && d2 == SW  && side_agent_2 == NE) {
        return true;
    } else if (d1 == NW && d2 == SE && side_agent_2 == NW) {
        return true;
    } else if (d1 == SW && d2 == NE && side_agent_2 == SW) {
        return true;
    } else if (d1 == SE && d2 == NW && side_agent_2 == SE) {
        return true;
    } else if (d1 == WEST && d2 == EAST && side_agent_2 == WEST) {
        return true;
    } else if (d1 == EAST && d2 == WEST && side_agent_2 == EAST) {
        return true;
    } else if (d1 == NORTH && d2 == SOUTH && side_agent_2 == NORTH) {
        return true;
    } else if (d1 == SOUTH && d2 == NORTH && side_agent_2 == SOUTH) {
        return true;
    }

    return false;
}


enum PointOfInterest {
    NA,
    PRODUCT,
    REPAIR_POINT,
    DROP_OFF_POINT
};

#endif //PATHFINDING_PROJECT_UTILITY_H
