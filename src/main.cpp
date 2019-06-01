#include <iostream>
#include "../libs/xml-parser/tinyxml2.h"
#include "../include/XmlParser.h"
#include "../include/State.h"
#include "../include/solver/Solver.h"
#include "../include/solver/SimpleSequentialSolver.h"
#include "../include/solver/cbs/ConflictBasedSearch.h"

int main() {
    std::cout << "Type the number of the map to load: ";
    int map_no = 1;
    std::cin >> map_no;
    std::string map_to_load = "../maps/map_" + std::to_string(map_no) + ".xml";
    std::string map_name = "map_" + std::to_string(map_no);

    Map map = XmlParser::buildMapFromXml(map_to_load.c_str(), map_name.c_str());
    std::cout << map << std::endl;

   // SimpleSequentialSolver solver(map);
  //  solver.solve();

    ConflictBasedSearch cbsSolver(map);
    cbsSolver.solve();

    return 0;
}