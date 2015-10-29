#include <stdio.h>
#include <pthread.h>

#include "simulation.h"
#include "UART.h"
#include "interfaces/asservissement.h"
#include "interfaces/cartographie.h"
#include "hardware.h"

#include <iostream>

Simulation* m_Simulation = 0;
AsservissementWorker* m_AsservissementWorker = 0;
CartographieWorker* m_CartographieWorker = 0;

void start_working_thread() {
    std::cout << "This is the basic working thread !" << std::endl;

    init_UART();

}


Simulation* get_simulation() {
    return m_Simulation;
}


bool setRobotPositionFromAsservissement() {
    get_simulation()->plateau->setRobotPosition(
        m_AsservissementWorker->get_x(),
        m_AsservissementWorker->get_y(),
        m_AsservissementWorker->get_theta());
    return true;
}

void new_trajectoire_xy_absolu(int x, int y){
    m_AsservissementWorker->new_trajectoire_xy_absolu(x,y);
}

void dessine_obstacle_rond(int cx, int cy, int r){
    m_CartographieWorker->dessine_obstacle_rond(cx, cy, r);
}
void dessine_obstacle_ligne(int x1, int y1, int x2, int y2){
    m_CartographieWorker->dessine_obstacle_ligne(x1, y1, x2, y2);
}
void dessine_point_passage_carto(int x, int y, int type) {
    m_CartographieWorker->dessine_point_passage_carto(x, y, type);
}



int main() {
    // Let's initialize the main simulation process
    m_Simulation = new Simulation();
    m_Simulation->init();

    // Pour ne pas initialiser plusieurs fois les threads, ce qui est fatal.
    if(!Glib::thread_supported())
        Glib::thread_init();

    m_AsservissementWorker = new AsservissementWorker();
    m_CartographieWorker   = new CartographieWorker();

    Glib::signal_timeout().connect( sigc::ptr_fun(&setRobotPositionFromAsservissement), 10);
    //m_AsservissementWorker->sig_RobotMoved.connect(sigc::ptr_fun(&setRobotPositionFromAsservissement));


    m_AsservissementWorker->start();
    m_CartographieWorker  ->start();



    m_Simulation->start();

    return 0;
}


