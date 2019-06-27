#include <iostream>
#include "../libs/xml-parser/tinyxml2.h"
#include "../include/XmlParser.h"
#include "../include/state/State.h"
#include "../include/solver/Solver.h"
#include "../include/solver/SimpleSequentialSolver.h"
#include "../include/solver/cbs/ConflictBasedSearch.h"


//TODO: temp
void scanSolution(Map& map, std::map<int, State>& solution, Solver& solver) {

    std::map<int, std::pair<Position, Position>> moves;
    bool stop = false;

    for (auto & state : solution) {

        for (auto& agent : state.second.getSearchSquares()) {

            // set the current position of the agent bc we will recompute paths very soon
            map.setCurrentPositionForAgent(agent.first, agent.second->position);

            if (moves.find(agent.first) == moves.end()) {
                moves[agent.first].second = agent.second->position;
                continue;
            } else {
                moves[agent.first].first = moves[agent.first].second;
                moves[agent.first].second = agent.second->position;
            }

            if (moves[agent.first].first != moves[agent.first].second &&
                        agent.second->position == solver.getAgentGoalPosition(map.getAgents().at(agent.first))) {
                stop = true;
                map.removeItemToPickupForAgent(agent.first);
            }
        }

        if (stop) {
            return;
        }
    }
}

bool shouldContinue(Map& map) {

    for (auto& agent : map.getAgents()) {
        if (agent.second.getCurrentPosition() != agent.second.getParkingCoord()) {
            return true;
        }
    }
    return false;
}

int main() {
    int map_no = 1;
    std::cout << "Type the number of the map to load: ";

    std::cin >> map_no;

    while (map_no != 0) {

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
            do {
                ConflictBasedSearch cbsSolver(map, map.getAgents());
                auto solution = cbsSolver.solve();
                scanSolution(map, solution, cbsSolver);
            } while (shouldContinue(map));
        }

        system("pause");

        std::cout << "Type the number of the map to load: ";

        std::cin >> map_no;
    }

    return 0;
}
