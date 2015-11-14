#include <vector>
#include <string>

#include "../libserial/SerialStream.h"


using namespace std;
using namespace LibSerial;

class SerialComm {
public:
    SerialComm() { }
    static vector<string> list_open_ports();

    void connect(string port);
    void send();


protected:
    string connected_port;
    SerialStream serial_port ;

    static vector<string> open_ports;
    static string get_driver(const string& tty);
    static void register_comport(const string& dir);


    // String send
    string to_send;
    void send_to_UART();
    void send_to_socket();
    void send_to_fakeUART();


};

