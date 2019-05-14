#include <iostream>
#include "../libs/xml-parser/tinyxml2.h"
#include "../include/XmlParser.h"
#include "../include/State.h"
#include <set>

struct lex_compare {
    bool operator() (const std::pair<int, Position>& left, const std::pair<int, Position>& right) const {
       return left.first < right.first;
    }
};


int main() {
    Map map = XmlParser::buildMapFromXml("../maps/map_1.xml", "map_1");
    std::cout << map << std::endl;

    State init_state;
    init_state.positions[1] = Position(map.getAgents()[1].getStartCoord().x, map.getAgents()[1].getStartCoord().y);
    init_state.positions[2] = Position(map.getAgents()[2].getStartCoord().x, map.getAgents()[2].getStartCoord().y);

    std::cout << "init state: " << init_state.positions[1].x << "," << init_state.positions[1].y << std::endl << std::endl;

    std::map<int, Position> path;

    path.insert(std::make_pair(3, Position(2,1)));
    path.insert(std::make_pair(1, Position(0,0)));
    path.insert(std::make_pair(4, Position(3,2)));
    path.insert(std::make_pair(2, Position(4,0)));

    for (auto& it : path) {
        std::cout << it.first << ": " << it.second.x << "," << it.second.y << std::endl;
    }

    //std::cout << path.find()

    return 0;
}