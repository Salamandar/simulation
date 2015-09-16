#include "asservissement.h"
#include "UART.h"
#include <stdio.h>
#include <pthread.h>

#define RX_BUFFER_SIZE 40
static unsigned char rxBuffer[RX_BUFFER_SIZE];
static unsigned short rxBufferDebut=0;
static unsigned short rxBufferFin=0;

int cmd_quit_received=0;

int arret()
{
    return cmd_quit_received;
}

void * fake_RX(void*)
{
    while(!arret()) {
        // On lit l'entrée standard pour simuler une reception sur l'UART
        // Si on reçoit "q/n" on quitte le programme
        char c=getc(stdin);
        if(c=='q')
        {
            char d=getc(stdin);
            if (d=='\n' || d=='\r' || d=='\0')
            {
                cmd_quit_received=1;
            }
            else
            {
                rxBuffer[rxBufferFin] = c;
                rxBuffer[rxBufferFin+1] = d;
                rxBufferFin = (rxBufferFin + 2) % RX_BUFFER_SIZE;
            }
        }
        else
        {
            rxBuffer[rxBufferFin] = c;
            rxBufferFin = (rxBufferFin + 1) % RX_BUFFER_SIZE;
        }
    }
    return NULL;
}

int UART_getc(unsigned char *c)
{
    if (rxBufferDebut == rxBufferFin) {
        // Il n'y avait pas de caractères en attente
        return 0;
    } else {
        // Il y des caractères à traiter
        *c = rxBuffer[rxBufferDebut];
        rxBufferDebut = (rxBufferDebut + 1) % RX_BUFFER_SIZE;
        return 1;
    }
}


void init_UART_thread() {
    pthread_t thread_RX;

    int ret = pthread_create(&thread_RX, NULL, fake_RX, NULL);
    if (ret != 0)
        fprintf(stderr, "erreur %d\n", ret);
}
/*
void UART_send_message(char* message) {
    char *actuel = message;
    while (*actuel)
        debug_byte(0,  *actuel++);
    debug_byte(0,'\0');
}
*/

void UART_send_message(char *msg, unsigned int nb_char)
{
    (void) nb_char; // pour éviter un warning inutile
    puts(msg);
}

