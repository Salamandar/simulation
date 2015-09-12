#include "plateau.h"
#include <iostream>

#include "asservissement.h"
#include "init_gtk.h"

bool setRobotPositionFromAsservissement() {
    bouge_robot_sdl(
        get_x_actuel()/PLATEAU_SCALE,
        (PLATEAU_H - get_y_actuel())/PLATEAU_SCALE,
        (double)get_theta_actuel()/-1000);
    return true;
}



Plateau::Plateau(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade)
: Gtk::DrawingArea(cobject),
  m_refGlade(refGlade) {
    set_size_request(
        PLATEAU_L/PLATEAU_SCALE,
        PLATEAU_H/PLATEAU_SCALE);

    try {
        background = Gdk::Pixbuf::create_from_file("../hardware/PC/plateau.png");

    } catch(const Glib::FileError& ex) {
    std::cerr << "FileError: " << ex.what() << std::endl;

    } catch(const Gdk::PixbufError& ex) {
    std::cerr << "PixbufError: " << ex.what() << std::endl;
    }

    Glib::signal_timeout().connect( sigc::ptr_fun(&setRobotPositionFromAsservissement), 20 );
}

bool Plateau::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    //Gtk::Allocation allocation = get_allocation();
    //const int width = allocation.get_width();
    //const int height = allocation.get_height();

    // Draw the image as background
    Gdk::Cairo::set_source_pixbuf(cr, background, 0, 0);
    cr->paint();

    drawRobot(cr);

    return true;
}

void Plateau::drawRobot(const Cairo::RefPtr<Cairo::Context>& cr) {
    // Position
    cr->move_to(robot_x,robot_y);

    // Offset for the robot to be well centered
    cr->rotate(robot_alpha);
    cr->rel_move_to(0,ROBOT_L/2);

    // Shape
    cr->rel_line_to(-ROBOT_L/2,0);
    cr->rel_line_to( ROBOT_L/2,-ROBOT_L);
    cr->rel_line_to( ROBOT_L/2, ROBOT_L);
    cr->rel_line_to(-ROBOT_L/2,0);

    cr->set_source_rgba(0,0,0,1);
    cr->fill();
}


void Plateau::setRobotPosition(int x,int y, double alpha) {
    robot_x = x;
    robot_y = y;
    robot_alpha = alpha;

    queue_draw();
}


void Plateau::setObstacleRond(int cx, int cy, int r){

    queue_draw();
}
void Plateau::setObstacleLine(int x1, int y1, int x2, int y2){

    queue_draw();
}
void Plateau::setPointPassageCarto(int x, int y, int type){

    switch (type) {
        case 0: // Point ouvert
            break;
        case 1: // Point visité
            break;
        case 2: // Point de passage réel
            break;
        case 3: // Point borne
            break;
        default:
            break;
    }
    queue_draw();
}
