#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <vector>
#include <string>

#include "../libserial/SerialStream.h"

#include <glibmm/main.h>
#include <glibmm/dispatcher.h>

using namespace std;
using namespace LibSerial;

class CommunicationView;

class SerialComm {
public:
    SerialComm(CommunicationView* c, Glib::Dispatcher* sig_reception)
    : sig_received(sig_reception),
      m_CommunicationView(c)
       { }
    static vector<string> list_open_ports();

    int connect(string port);
    int send(string texte);


protected:
    SerialStream serial_port;
    int baud_rate = LibSerial::SerialStreamBuf::BAUD_115200;
    static vector<string> open_ports;
    static string get_driver(const string& tty);
    static void register_comport(const string& dir);


    // String send
    string to_send;

    bool to_UART    = true,
         to_socket  = true,
         to_fakeUART= true;
    int send_to_UART();
    int send_to_socket();
    int send_to_fakeUART();

    // String reception
    // TODO tags inside the string for multiple emitors
    string received;

    sigc::connection read_timeout;
    bool read_from_serial();
    Glib::Dispatcher* sig_received;
    CommunicationView* m_CommunicationView;


};

#endif
