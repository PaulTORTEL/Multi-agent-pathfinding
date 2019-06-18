//
// Created by Paul Tortel on 18/06/2019.
//

#include <vector>
#include <iostream>
#include "../../include/stats/StatsManager.h"

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



