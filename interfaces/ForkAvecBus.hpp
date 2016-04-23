#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>


#define READ   0
#define WRITE  1


class ForkAvecBus {
public:
    ForkAvecBus(std::string ExecName);
   ~ForkAvecBus();

    bool start();
    void send(char* message);


protected:
    std::string ExecName;


private:
    int     child_stdin[2];
    int     child_stdout[2];
    pid_t   child_pid;


};
