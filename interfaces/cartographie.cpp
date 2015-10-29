#include <stdio.h>
#include <pthread.h>

#include "simulation.h"
#include "UART.h"
#include "cartographie.h"
#include "hardware.h"

#include <iostream>

Simulation* get_simulation();

// Called to start the processing on the thread
void CartographieWorker::start() {
        thread = Glib::Thread::create(sigc::mem_fun(*this, &CartographieWorker::runWork), true);
}

CartographieWorker::~CartographieWorker() {
    if (thread)
        thread->join(); // Here we block to truly wait for the thread to complete
}

void CartographieWorker::dessine_obstacle_rond(int cx, int cy, int r){
    get_simulation()->plateau->addObstacleRond(cx, cy, r);
}
void CartographieWorker::dessine_obstacle_ligne(int x1, int y1, int x2, int y2){
    get_simulation()->plateau->addObstacleLine(x1, y1, x2, y2);
}
void CartographieWorker::dessine_point_passage_carto(int x, int y, int type) {
    get_simulation()->plateau->addPointPassageCarto(x, y, type);
}


// This is where the real work happens
void CartographieWorker::runWork() {
    std::cout << "This is the carto working thread !" << std::endl;
    pathfinding_init();
    pathfinding(500,1500,2500,1501);

}
