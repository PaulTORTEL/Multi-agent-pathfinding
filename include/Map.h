//
// Created by Paul on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_MAP_H
#define PATHFINDING_PROJECT_MAP_H

#include <utility>
#include <ostream>
#include <vector>
#include <map>
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
    /**
     * A pointer to the agent currently occupying the square
     */
    Agent* agent = nullptr; //TODO: smart pointer? (weak)
} Square;

/**
 * Map class gathering all the necessary information and the _grid itself
 */
class Map {
private:
    int _width;
    int _height;

    /**
     * The number of agents in the world
     */
    int _population;

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
    Map(int width = 0, int height = 0, int population = 0) : _width(width), _height(height), _population(population) {
        __key_SquareType_map["NONE"] = SquareType::NONE;
        __key_SquareType_map["WALL"] = SquareType::WALL;
        __key_SquareType_map["SWAMP"] = SquareType::SWAMP;
    }

    int getWidth() const;

    void setWidth(int width);

    int getHeight() const;

    void setHeight(int height);

    int getPopulation() const;

    void setPopulation(int population);

    std::vector<std::vector<Square>>& getGrid();

    void setGrid(std::vector<std::vector<Square>>& grid);

    friend std::ostream &operator<<(std::ostream &os, const Map &map) {
        os << "Width: " << map._width << "; Height: " << map._height << "; Population: " << map._population << std::endl;
        return os;
    }

    /**
     * Set the square's type from a given string
     * @param key : the square's type
     * @param square : the actual square
     * @return FALSE if the given type is unknown from the SquareType enum
     */
    bool setSquareTypeFromString(const char* key, Square& square);
};


#endif //PATHFINDING_PROJECT_MAP_H
