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
    contextObstacles->set_line_width(3);
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
    cr->save();
    // Position
    cr->move_to(        robot_real_x /PLATEAU_SCALE,
        (PLATEAU_LARG - robot_real_y)/PLATEAU_SCALE);

    // Offset for the robot to be well centered
    cr->rotate((M_PI - robot_alpha)/1000);
    cr->rel_move_to(0,ROBOT_L/3);

    // Shape
    cr->rel_line_to(-ROBOT_L/2,0);
    cr->rel_line_to( ROBOT_L/2,-ROBOT_L);
    cr->rel_line_to( ROBOT_L/2, ROBOT_L);
    cr->rel_line_to(-ROBOT_L/2,0);

    cr->set_source_rgba(0,0,0,1);
    cr->fill();
    cr->restore();
}


// Dessin des chemins de la cartographie
void TableDrawingArea::addPointPassageCarto(double x, double y, int type){
    double  pix_x =                 x /PLATEAU_SCALE,
            pix_y = (PLATEAU_LARG - y)/PLATEAU_SCALE;

    contextCartographie->rectangle(
        pix_x-CARTO_POINT_SIZE/2,
        pix_y-CARTO_POINT_SIZE/2,
        CARTO_POINT_SIZE, CARTO_POINT_SIZE);

    switch(type) {
        case 0: // Point ouvert
            contextCartographie->set_source_rgba(0.0,0.0,1.0,0.5);
            break;
        case 1: // Point visité
            contextCartographie->set_source_rgba(0.0,0.0,0.5,1.0);
            break;
        case 2: // Point de passage astar
            contextCartographie->set_source_rgba(0.0,0.5,0.0,1.0);
            break;
        case 3: // Point de passage thetastar (réel)
            contextCartographie->set_source_rgba(0.0,1.0,0.0,1.0);
            break;
        case 4: // Point borne
            contextCartographie->set_source_rgba(1.0,1.0,1.0,1.0);
            break;
        default:
            contextCartographie->set_source_rgba(1.0,1.0,1.0,1.0);
    }

    contextCartographie->fill();

    //queue_draw();
}

void TableDrawingArea::drawPointsPassageCartoOnTable(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->set_source(surfaceCartographie, 0, 0);
    cr->paint();
}

// Dessin des obstacles de la cartographie
void TableDrawingArea::drawObstaclesOnTable(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->set_source(surfaceObstacles, 0., 0.);
    cr->paint();
}

void TableDrawingArea::setRobotPosition(int real_x,int real_y, double alpha) {
    robot_real_x = real_x;
    robot_real_y = real_y;
    robot_alpha  = alpha;

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
    queue_draw();
}
void TableDrawingArea::drawTrajectoireOnTable(const Cairo::RefPtr<Cairo::Context>& cr){
    cr->set_source(surfaceTrajectoire, 0., 0.);
    cr->paint();
}


void TableDrawingArea::addObstacleRond(int cx, int cy, int r){
    contextObstacles->arc(cx /PLATEAU_SCALE,
          (PLATEAU_LARG - cy)/PLATEAU_SCALE,
          r/PLATEAU_SCALE, 0, 2 * M_PI);
    contextObstacles->set_source_rgba(1,0,0,1);
    contextObstacles->stroke_preserve();
    contextObstacles->set_source_rgba(1,0,0,0.5);
    contextObstacles->fill();
    //queue_draw();
}
void TableDrawingArea::addObstacleLine(int x1, int y1, int x2, int y2){
    contextObstacles->set_source_rgba(1,0,0,0.5);
    contextObstacles->move_to(x1 /PLATEAU_SCALE,
              (PLATEAU_LARG - y1)/PLATEAU_SCALE);
    contextObstacles->line_to(x2 /PLATEAU_SCALE,
              (PLATEAU_LARG - y2)/PLATEAU_SCALE);
    contextObstacles->stroke();
    //queue_draw();
}


void TableDrawingArea::cleanContext(Cairo::RefPtr<Cairo::Context> contexte){
    contexte->save();
    contexte->set_source_rgba(0,0,0,0);
    contexte->set_operator(Cairo::OPERATOR_SOURCE);
    contexte->paint();
    contexte->restore();
}

void TableDrawingArea::cleanTrajectoire(){
    cleanContext(contextTrajectoire);
}
void TableDrawingArea::cleanPassageCarto(){
    cleanContext(contextCartographie);
}
void TableDrawingArea::cleanObstacles(){
    cleanContext(contextObstacles);
}
