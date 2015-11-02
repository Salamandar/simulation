#include <stdio.h>
#include <pthread.h>

#include "simulation.h"
#include "UART.h"
#include "cartographie.h"
#include "hardware.h"

#include <iostream>

// Called to start the processing on the thread
void CartographieWorker::start() {
    thread = Glib::Thread::create(sigc::mem_fun(*this, &CartographieWorker::runWorkInit), true);
}
void CartographieWorker::new_pathfinding(int x, int y) {
    x_newTrajectoire = x;
    y_newTrajectoire = y;
    thread = Glib::Thread::create(sigc::mem_fun(*this, &CartographieWorker::runWork), true);
}

CartographieWorker::~CartographieWorker() {
    if (thread)
        thread->join(); // Here we block to truly wait for the thread to complete
}

void CartographieWorker::dessine_obstacle_rond(int cx, int cy, int r){
    m_Simulation->plateau->addObstacleRond(cx, cy, r);
}
void CartographieWorker::dessine_obstacle_ligne(int x1, int y1, int x2, int y2){
    m_Simulation->plateau->addObstacleLine(x1, y1, x2, y2);
}
void CartographieWorker::dessine_point_passage_carto(int x, int y, int type) {
    m_Simulation->plateau->addPointPassageCarto(x, y, type);
}


// This is where the real work happens
void CartographieWorker::runWorkInit() {
    std::cout << "This is the carto init thread !" << std::endl;
    pathfinding_init();
}
void CartographieWorker::runWork() {
    int status = pathfinding(x_actuel, y_actuel, x_newTrajectoire, y_newTrajectoire);
    if (status) {
        x_actuel = x_newTrajectoire;
        y_actuel = y_newTrajectoire;
    }
}
