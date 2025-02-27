#ifndef HOST_A_H
#define HOST_A_H
#define A 0
#define B 1

#define TIMEOUT 20.0 

void A_output(struct msg message);
void A_input(struct pkt packet);
void A_timerinterrupt();
void A_init();

#endif
