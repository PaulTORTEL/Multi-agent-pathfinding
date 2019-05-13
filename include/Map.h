//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_MAP_H
#define PATHFINDING_PROJECT_MAP_H

#include <utility>
#include <ostream>
#include <vector>
#include <map>
#include <string>
#include "Agent.h"

/**
 * Enumeration of the different types that can have a square
 */
typedef enum {
    NONE,
    WALL,
    SWAMP
} SquareType;

/**
 * Structure gathering all the properties and information about a Square (a place in the environment)
 */
typedef struct {
    SquareType type = NONE;
    /**
     * Indicates the level of the square (if 0, the square is at sea-level)
     */
    int level = 0;
} Square;

/**
 * Map class gathering all the necessary information and the _grid itself
 */
class Map {
private:
    std::string _name;
    int _width;
    int _height;

    /**
     * The agents in the world
     */
    std::map<int, Agent> _agents;

    /**
     * The actual world/map representation
     */
    std::vector<std::vector<Square>> _grid;

    /**
     *  Associate a key with the corresponding SquareType value.
     *  Do not use it outside this class!
     */
    std::map<std::string, SquareType> __key_SquareType_map;

public:
    Map(std::string name, int width = 0, int height = 0) : _name(name), _width(width), _height(height) {
        __key_SquareType_map["NONE"] = SquareType::NONE;
        __key_SquareType_map["WALL"] = SquareType::WALL;
        __key_SquareType_map["SWAMP"] = SquareType::SWAMP;
    }

    int getWidth() const;

    void setWidth(int width);

    int getHeight() const;

    void setHeight(int height);

    std::vector<std::vector<Square>>& getGrid();

    void setGrid(std::vector<std::vector<Square>>& grid);

    friend std::ostream &operator<<(std::ostream &os, const Map &map) {
        os << "MAP [" << map._name << "] = Width: " << map._width << "; Height: " << map._height << "; Agents: " << map._agents.size() << std::endl;
        return os;
    }

    /**
     * Set the square's type from a given string
     * @param key : the square's type
     * @param square : the actual square
     * @return FALSE if the given type is unknown from the SquareType enum
     */
    bool setSquareTypeFromString(const char* key, Square& square);

    std::map<int, Agent> &getAgents();
};


#endif //PATHFINDING_PROJECT_MAP_H
