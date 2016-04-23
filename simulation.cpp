#include <iostream>
#include "simulation.h"

int Simulation::init() {
    // Obtain gtk's global lock
    m_Application = Gtk::Application::create("org.robotronik.simulation");

    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

    try {
        refBuilder->add_from_file(SIMULATION_UI_FILE);

        refBuilder->get_widget("GlobalWindow", m_Window);
        m_Window->set_title("Simulation Robotronik");

        refBuilder->get_widget("TableEventBox", TableEventBox);
        TableEventBox->set_events(Gdk::BUTTON_PRESS_MASK);
        TableEventBox->signal_motion_notify_event().connect(
            sigc::mem_fun(*this, &Simulation::on_plateau_movem));
        TableEventBox->signal_button_press_event().connect(
            sigc::mem_fun(*this, &Simulation::on_plateau_click));

        refBuilder->get_widget_derived("TableDrawingArea", plateau);

        Gsv::init();
        m_communicationView.init(refBuilder);
        return 0;

    } catch(const Glib::FileError& ex) {
        std::cerr << "FileError: "   <<ex.what() << std::endl;
        return 1;

    } catch(const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " <<ex.what() << std::endl;
        return 1;

    }catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: "<<ex.what() << std::endl;
        return 1;
    }
}

int Simulation::start() {
    std::cout << "This is the UI thread !" << std::endl;

    m_AsservissementWorker->start();
#ifdef SIMU_WITH_CARTO
    m_CartographieWorker  ->start();
#endif
    Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &Simulation::setRobotPositionFromAsservissement),
        40);
    return m_Application->run(*m_Window);
}


bool Simulation::on_plateau_movem(GdkEventMotion* event) {
    int x =                 event->x*PLATEAU_SCALE;
    int y = PLATEAU_LARG -  event->y*PLATEAU_SCALE;
    m_AsservissementWorker->x_newTrajectoire = x;
    m_AsservissementWorker->y_newTrajectoire = y;
    m_AsservissementWorker->sig_AskNewTrajectoire.emit();
    return true;
}
bool Simulation::on_plateau_click(GdkEventButton* event) {
    int x =                 event->x*PLATEAU_SCALE;
    int y = PLATEAU_LARG -  event->y*PLATEAU_SCALE;
    m_AsservissementWorker->x_newTrajectoire = x;
    m_AsservissementWorker->y_newTrajectoire = y;
    m_AsservissementWorker->sig_AskNewTrajectoire.emit();
    plateau->cleanPassageCarto();
#ifdef SIMU_WITH_CARTO
    m_CartographieWorker->new_pathfinding(x,y);
#endif
    return true;
}

bool Simulation::setRobotPositionFromAsservissement() {
    plateau->setRobotPosition(
        m_AsservissementWorker->get_x(),
        m_AsservissementWorker->get_y(),
        m_AsservissementWorker->get_theta());
    return true;
}
