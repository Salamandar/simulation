#ifndef ASSERVISSEMENT_H
#define ASSERVISSEMENT_H
#include <gtkmm.h>

extern "C" {
void init_odometrie();
void init_trajectoire();
void init_hardware();
void init_asser();
void start_asser();
void set_trajectoire_xy_absolu(int x, int y);

float float_get_x_actuel();
float float_get_y_actuel();
int   get_theta_actuel();

}


class Simulation;

class AsservissementWorker {
public:

    AsservissementWorker()
    : sig_AskNewTrajectoire(),
      thread(0)
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
