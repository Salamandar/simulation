#ifndef CARTOGRAPHIE_H
#define CARTOGRAPHIE_H
#include <gtkmm.h>

extern "C" {
#include "../../cartographie/cartographie.h"

}


class Simulation;

class CartographieWorker {
public:

    CartographieWorker() : thread(0) {}

    // Called to start the processing on the thread
    void start();

    void setSimulation(Simulation* simulation){
        m_Simulation = simulation;
    }

    // When shutting down, we need to stop the thread
    ~CartographieWorker();

    // To implement later for safer code.
    Glib::Dispatcher sig_AskDessineObstacle,
                     sig_AskDessinePointPassage;

    void dessine_obstacle_rond(int cx, int cy, int r);
    void dessine_obstacle_ligne(int x1, int y1, int x2, int y2);
    void dessine_point_passage_carto(int x, int y, int type);

    void new_pathfinding(int x, int y);

protected:
    int theta_actuel,
        x_actuel,
        y_actuel,
        x_newTrajectoire,
        y_newTrajectoire;

    Simulation* m_Simulation;

    // This is where the real work happens
    void runWorkInit();
    void runWork();

    Glib::Thread* thread;
};




#endif
