#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/eventbox.h>
#include <gtksourceviewmm.h>
#include "communication_view.h"
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

    CommunicationView m_communicationView;

    Gtk::Window* m_Window;
    TableDrawingArea* plateau;

    Gsv::View *UART_view;
    Glib::RefPtr<Gtk::TextBuffer> UART_tbuffer;

protected:

    // Member data.
    Glib::RefPtr<Gtk::Application> m_Application;

    Gtk::EventBox* TableEventBox;
    bool on_plateau_click(GdkEvent*);


    Glib::Dispatcher m_Dispatcher;
    Glib::Threads::Thread* m_UiThread;
};
