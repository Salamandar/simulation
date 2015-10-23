#include "simulation.h"
#include <iostream>
#include "interfaces/asservissement.h"

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
            sigc::mem_fun(*this, &Simulation::on_plateau_click) );

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
    return m_Application->run(*m_Window);
}

bool Simulation::on_plateau_click(GdkEventMotion* event) {
    int x =                 event->x*PLATEAU_SCALE;
    int y = PLATEAU_LARG -  event->y*PLATEAU_SCALE;
    set_trajectoire_xy_absolu(x, y);
    plateau->setRobotPosition(x, y,0);
    return true;
}
