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


void setRobotPositionFromAsservissement() {
    get_simulation()->plateau->setRobotPosition(
        m_AsservissementWorker->get_x_actuel(),
        m_AsservissementWorker->get_y_actuel(),
        m_AsservissementWorker->get_theta_actuel());
}

void set_trajectoire_xy_absolu(int x, int y){
    m_AsservissementWorker->set_trajectoire_xy_absolu(x,y);
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

    m_AsservissementWorker->sig_RobotMoved.connect(sigc::ptr_fun(&setRobotPositionFromAsservissement));


    m_AsservissementWorker->start();
    m_CartographieWorker  ->start();



    m_Simulation->start();

    return 0;
}


