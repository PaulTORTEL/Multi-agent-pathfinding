//
// Created by Paul Tortel on 18/06/2019.
//

#ifndef PATHFINDING_PROJECT_STATSMANAGER_H
#define PATHFINDING_PROJECT_STATSMANAGER_H

#include "AnalysisReport.h"
#include <ctime>

class StatsManager {
private:
    std::vector<AnalysisReport> reports;
    std::clock_t highLevelClock;
    std::clock_t lowLevelClock;

public:

    static void startClock(std::clock_t &clock);
    static double stopClock(std::clock_t &clock);

    void startAnalysis(const int num_of_agents);
    void stopAnalysis();
    AnalysisReport& getCurrentAnalysis();

    enum AnalysisLevel {
        LOW_LEVEL,
        HIGH_LEVEL
    };

    void startMeasuringDuration(AnalysisLevel analysisLevel);

    void stopMeasuringDuration(AnalysisLevel analysisLevel);


    void recordStatsOnTxt(const std::string& map_name);

    std::string createDirectory(const std::string &map_name);
};


#endif //PATHFINDING_PROJECT_STATSMANAGER_H
