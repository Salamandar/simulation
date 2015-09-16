#ifndef PLATEAU_H
#define PLATEAU_H
#include <gtkmm/drawingarea.h>
#include <gtkmm/builder.h>
#include <gdkmm/pixbuf.h>
#include <cairomm/cairomm.h>

// Dimensions
#define PLATEAU_LARG 2000
#define PLATEAU_LONG 3000

// Le facteur de zoom (mm par pixel)
#define PLATEAU_SCALE 5

#define ROBOT_H 40
#define ROBOT_L 40

// Cartographie
#define CARTO_POINT_SIZE 10

#define IMAGE_PLATEAU "plateau.png"




class TableDrawingArea : public Gtk::DrawingArea {
public:
    TableDrawingArea(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
    virtual ~TableDrawingArea(){};

    // "Ask for drawing" functions
    void setRobotPosition(int real_x,int real_y, double alpha);

    // Cartographie
    void addPointPassageCarto(double x, double y, int type);

    // Obstacles
    void addObstacleRond(int cx, int cy, int r);
    void addObstacleLine(int x1, int y1, int x2, int y2);

    bool aui();


    const int pix_larg = PLATEAU_LARG/PLATEAU_SCALE;
    const int pix_long = PLATEAU_LONG/PLATEAU_SCALE;


protected:
    // Drawing functions
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

    // Dessin de la table
    Cairo::RefPtr<Cairo::Surface> background;

    // Dessin du robot (direct) et de sa trajectoire (surface)
    void drawRobot(const Cairo::RefPtr<Cairo::Context>& cr);
    Cairo::RefPtr<Cairo::Surface> surfaceTrajectoire;
    Cairo::RefPtr<Cairo::Context> contextTrajectoire;
    void drawTrajectoireOnTable(const Cairo::RefPtr<Cairo::Context>& cr);
    // Dessin des chemins de la cartographie
    Cairo::RefPtr<Cairo::Surface> surfaceCartographie;
    Cairo::RefPtr<Cairo::Context> contextCartographie;
    void drawPointsPassageCartoOnTable(const Cairo::RefPtr<Cairo::Context>& cr);

    // Dessin des obstacles de la cartographie
    Cairo::RefPtr<Cairo::Surface> surfaceObstacles;
    Cairo::RefPtr<Cairo::Context> contextObstacles;
    void drawObstaclesOnTable(const Cairo::RefPtr<Cairo::Context>& cr);

    int robot_real_x=100,
        robot_real_y=100;
    double robot_alpha=0;
};

#endif
