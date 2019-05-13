//
// Created by Paul on 13/05/2019.
//

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

int Map::getPopulation() const {
    return _population;
}

void Map::setPopulation(int population) {
    Map::_population = population;
}
