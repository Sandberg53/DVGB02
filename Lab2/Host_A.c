#include "Sim_Engine.h"
#include "Host_A.h"
#include <string.h>

int A_seqnum = 0;
int A_waiting_for_ack = 0;
struct pkt A_last_packet;
int compute_checksum(struct pkt packet) {
    int checksum = packet.seqnum + packet.acknum;
    for (int i = 0; i < 20; i++) {
        checksum += packet.payload[i];
    }
    return checksum;
}

void A_output(struct msg message) {
    if (A_waiting_for_ack) return;  // Only send if not waiting for ACK
    
    struct pkt sendpkt;
    sendpkt.seqnum = A_seqnum;
    sendpkt.acknum = 0;
    strncpy(sendpkt.payload, message.data, 20);
    sendpkt.checksum = compute_checksum(sendpkt);
    
    A_last_packet = sendpkt;
    A_waiting_for_ack = 1;
    
    tolayer3(A, sendpkt);
    starttimer(A, TIMEOUT);
}

void A_input(struct pkt packet) {
    int checksum = compute_checksum(packet);
    if (packet.checksum != compute_checksum(packet)) {
    printf("A received corrupted ACK, ignoring...\n");
    tolayer3(0, A_last_packet); // Retransmit immediately
    return;
}

    if (packet.acknum == A_seqnum) {
        stoptimer(A);
        A_seqnum = 1 - A_seqnum;  // Toggle sequence number
        A_waiting_for_ack = 0;
    }
}

void A_timerinterrupt() {
    tolayer3(A, A_last_packet);
    starttimer(A, TIMEOUT);
}

void A_init() {
    A_seqnum = 0;
    A_waiting_for_ack = 0;
}

