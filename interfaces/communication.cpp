#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

#include "communication.h"

#include "../communication_view.h"

extern "C" {
    void append_to_UART(unsigned char c);
}

using namespace std;
using namespace LibSerial;

vector<string> SerialComm::open_ports;

string SerialComm::get_driver(const string& tty) {
    struct stat st;
    string devicedir = tty;

    // Append '/device' to the tty-path
    devicedir += "/device";

    // Stat the devicedir and handle it if it is a symlink
    if (lstat(devicedir.c_str(), &st)==0 && S_ISLNK(st.st_mode)) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        // Append '/driver' and return basename of the target
        devicedir += "/driver";

        if (readlink(devicedir.c_str(), buffer, sizeof(buffer)) > 0)
            return basename(buffer);
    }
    return "";
}

void SerialComm::register_comport(const string& dir) {
    // Get the driver the device is using
    string driver = get_driver(dir);

    // Skip devices without a driver
    if (driver.size() > 0) {
        string devfile = string("/dev/") + basename(dir.c_str());

        // Put serial8250-devices in a seperate list
        if (driver != "serial8250")
            open_ports.push_back(devfile);
    }
}

vector<string> SerialComm::list_open_ports() {
    open_ports.clear();
    int n;
    struct dirent **namelist;
    const char* sysdir = "/sys/class/tty/";

    // Scan through /sys/class/tty - it contains all tty-devices in the system
    n = scandir(sysdir, &namelist, NULL, NULL);
    if (n < 0)
        perror("scandir");
    else {
        while (n--) {
            if (strcmp(namelist[n]->d_name,"..") && strcmp(namelist[n]->d_name,".")) {

                // Construct full absolute file path
                string devicedir = sysdir;
                devicedir += namelist[n]->d_name;

                // Register the device
                register_comport(devicedir);
            }
            free(namelist[n]);
        }
        free(namelist);
    }

    // Return the lsit of detected comports
    return open_ports;
}

int SerialComm::connect(string port) {
    if (serial_port.IsOpen())
        serial_port.Close();


    serial_port.Open(port);
    if (!serial_port.good()) {
        std::cerr << "Error: Could not open serial port " << port << std::endl;
        return -1;
    }

    // Set the baud rate of the serial port.
    serial_port.SetBaudRate(SerialStreamBuf::BAUD_115200);
    if (! serial_port.good()) {
        std::cerr << "Error: Could not set the baud rate." << std::endl ;
        return -1;
    }

    // Set the number of data bits.
    serial_port.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
    if (!serial_port.good()) {
        std::cerr << "Error: Could not set the character size." << std::endl ;
        return -1;
    }

    // Disable parity.
    serial_port.SetParity(SerialStreamBuf::PARITY_NONE);
    if (!serial_port.good()) {
        std::cerr << "Error: Could not disable the parity." << std::endl ;
        return -1;
    }

    // Set the number of stop bits.
    serial_port.SetNumOfStopBits(1);
    if (!serial_port.good()) {
        std::cerr << "Error: Could not set the number of stop bits." << std::endl ;
        return -1;
    }

    // Turn on hardware flow control.
    serial_port.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
    if (!serial_port.good()) {
        std::cerr << "Error: Could not use hardware flow control." << std::endl ;
        return -1;
    }

    read_timeout = Glib::signal_timeout().connect(
        sigc::mem_fun(*this, &SerialComm::read_from_serial),
        40);

    return 0;
}


// TODO une seule boucle for avec des appels dedans ?

int SerialComm::send(string texte){
    to_send = texte;

    bool result = true;
    if (to_UART)
        result&=send_to_UART();
    if (to_socket)
        result&=send_to_socket();
    if (to_fakeUART)
        result&=send_to_fakeUART();
    to_send.clear();
    return result;
}

int SerialComm::send_to_UART(){
    for(char& c : to_send)
        serial_port.write(&c, 1);
    char c = '\n';
    serial_port.write(&c, 1);
    return 0;
}
int SerialComm::send_to_socket(){
    return 0;

}
int SerialComm::send_to_fakeUART(){
    for(char& c : to_send)
        append_to_UART(c);
    append_to_UART('\n');
    return 0;
}


bool SerialComm::read_from_serial(){
    while(serial_port.rdbuf()->in_avail() > 0) {
        char next_byte;
        serial_port.get(next_byte);
        received +=     next_byte;
        std::cout << next_byte;
        usleep(10);
    }
    std::cout << std::flush;
    if (received.size()>0) {
        m_CommunicationView->receive_string(received);
        received.clear();
    }

    return true;
}
