#include "Sim_Engine.h"
#include "Host_B.h"
#include <string.h>

int B_expected_seqnum = 0;
int compute_checksum_B(struct pkt packet) {
    int checksum = packet.seqnum + packet.acknum;
    for (int i = 0; i < 20; i++) {
        checksum += packet.payload[i];
    }
    return checksum;
}
/* Called from layer 5, passed the data to be sent to other side */
void B_output( struct msg message) {
  /* DON'T IMPLEMENT */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void B_input(struct pkt packet) {
    int checksum = compute_checksum_B(packet);
    struct pkt ackpkt;
    
    if (checksum == packet.checksum && packet.seqnum == B_expected_seqnum) {
        tolayer5(B, packet.payload);
        ackpkt.acknum = B_expected_seqnum;
        B_expected_seqnum = 1 - B_expected_seqnum;
    } else {
        ackpkt.acknum = 1 - B_expected_seqnum;
    }
    
    ackpkt.seqnum = 0;
    memset(ackpkt.payload, 0, 20);
    ackpkt.checksum = compute_checksum_B(ackpkt);
    
    tolayer3(B, ackpkt);
}

/* Called when B's timer goes off */
void B_timerinterrupt() {
  /* TODO */
}  

/* The following routine will be called once (only) before any other */
/* Host B routines are called. You can use it to do any initialization */
void B_init() {
    B_expected_seqnum = 0;
}
