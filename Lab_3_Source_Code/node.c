#include "node.h"


void rtinit(struct distance_table *table, int node) {
    sendPKT(table, node);
    printdt(table, node);
}


void rtupdate(struct distance_table *table, int node, struct rtpkt *pkt) {
    int src = node;
    int via = pkt->sourceid; 
    int updated = 0;         


    for (int i = 0; i < 4; i++) {  
        if (table->costs[via][i] > pkt->mincost[i]) {  
            table->costs[via][i] = pkt->mincost[i];
            updated = 1;            
        }
    }

    if (updated) {
        sendPKT(table, node);
    }
        printdt(table, node); 
}


void sendPKT(struct distance_table *table, int node){
    int src = node;
    struct rtpkt new_pkt;
    new_pkt.sourceid = node;

    for(int i = 0; i < 4; i++){
        int temp = INF;
        for(int j=0; j<4; j++){
            int cost;
            if(j == node){cost = table->costs[node][i];}
            else{cost = table->costs[node][j] + table->costs[j][i];}

            if(cost < temp){temp = cost;}
        }
        new_pkt.mincost[i] = temp;
    }
    for(int i=0; i<4; i++){
        if(is_neighbor(src, i)){
            new_pkt.destid = i;
            tolayer2(new_pkt);
        }
    }
}