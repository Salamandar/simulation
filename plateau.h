#ifndef PLATEAU_H

#include <cairomm/context.h>
#include <gtkmm.h>
#include <gdkmm/pixbuf.h>
//#include <glibmm/fileutils.h>


// Dimensions
#define PLATEAU_H 2000
#define PLATEAU_L 3000

// Le facteur de zoom (mm par pixel)
#define PLATEAU_SCALE 5

#define ROBOT_H 40
#define ROBOT_L 40

#define IMAGE_PLATEAU "plateau.png"


class Plateau : public Gtk::DrawingArea {
public:
    Plateau(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
    virtual ~Plateau(){};

    // "Ask for drawing" functions
    void setRobotPosition(int x, int y, double alpha);
    void setObstacleRond(int cx, int cy, int r);
    void setObstacleLine(int x1, int y1, int x2, int y2);
    void setPointPassageCarto(int x, int y, int type);

    bool aui();


protected:
    // Drawing functions
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
    void drawRobot(const Cairo::RefPtr<Cairo::Context>& cr);

    void drawObstacleRond(int cx, int cy, int r);
    void drawObstacleLine(int x1, int y1, int x2, int y2);
    void drawPointPassageCarto(int x, int y, int type);

    void drawAllObstaclesRond();
    void drawAllObstaclesLine();
    void drawAllPointsPassageCarto();


    Glib::RefPtr<Gtk::Builder> m_refGlade;

    Glib::RefPtr<Gdk::Pixbuf> background;

    int robot_x=100,
        robot_y=100;
    double robot_alpha=0;

};

#endif
