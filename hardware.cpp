#include <stdio.h>
#include <pthread.h>

#include "simulation.h"
#include "UART.h"
#include "asservissement.h"
#include "hardware.h"

#include <iostream>

Simulation* m_Simulation = 0;

void start_simulation_thread() {
    m_Simulation->start();
    exit(0);
}

bool setRobotPositionFromAsservissement() {
    bouge_robot_sdl(float_get_x_actuel(), float_get_y_actuel(), (double)get_theta_actuel());
    return true;
}

void bouge_robot_sdl(int x, int y, double alpha){
    m_Simulation->plateau->setRobotPosition(x, y, alpha);
}
void dessine_obstacle_rond(int cx, int cy, int r){
    m_Simulation->plateau->addObstacleRond(cx, cy, r);
}
void dessine_obstacle_ligne(int x1, int y1, int x2, int y2){
    m_Simulation->plateau->addObstacleLine(x1, y1, x2, y2);
}
void dessine_point_passage_carto(int x, int y, int type) {
    m_Simulation->plateau->addPointPassageCarto(x, y, type);
}

int init_hardware_GTK() {
    m_Simulation = new Simulation();
    m_Simulation->init();

    if(!Glib::thread_supported()) Glib::thread_init();

    Glib::Thread* thread;
    sigc::ptr_fun(&start_simulation_thread);
    thread = Glib::Thread::create(sigc::ptr_fun(&start_simulation_thread), true);

    std::cout << "This is the working thread !" << std::endl;
    Glib::signal_timeout().connect( sigc::ptr_fun(&setRobotPositionFromAsservissement), 20);

    return 0;
}

Simulation* get_simulation() {
    return m_Simulation;
}
