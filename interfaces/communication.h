#include <vector>
#include <string>


using namespace std;


class SerialComm {
public:
    SerialComm() { }
    static vector<string> list_open_ports();

    void connect(string port);
    void send();


protected:
    string connected_port;

    static vector<string> open_ports;
    static string get_driver(const string& tty);
    static void register_comport(const string& dir);


};

