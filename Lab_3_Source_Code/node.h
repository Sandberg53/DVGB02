#ifndef NODE_H
#define NODE_H

#include "sim_engine.h"
#include <stdio.h>
#include <string.h>

void rtinit(struct distance_table *table, int nodeno);
void rtupdate(struct distance_table *table, int nodeno, struct rtpkt *pkt);
void printdt(struct distance_table *table, int nodeno);
void sendPKT(struct distance_table *table, int node);

#endif
