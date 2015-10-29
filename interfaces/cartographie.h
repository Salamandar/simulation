#ifndef CARTOGRAPHIE_H
#define CARTOGRAPHIE_H

extern "C" {
#include "../../cartographie/cartographie.h"

void dessine_obstacle_rond(int cx, int cy, int r);
void dessine_obstacle_ligne(int x1, int y1, int x2, int y2);
void dessine_point_passage_carto(int x, int y, int type);

}


class CartographieWorker {
public:

    CartographieWorker() : thread(0) {}

    // Called to start the processing on the thread
    void start();

    // When shutting down, we need to stop the thread
    ~CartographieWorker();

    // To implement later for safer code.
    Glib::Dispatcher sig_AskDessineObstacle,
                     sig_AskDessinePointPassage;

    void dessine_obstacle_rond(int cx, int cy, int r);
    void dessine_obstacle_ligne(int x1, int y1, int x2, int y2);
    void dessine_point_passage_carto(int x, int y, int type);

protected:
    int theta_actuel,
        x_actuel,
        y_actuel,
        x_newTrajectoire,
        y_newTrajectoire;

    // This is where the real work happens
    void runWork();

    Glib::Thread* thread;
};




#endif
