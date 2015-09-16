#include "plateau.h"
#include <iostream>

TableDrawingArea::TableDrawingArea(BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>&)
: Gtk::DrawingArea(cobject) {

    set_size_request(pix_long, pix_larg);

    try {
        background = Cairo::ImageSurface::create_from_png(IMAGE_PLATEAU);
    } catch(const Glib::Error& ex) {
    std::cerr << "Erreur de chargement de l'image du plateau: "
        << ex.what() << std::endl;
        exit(1);
    }

    // Initialisation du tracé de la trajectoire du robot
    surfaceTrajectoire = Cairo::ImageSurface::create(
        Cairo::Format::FORMAT_ARGB32, pix_long, pix_larg);
    contextTrajectoire = Cairo::Context::create(surfaceTrajectoire);
    contextTrajectoire->set_line_width(1.5);

    // Initialisation de la carto
    surfaceCartographie = Cairo::ImageSurface::create(
        Cairo::Format::FORMAT_ARGB32, pix_long, pix_larg);
    contextCartographie = Cairo::Context::create(surfaceCartographie);

    // Initialisation des obstacles
    surfaceObstacles    = Cairo::ImageSurface::create(
        Cairo::Format::FORMAT_ARGB32, pix_long, pix_larg);
    contextObstacles    = Cairo::Context::create(surfaceObstacles);

}

bool TableDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) {
    // Draw the image as background
    cr->set_source(background, 0, 0);
    cr->paint();

    drawPointsPassageCartoOnTable(cr);
    drawObstaclesOnTable(cr);
    drawRobot(cr);
    drawTrajectoireOnTable(cr);

    return true;
}

void TableDrawingArea::drawRobot(const Cairo::RefPtr<Cairo::Context>& cr) {
    // Position
    cr->move_to(        robot_real_x /PLATEAU_SCALE,
        (PLATEAU_LARG - robot_real_y)/PLATEAU_SCALE);

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

    // Dessin de la trajectoire
    contextTrajectoire->set_source_rgba(1,1,1,1);
    // Single pixel
    /*contextTrajectoire->rectangle(robot_real_x /PLATEAU_SCALE,
                (PLATEAU_LARG -   robot_real_y)/PLATEAU_SCALE,1,1);
    contextTrajectoire->fill();*/
    // Line
    static double old_real_x = robot_real_x,
                  old_real_y = robot_real_y;
    contextTrajectoire->move_to(old_real_x /PLATEAU_SCALE,
                (PLATEAU_LARG - old_real_y)/PLATEAU_SCALE);
    contextTrajectoire->line_to(robot_real_x /PLATEAU_SCALE,
                (PLATEAU_LARG - robot_real_y)/PLATEAU_SCALE);
    contextTrajectoire->stroke();
    old_real_x = robot_real_x;
    old_real_y = robot_real_y;
}


// Dessin des chemins de la cartographie
void TableDrawingArea::addPointPassageCarto(double x, double y, int type){
    double  pix_x =                 x /PLATEAU_SCALE,
            pix_y = (PLATEAU_LARG - y)/PLATEAU_SCALE;

    contextCartographie->set_source_rgba(1,1,1,1);

    contextCartographie->rectangle(
        pix_x-CARTO_POINT_SIZE/2,
        pix_y-CARTO_POINT_SIZE/2,
        CARTO_POINT_SIZE, CARTO_POINT_SIZE);

    contextCartographie->fill();

    queue_draw();
}

void TableDrawingArea::drawPointsPassageCartoOnTable(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->set_source(surfaceCartographie, 0, 0);
    cr->paint();
}

// Dessin des obstacles de la cartographie
void TableDrawingArea::drawObstacleRond(int cx, int cy, int r){

}
void TableDrawingArea::drawObstacleLine(int x1, int y1, int x2, int y2){

}
void TableDrawingArea::drawObstaclesOnTable(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->set_source(surfaceObstacles, 0., 0.);
    cr->paint();
}

void TableDrawingArea::setRobotPosition(int real_x,int real_y, double alpha) {
    robot_real_x = real_x;
    robot_real_y = real_y;
    robot_alpha  = alpha;
    queue_draw();
}
void TableDrawingArea::drawTrajectoireOnTable(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->set_source(surfaceTrajectoire, 0., 0.);
    cr->paint();
}


void TableDrawingArea::addObstacleRond(int cx, int cy, int r){

    queue_draw();
}
void TableDrawingArea::addObstacleLine(int x1, int y1, int x2, int y2){

    queue_draw();
}
