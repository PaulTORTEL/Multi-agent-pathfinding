//
// Created by Paul Tortel on 13/05/2019.
//

#include <utility>

#include "../include/Map.h"

int Map::getWidth() const {
    return _width;
}

void Map::setWidth(int width) {
    Map::_width = width;
}

int Map::getHeight() const {
    return _height;
}

void Map::setHeight(int height) {
    Map::_height = height;
}

const std::vector<std::vector<MapSquare>>& Map::getGrid() const {
    return _grid;
}

void Map::setGrid(std::vector<std::vector<MapSquare>>& grid) {
    _grid = grid;
}

bool Map::setSquareTypeFromString(const char *key, const int &x, const int &y) {
    // We get an iterator on the value from the key
    auto it = __key_SquareType_map.find(key);

    // True if the type has not been found
    if (it == __key_SquareType_map.end()) {
        return false;
    }

    // We set up the type for the square
    _grid[x][y].type = it->second;
    return true;
}

const std::map<int, Agent>& Map::getAgents() const {
    return _agents;
}

void Map::addAgent(const Agent &agent) {
    _agents[agent.getId()] = agent;
}

const MapSquare & Map::getMapSquare(const Position &position) const {
    return _grid[position.x][position.y];
}

const int Map::getExtraCostFromMapSquareType(const Position &position) const {
    const MapSquare& map_square = getMapSquare(position);

    //TODO: remove extra cost for agents able to move efficiently in specific areas

    int extra_cost = 0;

    switch (map_square.type) {

        case NONE:break;
        case WALL:break;
        case SWAMP:
            extra_cost = 2;
            break;
    }

    return extra_cost;
}
