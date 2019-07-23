#include <iostream>
#include "../libs/xml-parser/tinyxml2.h"
#include "../include/XmlParser.h"
#include "../include/state/State.h"
#include "../include/solver/Solver.h"
#include "../include/solver/SimpleSequentialSolver.h"
#include "../include/solver/cbs/ConflictBasedSearch.h"

void recordState(std::map<int, State>& final_solution, State& state) {

    if (final_solution.empty()) {
        final_solution[0] = state;
    } else {
        int next_index = final_solution.rbegin()->first + 1;
        final_solution[next_index] = state;
    }
}

//TODO: temp
void scanSolution(Map& map, std::map<int, State>& solution, Solver& solver, std::map<int, State>& final_solution) {

    std::map<int, std::pair<Position, Position>> moves;
    bool stop = false;

    for (auto & state : solution) {
        int num_agents_parked = 0;

        for (auto& agent : state.second.getSearchSquares()) {

            // set the current position of the agent bc we will recompute paths very soon
            map.setCurrentPositionForAgent(agent.first, agent.second->position);
            map.setAgentInteractingTime(agent.first, agent.second->interacting_time_left);
            map.setAgentStatus(agent.first, agent.second->agent_status);

            if (moves.find(agent.first) == moves.end()) {
                moves[agent.first].second = agent.second->position;
                continue;
            } else {
                moves[agent.first].first = moves[agent.first].second;
                moves[agent.first].second = agent.second->position;
            }

            const Agent &agent_ref = map.getAgents().at(agent.first);

            if (agent.second->position == solver.getAgentGoalPosition(agent_ref)) {
                if (agent.second->position == agent_ref.getParkingCoord()) {
                    num_agents_parked++;
                }
                
               // if (moves[agent.first].first != moves[agent.first].second) {
                    if (agent.second->isAgentInteracting() && agent.second->interacting_time_left == 1) {
                        stop = true;
                        map.removeItemToPickupForAgent(agent.first);
                    }
             //   }
            }
        }

        if (stop) {
            if (num_agents_parked == map.getAgents().size()) {
                recordState(final_solution, state.second);
            }
            return;
        }

        recordState(final_solution, state.second);
    }
}

bool shouldContinue(Map &map, Solver::Status &status) {

    if (status == Solver::NO_SOLUTION) {
        return false;
    }

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
        StatsManager statsManager;

        if (technique_no == 1) {
            SimpleSequentialSolver solver(map, map.getAgents(), statsManager);
            solver.solve();
        } else if (technique_no == 2) {
            ConflictBasedSearch::Status status = Solver::OK;
            do {
                ConflictBasedSearch cbsSolver(map, map.getAgents(), statsManager);
                auto solution = cbsSolver.solve();
                status = cbsSolver.getStatus();
                scanSolution(map, solution, cbsSolver, final_solution);
            } while (shouldContinue(map, status));

            if (status != Solver::NO_SOLUTION) {

                statsManager.recordStatsOnTxt(map_name, final_solution.rbegin()->first);

                std::cout << "FINAL SOLUTION: " << std::endl;
                for (auto& it : final_solution) {
                    std::cout << "T" << it.first << ": ";

                    for (auto &it_search_square : it.second.getSearchSquares()) {
                        std::cout << "A" << it_search_square.first << " = " << it_search_square.second->position << "; ";
                    }
                    std::cout << std::endl;
                }
            }
        }

        system("pause");

        std::cout << "Type the number of the map to load: ";

        std::cin >> map_no;
    }

    return 0;
}
