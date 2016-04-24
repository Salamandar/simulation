#include "ForkAvecBus.hpp"

#include <cstring>
#include <iostream>

void exit_on_parent_quit(int status) {
    std::cout << "quit" << std::endl;
    kill(0, 2);
}

ForkAvecBus::ForkAvecBus(std::string ExecName)
: ExecName(ExecName),
  child_pid(-1) {
    pipe(child_stdin);
    pipe(child_stdout);
}

ForkAvecBus::~ForkAvecBus(){
    std::cout << "kill process" << std::endl;
    kill(child_pid, SIGKILL);
}

bool ForkAvecBus::start() {
    child_pid = fork();

    if(child_pid == -1)
        return true;

    if(child_pid == 0) {
        /* Child process closes up input side of pipe */
        close(child_stdin[WRITE]);
        dup2 (child_stdin[READ], STDIN_FILENO);

        close(child_stdout[READ]);
        dup2 (child_stdout[WRITE], STDOUT_FILENO);
        close(child_stdout[WRITE]);

        execl("/bin/sh", "/bin/sh", "-c", ExecName.c_str(), NULL);
        exit(0);

    } else {
        /* Parent process closes up output side of pipe */
        close(child_stdin[READ]);
        close(child_stdout[WRITE]);

        signal(SIGTERM,exit_on_parent_quit);
    }
}


void ForkAvecBus::send(char* message) {
    write(child_stdin[WRITE], message, strlen(message));
}

void ForkAvecBus::readFromChild() {
    FILE* fp = fdopen(child_stdout[READ], "r");
    // Tests
    int bytes_read = -1;
    char readbuffer[1000];
    while (1) {
        //char c=getc(fp);
        //std::cout << "From child: <<" << c << ">>" << std::endl;



        bytes_read = read(child_stdout[READ], readbuffer, 999);
        if (bytes_read <= 0)
            break;
        readbuffer[bytes_read] = '\0';
        std::cout << "From child: <<" << readbuffer << ">>" << std::endl;



        // receive_output += readbuffer;
    }
}
