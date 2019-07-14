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

void StatsManager::recordStatsOnTxt(const std::string &map_name, const int num_time_step) {

    std::string directory = createDirectory(map_name);

    std::ofstream outfile (directory + "/general_v1.txt");

    outfile << "Version 1" << std::endl;
    outfile << "Map name: " << map_name << std::endl;

    unsigned int low_level_calls_total = 0;
    unsigned int high_level_calls_total = 0;
    unsigned int constraint_nodes_total = 0;
    double duration_total = 0.;

    for (auto& report : reports) {
        low_level_calls_total += report.low_level_calls;
        high_level_calls_total += report.high_level_calls;
        constraint_nodes_total += report.constraint_nodes_created;
        duration_total += report.avg_high_level_duration;
    }

    unsigned int low_level_calls_avg = low_level_calls_total / reports.size();
    unsigned int constraint_nodes_avg = constraint_nodes_total / reports.size();

    double sd_sum_const_node = 0;
    double sd_sum_low_level_calls = 0;
    for (auto& report : reports) {
        sd_sum_const_node += pow(report.constraint_nodes_created, 2) - pow(constraint_nodes_avg, 2);
        sd_sum_low_level_calls += pow(report.low_level_calls, 2) - pow(low_level_calls_avg, 2);
    }

    double standard_dev_const_node = sqrt((1./reports.size()) * sd_sum_const_node);
    double standard_dev_low_level_calls = sqrt((1./reports.size()) * sd_sum_low_level_calls);

    outfile << "Time steps: " << num_time_step << std::endl;
    outfile << "Duration: " << duration_total << std::endl;
    outfile << "High level calls: " << high_level_calls_total << std::endl;
    outfile << "Constraint nodes: " << constraint_nodes_total << " / avg: " << constraint_nodes_avg << " / SD: " << standard_dev_const_node << std::endl;
    outfile << "Low level calls: " << low_level_calls_total << " / avg: " << low_level_calls_avg << " / SD: " << standard_dev_low_level_calls << std::endl;

    outfile.close();
}



