#include "init_gtk.h"

#include <iostream>

#include "plateau.h"

#define POINT_SIZE 40

Simulation* m_Simulation = 0;

void* start_simulation_thread(void*) {
    std::cout << "auie" << std::endl;
    m_Simulation->start();
    exit(0);
}

int init_simulation() {
    m_Simulation = new Simulation();
    m_Simulation->init();

    pthread_t uiThread;
    pthread_create(&uiThread, NULL, &start_simulation_thread, NULL);

    return 0;
}



int Simulation::init() {
    // Obtain gtk's global lock
    m_Application = Gtk::Application::create("org.robotronik.simulation");


    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
    refBuilder->add_from_file("Window.glade");

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

    refBuilder->get_widget("GlobalWindow", m_Window);

    if(m_Window) {
        m_Window->set_title("Simulation Robotronik");

        refBuilder->get_widget("PlateauEventBox", PlateauEventBox);
        PlateauEventBox->set_events(Gdk::BUTTON_PRESS_MASK);
        PlateauEventBox->signal_button_press_event().connect(
            sigc::mem_fun(*this, &Simulation::on_plateau_click) );

        refBuilder->get_widget_derived("Plateau", plateau);
        plateau->show();
        return 0;
    } else
        return 1;
}
int Simulation::start() {
    return m_Application->run(*m_Window);
}

void bouge_robot_sdl(int x, int y, double alpha){
    m_Simulation->plateau->setRobotPosition(x, y, alpha);
}
void dessine_obstacle_rond(int cx, int cy, int r){
    m_Simulation->plateau->setObstacleRond(cx, cy, r);
}
void dessine_obstacle_ligne(int x1, int y1, int x2, int y2){
    m_Simulation->plateau->setObstacleLine(x1, y1, x2, y2);
}
void dessine_point_passage_carto(int x, int y, int type) {
    m_Simulation->plateau->setPointPassageCarto(x, y, type);
}

bool Simulation::on_plateau_click(GdkEventButton* event) {
    int x =             event->x*PLATEAU_SCALE;
    int y = PLATEAU_H - event->y*PLATEAU_SCALE;
    set_trajectoire_xy_absolu(x, y);
    return true;
}
