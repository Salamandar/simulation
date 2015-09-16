#include "init_gtk.h"

#include <iostream>

#include "plateau.h"

#define POINT_SIZE 40

int Simulation::init() {
    // Obtain gtk's global lock
    m_Application = Gtk::Application::create("org.robotronik.simulation");

    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

    try {
        refBuilder->add_from_file("Window.glade");

        refBuilder->get_widget("GlobalWindow", m_Window);
        m_Window->set_title("Simulation Robotronik");

        refBuilder->get_widget("TableEventBox", TableEventBox);
        TableEventBox->set_events(Gdk::BUTTON_PRESS_MASK);
        TableEventBox->signal_event().connect(
            sigc::mem_fun(*this, &Simulation::on_plateau_click) );

        refBuilder->get_widget_derived("TableDrawingArea", plateau);

        Gsv::init();
        m_communicationView.init(refBuilder);
        refBuilder->get_widget("CommunicationSourceView", UART_view);
        UART_tbuffer = Gtk::TextBuffer::create();

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
    return m_Application->run(*m_Window);
}

bool Simulation::on_plateau_click(GdkEvent* event) {
    double pix_x, pix_y;
    Gdk::Event _event(event);
    double& rpix_x(pix_x);
    double& rpix_y(pix_y);
    _event.get_coords(rpix_x, rpix_y);
    int x =                 pix_x*PLATEAU_SCALE;
    int y = PLATEAU_LARG -  pix_y*PLATEAU_SCALE;
    set_trajectoire_xy_absolu(x, y);
    plateau->setRobotPosition(x,y,0);
    return true;
}
