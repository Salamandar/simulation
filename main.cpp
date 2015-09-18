#include <stdio.h>
#include <pthread.h>

#include "simulation.h"
#include "UART.h"
#include "asservissement.h"
#include "hardware.h"

#include <iostream>

bool bouge_robot() {
    static double x = 0, y = 0, alpha = 0;
    bouge_robot_sdl(x,y,alpha);
    x+=10;
    y+=10;
    alpha+=0.10;
    return true;
}


int main() {
    init_hardware_GTK();
    init_UART();
    std::cout << "auie" << std::endl;

    dessine_obstacle_ligne(0,0,1000,1500);
    dessine_obstacle_rond(1000,1500,300);

    for (int i = 0; i < 20; ++i)
        dessine_point_passage_carto(100*i,100*i,i%4);

    Glib::signal_timeout().connect( sigc::ptr_fun(&bouge_robot), 20);

    pause();

    return 0;
}


