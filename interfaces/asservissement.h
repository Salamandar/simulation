#ifndef ASSERVISSEMENT_H
#define ASSERVISSEMENT_H

void set_trajectoire_xy_absolu(int x, int y);


class AsservissementWorker {
public:

    AsservissementWorker() : thread(0) {}

    // Called to start the processing on the thread
    void start();

    // When shutting down, we need to stop the thread
    ~AsservissementWorker();

    Glib::Dispatcher sig_RobotMoved,
                     sig_AskNewTrajectoire;

    int get_theta_actuel();
    int get_x_actuel();
    int get_y_actuel();
    void set_trajectoire_xy_absolu(int x, int y);

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
