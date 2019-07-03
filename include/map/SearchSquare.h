//
// Created by Paul Tortel on 17/05/2019.
//

#ifndef PATHFINDING_PROJECT_SEARCHSQUARE_H
#define PATHFINDING_PROJECT_SEARCHSQUARE_H

#include "../utility.h"
#include <memory>

struct SearchSquare {

    enum AgentStatus {
        READY,
        FINISHED,
        MOVING,
        PICKING,
        DROPPING,
        ERROR,
        BEING_FIXED
    };

    /**
     * The position related to the search square
     */
    Position position;

    /**
     * The cost of the movement and of the heuristic
     */
    int cost_movement, cost_heuristic;

    /**
     * The time step from which the search square has been created
     */
    int time_step;

    /**
     * A shared pointer on the parent (which is a search square as well)
     */
    std::shared_ptr<SearchSquare> parent;

    AgentStatus agent_status = READY;
    int interacting_time_left = 0;


    explicit SearchSquare(Position position, std::shared_ptr<SearchSquare> parent = nullptr,
                          int cost_movement = 0, int cost_heuristic = 0) :
                position(position), parent(parent), cost_movement(cost_movement), cost_heuristic(cost_heuristic) {

        if (parent != nullptr) {
            time_step = parent.get()->time_step + 1;
        } else {
            time_step = 0;
        }
    }


    explicit SearchSquare() {
        position = Position(0,0);
        cost_heuristic = 0;
        cost_movement = 0;
        time_step = 0;
        parent = nullptr;
    }

    int cost() const {
        return cost_movement + cost_heuristic;
    }

    void setCurrentStatus(AgentStatus new_current_status) {
        agent_status = new_current_status;
    }

    /**
     * Set the current agent state
     * @param new_current_status : the new status of the agent
     * @param point_of_interest : the pontential point of interest that explains the status changement
     */
    void setCurrentStatus(AgentStatus new_current_status, PointOfInterest point_of_interest) {
        setCurrentStatus(new_current_status);

        switch (point_of_interest) {
            case NA:
                interacting_time_left = 0;
                break;
            case PRODUCT:
                interacting_time_left = 1;
                break;
            case REPAIR_POINT:
                interacting_time_left = 5;
                break;
            case DROP_OFF_POINT:
                interacting_time_left = 2;
                break;
        }
    }

    void setInteractingTimeLeft(const int time_left) {
        if (time_left > 0) {
            interacting_time_left = time_left;
        } else {
            setCurrentStatus(READY, NA);
        }
    }

    bool isAgentInteracting() {
        switch (agent_status) {

            case READY: return false;
            case MOVING: return false;
            case PICKING: return true;
            case DROPPING: return true;
            case ERROR: return false;
            case BEING_FIXED: return true;
            case FINISHED: return false;
        }
        return false;
    }

    bool isAgentReady() {
        return agent_status == READY;
    }
};
#endif //PATHFINDING_PROJECT_SEARCHSQUARE_H
