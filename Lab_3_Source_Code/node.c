#include "node.h"

#include <string.h>  // For memset

void rtinit(struct distance_table *table, int node) {
    printdt(table, node);

    for (int neighbor = 0; neighbor < 4; neighbor++) { // Start from 0
        if (is_neighbor(node, neighbor)) {
            struct rtpkt pkt;
            memset(&pkt, 0, sizeof(pkt)); // Ensure pkt is zeroed out

            pkt.sourceid = node;
            pkt.destid = neighbor;
            for (int i = 0; i < 4; i++) {
                pkt.mincost[i] = table->costs[i][node];  // Initialize mincosts
            }

            tolayer2(pkt);
        }
    }
}


void rtupdate(struct distance_table *table, int node, struct rtpkt *pkt) {
    int src = pkt->sourceid;  // Neighbor sending the update
    int updated = 0;          // Flag to check if changes occur

    for (int i = 0; i < 4; i++) {  // Loop through all nodes
        int new_cost = table->costs[node][src] + pkt->mincost[i];  // Bellman-Ford update rule
        if (new_cost < table->costs[i][node]) {  // Check if the new path is better
            table->costs[i][node] = new_cost;
            updated = 1;
        }
    }

    printdt(table, node);  // Print table after update

    // If table changed, send new routing updates to neighbors
    if (updated) {
        struct rtpkt new_pkt;
        new_pkt.sourceid = node;

        for (int i = 0; i < 4; i++) {
            new_pkt.mincost[i] = table->costs[i][node];  // Copy updated min costs
        }

        // Send to all neighbors
        for (int dest = 0; dest < 4; dest++) {
            if (is_neighbor(node, dest)) {
                new_pkt.destid = dest;
                tolayer2(new_pkt);
            }
        }
		
    }
}
