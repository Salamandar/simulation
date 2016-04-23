#include <iostream>
#include "asservissement.h"
#include "../common_code/communication/keys.h"
#include "../common_code/communication/emission.h"


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
void AsservissementWorker::asser_set_trajectoire_xy_absolu(){
    set_trajectoire_xy_absolu(x_newTrajectoire, y_newTrajectoire);
}

// This is where the real work happens
void AsservissementWorker::runWork () {
    std::cout << "This is the asser working thread !" << std::endl;
    sig_AskNewTrajectoire.connect(sigc::mem_fun(*this,
        &AsservissementWorker::asser_set_trajectoire_xy_absolu));

    //init
    init_odometrie();
    init_trajectoire();
    init_hardware();
    init_asser();

    init_reception_communication();

    //lancement du robot
    //launch_tests();
    start_asser();
}
