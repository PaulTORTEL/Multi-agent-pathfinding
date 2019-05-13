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

std::vector<std::vector<Square>>& Map::getGrid() {
    return _grid;
}

void Map::setGrid(std::vector<std::vector<Square>>& grid) {
    _grid = grid;
}

bool Map::setSquareTypeFromString(const char *key, Square &square) {
    // We get an iterator on the value from the key
    auto it = __key_SquareType_map.find(key);

    // True if the type has not been found
    if (it == __key_SquareType_map.end()) {
        return false;
    }

    // We set up the type for the square
    square.type = it->second;
    return true;
}

std::map<int, Agent> &Map::getAgents() {
    return _agents;
}
