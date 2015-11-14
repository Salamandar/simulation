#include <stdio.h>
#include <iostream>

#include "simulation.h"
#include "interfaces/asservissement.h"
#include "interfaces/cartographie.h"

#include "interfaces/communication.h"

/*
 TODO :
 * Leds de débug pour stm32
 * "Parsing" de la réponse de la carto pour l'envoyer à l'asser

 Plus long terme :
 * Sockets
 * Communication USB Série
 * Servo
 * soft carte alim :P
 */


Simulation* m_Simulation = 0;
AsservissementWorker* m_AsservissementWorker = 0;
CartographieWorker* m_CartographieWorker = 0;

extern "C" {
    // Interfaçage avec le code C de la cartographie
    void dessine_obstacle_rond(int cx, int cy, int r){
        m_CartographieWorker->dessine_obstacle_rond(cx, cy, r);
    }
    void dessine_obstacle_ligne(int x1, int y1, int x2, int y2){
        m_CartographieWorker->dessine_obstacle_ligne(x1, y1, x2, y2);
    }
    void dessine_point_passage_carto(int x, int y, int type) {
        m_CartographieWorker->dessine_point_passage_carto(x, y, type);
    }
    void UART_send_message(char *msg, unsigned int nb_char) {
        m_Simulation->m_communicationView.append_received_line(msg);
    }
}

int main() {
    // Let's initialize the main simulation process
    m_Simulation = new Simulation();
    m_Simulation->init();

    // Pour ne pas initialiser plusieurs fois les threads, ce qui est fatal.
    if(!Glib::thread_supported())
        Glib::thread_init();

    m_AsservissementWorker = new AsservissementWorker();
    m_CartographieWorker   = new CartographieWorker();

    m_Simulation->setAsservissementWorker(m_AsservissementWorker);
    m_Simulation->setCartographieWorker(m_CartographieWorker);


    // Just a test for the serial port detection
    vector<string> l =SerialComm::list_open_ports();
    vector<string>::iterator it = l.begin();
    while (it != l.end()) {
        cout << *it << endl;
        it++;
    }

    m_Simulation->start();

    return 0;
}


