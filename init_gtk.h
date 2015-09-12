#include <gtkmm.h>
#include "plateau.h"

#ifdef __cplusplus
extern "C" {
#endif

int init_simulation();

void set_trajectoire_xy_absolu(int x, int y);

void bouge_robot_sdl(int x, int y, double alpha);

void dessine_obstacle_rond(int cx, int cy, int r);
void dessine_obstacle_ligne(int x1, int y1, int x2, int y2);

void dessine_point_passage_carto(int x, int y, int type);

#ifdef __cplusplus
}
#endif

int init_gtk();


class Simulation {
public:
    Simulation() :
        m_Window(0),
        m_Dispatcher(),
        m_UiThread(0){};
    int init();
    int start();

    // Called from the worker thread.
    void notify();

    Gtk::Window* m_Window;
    Plateau* plateau;

protected:

    // Member data.
    Glib::RefPtr<Gtk::Application> m_Application;

    Gtk::EventBox* PlateauEventBox;
    bool on_plateau_click(GdkEventButton*);


    Glib::Dispatcher m_Dispatcher;
    Glib::Threads::Thread* m_UiThread;
};
