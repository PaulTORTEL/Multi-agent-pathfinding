//
// Created by Paul Tortel on 18/06/2019.
//

#ifndef PATHFINDING_PROJECT_ANALYSISREPORT_H
#define PATHFINDING_PROJECT_ANALYSISREPORT_H

#include <ostream>

struct AnalysisReport {

    unsigned int num_agents_involved = 0;
    unsigned int high_level_calls = 0;
    unsigned int constraint_nodes_created = 0;
    unsigned int low_level_calls = 0;
    double avg_high_level_duration = 0.;
    double avg_low_level_duration = 0.;

    friend std::ostream &operator<<(std::ostream &os, const AnalysisReport &report) {
        os << "*** ANALYSIS REPORT ***" << std::endl
        << "Number of agents involved: " << report.num_agents_involved << std::endl
        << "High level calls: " << report.high_level_calls << std::endl
        << "Const. nodes created: " << report.constraint_nodes_created << std::endl
        << "Low level calls: " << report.low_level_calls << std::endl
        << "Avg. high level duration: " << report.avg_high_level_duration << std::endl
        << "Avg. low level duration: " << report.avg_low_level_duration << std::endl;
        return os;
    }
};

#endif //PATHFINDING_PROJECT_ANALYSISREPORT_H
