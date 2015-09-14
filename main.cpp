#include <stdio.h>
#include <pthread.h>

#include "init_gtk.h"
#include "UART.h"

int main(int argc, char const *argv[]) {

    pthread_t thread_RX;

    int ret = pthread_create(&thread_RX, NULL, fake_RX, NULL);
    if (ret != 0)
        fprintf(stderr, "erreur %d\n", ret);

    init_simulation();

    pause();

    return 0;
}
