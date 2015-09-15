#include <stdio.h>
#include <pthread.h>

#include "init_gtk.h"
#include "UART.h"

#include <iostream>

Simulation* m_Simulation = 0;

void* start_simulation_thread(void*) {
    m_Simulation->start();
    exit(0);
}

int main(int argc, char const *argv[]) {
    m_Simulation = new Simulation();
    m_Simulation->init();

    pthread_t uiThread;
    pthread_create(&uiThread, NULL, &start_simulation_thread, NULL);

    init_UART_thread();

    std::cout << "auie" << std::endl;



    pause();

    return 0;
}




void bouge_robot_sdl(int x, int y, double alpha){
    m_Simulation->plateau->setRobotPosition(x, y, alpha);
}
void dessine_obstacle_rond(int cx, int cy, int r){
    m_Simulation->plateau->setObstacleRond(cx, cy, r);
}
void dessine_obstacle_ligne(int x1, int y1, int x2, int y2){
    m_Simulation->plateau->setObstacleLine(x1, y1, x2, y2);
}
void dessine_point_passage_carto(int x, int y, int type) {
    m_Simulation->plateau->setPointPassageCarto(x, y, type);
}
