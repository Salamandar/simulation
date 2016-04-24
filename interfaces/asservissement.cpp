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
    send_val(SendBuffer, keys[VAL_X], x_newTrajectoire);
    AsserFork.send(SendBuffer);
    send_val(SendBuffer, keys[VAL_Y], y_newTrajectoire);
    AsserFork.send(SendBuffer);
    send_cmd(SendBuffer, keys[FCT_XY_ABSOLU]);
    AsserFork.send(SendBuffer);
}

// This is where the real work happens
void AsservissementWorker::runWork () {
    std::cout << "This is the asser working thread !" << std::endl;
    sig_AskNewTrajectoire.connect(sigc::mem_fun(*this,
        &AsservissementWorker::asser_set_trajectoire_xy_absolu));

    AsserFork.start();

}
