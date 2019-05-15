#include <iostream>
#include "../libs/xml-parser/tinyxml2.h"
#include "../include/XmlParser.h"
#include "../include/State.h"
#include "../include/solver/Solver.h"

int main() {
    Map map = XmlParser::buildMapFromXml("../maps/map_1.xml", "map_1");
    std::cout << map << std::endl;

    Solver solver(map);


    State s;
    s.positions[1] = Position(0,0);
    s.positions[2] = Position(1,0);

    State s2;
    s2.positions[1] = Position(0,0);
    s2.positions[2] = Position(1,1);

    std::cout << "Res: " << std::to_string(s != s2) << std::endl;

    std::cout << solver.heuristic(Position(10,54), Position(103,7)) << std::endl;

    return 0;
}