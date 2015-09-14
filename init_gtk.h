#include <gtkmm.h>
#include "plateau.h"

void set_trajectoire_xy_absolu(int x, int y);

int init_simulation();

void bouge_robot_sdl(int x, int y, double alpha);

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
