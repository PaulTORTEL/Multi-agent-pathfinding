#include <iostream>
#include "../libs/xml-parser/tinyxml2.h"
#include "../include/XmlParser.h"
#include "../include/State.h"
#include "../include/solver/Solver.h"
#include "../include/solver/SimpleSequentialSolver.h"
#include "../include/solver/cbs/ConflictBasedSearch.h"

int main() {
    Map map = XmlParser::buildMapFromXml("../maps/map_1.xml", "map_1");
    std::cout << map << std::endl;

   // SimpleSequentialSolver solver(map);
    //solver.solve();

    ConflictBasedSearch cbsSolver(map);
    cbsSolver.solve();

    return 0;
}