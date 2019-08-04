//
// Created by Paul Tortel on 18/06/2019.
//

#include <vector>
#include <iostream>
#include "../../include/stats/StatsManager.h"
#include <stdio.h>
#include <windows.h>
#include <fstream>
#include <math.h>

void StatsManager::startClock(std::clock_t &clock) {
    clock = std::clock();
}

double StatsManager::stopClock(std::clock_t &clock) {
    return ( std::clock() - clock ) / (double) CLOCKS_PER_SEC;
}

void StatsManager::startAnalysis(const int num_of_agents) {
    reports.emplace_back(AnalysisReport());
    getCurrentAnalysis().num_agents_involved = num_of_agents;
    startMeasuringDuration(HIGH_LEVEL);
}

void StatsManager::startMeasuringDuration(AnalysisLevel analysisLevel) {
    switch (analysisLevel) {

        case LOW_LEVEL:
            getCurrentAnalysis().low_level_calls++;
            startClock(lowLevelClock);
            break;
        case HIGH_LEVEL:
            getCurrentAnalysis().high_level_calls++;
            startClock(highLevelClock);
            break;
    }
}

void StatsManager::stopAnalysis() {
    stopMeasuringDuration(HIGH_LEVEL);

    std::cout << std::endl << getCurrentAnalysis() << std::endl;
}

void StatsManager::stopMeasuringDuration(AnalysisLevel analysisLevel) {

    double elapsedTime;
    double avg_of_level;
    int level_calls;

    switch (analysisLevel) {
        case LOW_LEVEL:
            elapsedTime = stopClock(lowLevelClock);
            avg_of_level = getCurrentAnalysis().avg_low_level_duration;
            level_calls = getCurrentAnalysis().low_level_calls;
            getCurrentAnalysis().avg_low_level_duration = (avg_of_level + elapsedTime) / level_calls;
            break;
        case HIGH_LEVEL:
            elapsedTime = stopClock(highLevelClock);
            avg_of_level = getCurrentAnalysis().avg_high_level_duration;
            level_calls = getCurrentAnalysis().high_level_calls;
            getCurrentAnalysis().avg_high_level_duration = (avg_of_level + elapsedTime) / level_calls;
            break;
    }
}

AnalysisReport& StatsManager::getCurrentAnalysis() {
    return *reports.rbegin();
}

std::string StatsManager::createDirectory(const std::string &map_name) {
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    auto path = std::string( buffer ).substr( 0, pos);
    std::string result = path + "/" + map_name;

    CreateDirectory(result.c_str(), nullptr);

    return result;
}

void StatsManager::recordStatsOnTxt(const std::string &map_name, const std::map<int, State> &solution) {

    std::string directory = createDirectory(map_name);

    std::ofstream outfile (directory + "/general_v3.txt");

    outfile << "Version 3" << std::endl;
    outfile << "Map name: " << map_name << std::endl;

    auto report = reports.front();

    unsigned int low_level_calls_total = report.low_level_calls;
    unsigned int high_level_calls_total = report.high_level_calls;
    unsigned int constraint_nodes_total = report.constraint_nodes_created;
    double duration_total = report.avg_high_level_duration;

    if (solution.empty()) {
        outfile << "Time steps: " << 0 << std::endl;
    } else {
        outfile << "Time steps: " << solution.rbegin()->first << std::endl;
    }

    outfile << "Duration: " << duration_total << std::endl;
    outfile << "High level calls: " << high_level_calls_total << std::endl;
    outfile << "Constraint nodes: " << constraint_nodes_total << std::endl;
    outfile << "Low level calls: " << low_level_calls_total << std::endl;

    outfile << std::endl << std::endl;
    outfile << "Solution found:" << std::endl;

    for (auto& it : solution) {
        outfile << "T" << it.first << ": ";

        for (auto &it_search_square : it.second.getSearchSquares()) {
            outfile << "A" << it_search_square.first << " = " << it_search_square.second->position << "; ";
        }
        outfile << std::endl;
    }

    outfile.close();
}





