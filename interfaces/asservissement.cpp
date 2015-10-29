
#include "simulation.h"
#include "UART.h"
#include "asservissement.h"
#include "hardware.h"
#include <iostream>

Simulation* get_simulation();


// Called to start the processing on the thread
void AsservissementWorker::start() {
    thread = Glib::Thread::create(sigc::mem_fun(*this, &AsservissementWorker::runWork), true);
}

AsservissementWorker::~AsservissementWorker() {
    if (thread)
        thread->join(); // Here we block to truly wait for the thread to complete
}

int AsservissementWorker::get_theta(){
}
int AsservissementWorker::get_x(){
}
int AsservissementWorker::get_y(){
}
void AsservissementWorker::new_trajectoire_xy_absolu(int x, int y){
    std::cout << "Demande de nouvelle trajectoire " << x << " " << y << std::endl;
    x_newTrajectoire = x;
    y_newTrajectoire = y;
    sig_AskNewTrajectoire();
}

// This is where the real work happens
void AsservissementWorker::runWork () {
    std::cout << "This is the asser working thread !" << std::endl;
    this->sig_AskNewTrajectoire.connect(sigc::mem_fun(*this, &AsservissementWorker::nouvelle_trajectoire));

    while(true) {
        x_actuel+=1;
        y_actuel+=1;
        theta_actuel+=10;
        sig_RobotMoved();
        usleep(2000);
    }
}

void AsservissementWorker::nouvelle_trajectoire() {
    x_actuel = x_newTrajectoire;
    y_actuel = y_newTrajectoire;
}
