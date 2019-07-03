//
// Created by Paul Tortel on 13/05/2019.
//

#ifndef PATHFINDING_PROJECT_MAP_H
#define PATHFINDING_PROJECT_MAP_H

#include <utility>
#include <ostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "Agent.h"

/**
* Enumeration of the different types that can have a square
*/
enum SquareType {
    NONE,
    WALL,
    //SWAMP  => create another enum for ground type
    HUMAN_POINT
};

/**
* Structure gathering all the properties and information about a Square (a place in the environment)
*/
struct MapSquare {
    int id = 0;
    SquareType type = NONE;
    std::set<Direction> forbidden_exits;
    std::set<Direction> forbidden_accesses;
};

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

        std::map<int, Position> _products;
        std::map<int, Position> _drop_off_points;
        std::map<int, Position> _repair_points;

        /**
         * The actual world/map representation
         */
        std::vector<std::vector<MapSquare>> _grid;

        /**
         *  Associate a key with the corresponding SquareType value.
         *  Do not use it outside this class!
         */
        std::map<std::string, SquareType> __key_SquareType_map;

        /**
         *  Associate a key with the corresponding Direction value.
         *  Do not use it outside this class!
         */
        std::map<std::string, Direction > __key_Direction_map;

public:
    Map(std::string name, int width = 0, int height = 0) : _name(name), _width(width), _height(height) {
        __key_SquareType_map["NONE"] = SquareType::NONE;
        __key_SquareType_map["WALL"] = SquareType::WALL;
        __key_SquareType_map["HUMAN_POINT"] = SquareType::HUMAN_POINT;

        __key_Direction_map["NORTH"] = Direction::NORTH;
        __key_Direction_map["SOUTH"] = Direction::SOUTH;
        __key_Direction_map["EAST"] = Direction::EAST;
        __key_Direction_map["WEST"] = Direction::WEST;

        __key_Direction_map["NE"] = Direction::NE;
        __key_Direction_map["NW"] = Direction::NW;
        __key_Direction_map["SW"] = Direction::SW;
        __key_Direction_map["SE"] = Direction::SE;
    }

    int getWidth() const;

    void setWidth(int width);

    int getHeight() const;

    void setHeight(int height);

    const std::vector<std::vector<MapSquare>>& getGrid() const;

    void setGrid(std::vector<std::vector<MapSquare>>& grid);

    friend std::ostream &operator<<(std::ostream &os, const Map &map) {
        os << "MAP [" << map._name << "] = Width: " << map._width << "; Height: " << map._height << "; Agents: " << map._agents.size() << std::endl;
        return os;
    }

    /**
     * Set the square's type from a given string
     * @param key : the square's type
     * @param x : the actual square
     * @return FALSE if the given type is unknown from the SquareType enum
     */
    bool setSquareTypeFromString(const char *key, const int &x, const int &y);

    const std::map<int, Agent>& getAgents() const;

    void addAgent(const Agent& agent);

    /**
     * Get a MapSquare from a given position
     * @param position : the position that we want to get
     * @return a const reference on the MapSquare
     */
    const MapSquare & getMapSquare(const Position &position) const;

    /**
     * Returns the extra movement cost from a position
     * @param position : the position
     * @return the extra cost
     */
    const int getExtraCostFromMapSquareType(const Position &position) const;

    const std::map<int, Position> getProducts() const;

    void setProducts(const std::map<int, Position> &products);

    const std::map<int, Position> getDropOffPoints() const;

    void setDropOffPoints(const std::map<int, Position> &dropOffPoints);

    const std::map<int, Position> getRepairPoints() const;

    void setRepairPoints(const std::map<int, Position> &repairPoints);

    void addProduct(const int x, const int y, const int id);

    void addDropOffPoint(const int x, const int y, const int id);

    void addRepairPoint(const int x, const int y, const int id);

    void removeItemToPickupForAgent(const int &agent_id);

    void setCurrentPositionForAgent(const int &agent_id, const Position &position);

    void addForbiddenAccess(const int x, const int y, const char *direction);

    void addForbiddenExit(const int x, const int y, const char *direction);

    PointOfInterest getInterestOfPosition(const Position& position) const;

    void setAgentInteractingTime(const int& agent_id, const int time_left);

    void setAgentStatus(const int& agent_id, const SearchSquare::AgentStatus agent_status);
};


#endif //PATHFINDING_PROJECT_MAP_H
