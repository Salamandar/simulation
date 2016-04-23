#ifndef ASSERVISSEMENT_H
#define ASSERVISSEMENT_H
#include <gtkmm.h>
#include "ForkAvecBus.hpp"

extern "C" {
void init_odometrie();
void init_trajectoire();
void init_hardware();
void init_asser();
void init_reception_communication();

void start_asser();
void set_trajectoire_xy_absolu(int x, int y);

float float_get_x_actuel();
float float_get_y_actuel();
int   get_theta_actuel();

}

#define EXEC "../asservissement/build/PC/_DEBUG_/asser_robot"


class Simulation;

class AsservissementWorker {
public:

    AsservissementWorker()
    : sig_AskNewTrajectoire(),
      thread(0),
      AsserFork(EXEC)
    {}

    // Called to start the processing on the thread
    void start();
    void setSimulation(Simulation* simulation){
        m_Simulation = simulation;
    }

    // When shutting down, we need to stop the thread
    ~AsservissementWorker();

    Glib::Dispatcher sig_AskNewTrajectoire;

    int get_theta();
    int get_x();
    int get_y();
    void asser_set_trajectoire_xy_absolu();

    char SendBuffer[100];
    ForkAvecBus AsserFork;


    int theta_actuel,
        x_actuel,
        y_actuel,
        x_newTrajectoire,
        y_newTrajectoire;

    Simulation* m_Simulation;

    // This is where the real work happens
    void runWork();

    Glib::Thread* thread;
};




#endif
