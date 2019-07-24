#include <iostream>
#include "../libs/xml-parser/tinyxml2.h"
#include "../include/XmlParser.h"
#include "../include/state/State.h"
#include "../include/solver/Solver.h"
#include "../include/solver/SimpleSequentialSolver.h"
#include "../include/solver/cbs/ConflictBasedSearch.h"


int main() {
    int map_no = 1;
    std::cout << "VERSION 3" << std::endl;
    std::cout << "Type the number of the map to load: ";

    std::cin >> map_no;

    std::map<int, State> final_solution;

    while (map_no != 0) {
        final_solution.clear();

        std::string map_to_load = "../maps/map_" + std::to_string(map_no) + ".xml";
        std::string map_name = "map_" + std::to_string(map_no);

        Map map = XmlParser::buildMapFromXml(map_to_load.c_str(), map_name.c_str());
        std::cout << map << std::endl;

        std::cout << "Type the number of the technique to use: ";
        int technique_no = 1;
        std::cin >> technique_no;

        if (technique_no == 1) {
            SimpleSequentialSolver solver(map, map.getAgents());
            solver.solve();
        } else if (technique_no == 2) {

            ConflictBasedSearch cbsSolver(map, map.getAgents());
            auto solution = cbsSolver.solve();

        }

        system("pause");

        std::cout << "Type the number of the map to load: ";

        std::cin >> map_no;
    }

    return 0;
}
