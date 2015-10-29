#ifndef ASSERVISSEMENT_H
#define ASSERVISSEMENT_H

void new_trajectoire_xy_absolu(int x, int y);


class AsservissementWorker {
public:

    AsservissementWorker() : thread(0) {}

    // Called to start the processing on the thread
    void start();

    // When shutting down, we need to stop the thread
    ~AsservissementWorker();

    Glib::Dispatcher sig_RobotMoved,
                     sig_AskNewTrajectoire;

    int get_theta();
    int get_x();
    int get_y();
    void new_trajectoire_xy_absolu(int x, int y);

protected:
    int theta_actuel,
        x_actuel,
        y_actuel,
        x_newTrajectoire,
        y_newTrajectoire;

    // This is where the real work happens
    void runWork();
    void nouvelle_trajectoire();

    Glib::Thread* thread;
};




#endif
