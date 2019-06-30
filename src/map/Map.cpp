//
// Created by Paul Tortel on 13/05/2019.
//

#include <utility>

#include "../../include/map/Map.h"

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
        /*case SWAMP:
            extra_cost = 2;
            break;*/
        case HUMAN_POINT:break;
    }

    return extra_cost;
}

const std::map<int, Position> Map::getProducts() const {
    return _products;
}

void Map::setProducts(const std::map<int, Position> &products) {
    _products = products;
}

const std::map<int, Position> Map::getDropOffPoints() const {
    return _drop_off_points;
}

void Map::setDropOffPoints(const std::map<int, Position> &dropOffPoints) {
    _drop_off_points = dropOffPoints;
}

const std::map<int, Position> Map::getRepairPoints() const {
    return _repair_points;
}

void Map::setRepairPoints(const std::map<int, Position> &repairPoints) {
    _repair_points = repairPoints;
}

void Map::addProduct(const int x, const int y, const int id) {
    _products[id] = Position(x, y);
}

void Map::addDropOffPoint(const int x, const int y, const int id) {
    _drop_off_points[id] = Position(x, y);
}

void Map::addRepairPoint(const int x, const int y, const int id) {
    _repair_points[id] = Position(x, y);
}

void Map::removeItemToPickupForAgent(const int& agent_id) {
    _agents[agent_id].removeItemToPickup();
}

void Map::setCurrentPositionForAgent(const int& agent_id, const Position& position) {
    _agents[agent_id].setCurrentPosition(position);
}

void Map::addForbiddenAccess(const int x, const int y, const char *direction) {
    // We get an iterator on the value from the key
    auto it = __key_Direction_map.find(direction);

    // True if the type has not been found
    if (it == __key_Direction_map.end()) {
        return;
    }
    _grid[x][y].forbidden_accesses.insert(it->second);
}

void Map::addForbiddenExit(const int x, const int y, const char *direction) {
    // We get an iterator on the value from the key
    auto it = __key_Direction_map.find(direction);

    // True if the type has not been found
    if (it == __key_Direction_map.end()) {
        return;
    }

    _grid[x][y].forbidden_exits.insert(it->second);
}

PointOfInterest Map::getInterestOfPosition(const Position &position) const {

    for (auto& product : _products) {
        if (product.second == position) {
            return PRODUCT;
        }
    }

    for (auto& drop_off_point : _drop_off_points) {
        if (drop_off_point.second == position) {
            return DROP_OFF_POINT;
        }
    }

    for (auto& repair_point : _repair_points) {
        if (repair_point.second == position) {
            return REPAIR_POINT;
        }
    }

    return NA;
}


