#ifndef SIMULATION_H
#define SIMULATION_H

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/textbuffer.h>
#include <gtkmm/eventbox.h>
#include <gtksourceviewmm.h>
#include "communication_view.h"
#include "plateau.h"

#include "interfaces/asservissement.h"
#include "interfaces/cartographie.h"

#define SIMULATION_UI_FILE "Window.glade"

int init_simulation();

int init_gtk();


class Simulation {
public:
    Simulation() :
        m_Window(0),
        sig_RobotMoved(),
        m_UiThread(0){};
    int init();
    int start();

    void setAsservissementWorker(AsservissementWorker* asservissementWorker) {
        m_AsservissementWorker = asservissementWorker;
        m_AsservissementWorker->setSimulation(this);
    }
    void setCartographieWorker(CartographieWorker* cartographieWorker) {
        m_CartographieWorker = cartographieWorker;
        m_CartographieWorker->setSimulation(this);
    }

    CommunicationView m_communicationView;

    Gtk::Window* m_Window;
    TableDrawingArea* plateau;

    Gsv::View *UART_view;
    Glib::RefPtr<Gtk::TextBuffer> UART_tbuffer;
    bool setRobotPositionFromAsservissement();


protected:

    // Member data.
    Glib::RefPtr<Gtk::Application> m_Application;

    Gtk::EventBox* TableEventBox;
    bool on_plateau_movem(GdkEventMotion*);
    bool on_plateau_click(GdkEventButton*);

    AsservissementWorker*   m_AsservissementWorker;
    CartographieWorker*     m_CartographieWorker;

    Glib::Dispatcher sig_RobotMoved;
    Glib::Threads::Thread* m_UiThread;
};

#endif
