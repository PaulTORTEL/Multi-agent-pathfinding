cmake_minimum_required(VERSION 3.14)
project(Pathfinding_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Pathfinding_project src/main.cpp libs/xml-parser/tinyxml2.cpp src/XmlParser.cpp include/XmlParser.h src/map/Map.cpp include/map/Map.h src/map/Agent.cpp include/map/Agent.h include/utility.h src/solver/Solver.cpp include/solver/Solver.h src/solver/SimpleSequentialSolver.cpp include/solver/SimpleSequentialSolver.h include/map/SearchSquare.h include/state/StateDictionary.h include/solver/cbs/Constraint.h src/solver/cbs/ConflictBasedSearch.cpp include/solver/cbs/ConflictBasedSearch.h include/solver/cbs/ConstraintNode.h include/solver/cbs/Conflict.h src/state/State.cpp include/state/State.h src/stats/StatsManager.cpp include/stats/StatsManager.h include/stats/AnalysisReport.h)