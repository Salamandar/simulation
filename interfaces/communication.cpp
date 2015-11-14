#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "communication.h"

extern "C" {
    void append_to_UART(unsigned char c);
}

using namespace std;

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



void SerialComm::send_to_UART(){

}
void SerialComm::send_to_socket(){

}
void SerialComm::send_to_fakeUART(){
    for(char& c : to_send)
        append_to_UART(c);
    append_to_UART('\n');
    to_send.clear();
}
