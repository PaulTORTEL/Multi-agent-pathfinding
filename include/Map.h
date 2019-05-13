//
// Created by Paul on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_MAP_H
#define PATHFINDING_PROJECT_MAP_H

#include <utility>
#include <ostream>
#include "Agent.h"

// Enumeration of the different types that can have a square
enum {
    NONE,
    WALL,
    SWAMP
} typedef SquareType;

// Structure gathering all the properties and information about a Square (a place in the environment)
struct {
    SquareType type;
    // Indicates the level of the square (if 0, the square is at sea-level)
    int level;
    std::pair<int , int> coord;
    // A pointer to the agent currently occupying the square
    Agent* agent = nullptr; //TODO: smart pointer? (weak)
} typedef Square;

// Map class gathering all the necessary information and the _grid itself
class Map {
private:
    int _width;
    int _height;
    int _population;

    SquareType** _grid; //TODO: smart pointer? (strong)

public:
    Map(int width = 0, int height = 0, int population = 0) : _width(width), _height(height), _population(population) {}

    int getWidth() const;

    void setWidth(int width);

    int getHeight() const;

    void setHeight(int height);

    int getPopulation() const;

    void setPopulation(int population);

    friend std::ostream &operator<<(std::ostream &os, const Map &map) {
        os << "Width: " << map._width << "; Height: " << map._height << "; Population: " << map._population;
        return os;
    }
};


#endif //PATHFINDING_PROJECT_MAP_H
