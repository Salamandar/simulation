
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
    return get_theta_actuel();
}
int AsservissementWorker::get_x(){
    return float_get_x_actuel();
}
int AsservissementWorker::get_y(){
    return float_get_y_actuel();
}
void AsservissementWorker::new_trajectoire_xy_absolu(int x, int y){
    std::cout << "Demande de nouvelle trajectoire " << x << " " << y << std::endl;
    x_newTrajectoire = x;
    y_newTrajectoire = y;
    sig_AskNewTrajectoire();
    set_trajectoire_xy_absolu(x, y);
}

// This is where the real work happens
void AsservissementWorker::runWork () {
    std::cout << "This is the asser working thread !" << std::endl;
    //this->sig_AskNewTrajectoire.connect(sigc::mem_fun(*this, &AsservissementWorker::nouvelle_trajectoire));

    //init
    init_odometrie();
    init_trajectoire();
    init_hardware();
    init_asser();

    //lancement du robot
    //launch_tests();
    start_asser();
}

void AsservissementWorker::nouvelle_trajectoire() {
    x_actuel = x_newTrajectoire;
    y_actuel = y_newTrajectoire;
}
