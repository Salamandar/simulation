int arret();
int UART_getc(unsigned char *c);
void UART_send_message(char *msg, unsigned int nb_char);
void * fake_RX(void*);
void init_UART_thread();
