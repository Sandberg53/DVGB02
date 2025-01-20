#include "client.h"

int main(int argc, char **argv){
    if(argc != 2){
		printf("Usage: %s <port>", argv[0]);
		exit(0);
	}
	
	int port = atoi(argv[1]);
	int sockfd;
	char* ip = LOCALHOST;
	struct sockaddr_in serverAddr;
	time_t buffer;
	uint32_t t;
	socklen_t addr_size;
	char empty[10];
	memset(empty, '\0', 0);
	sockfd=socket(PF_INET, SOCK_DGRAM, 0);
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(port);
    inet_pton(AF_INET, ip ,&serverAddr.sin_addr);
	while(1){
 	sendto(sockfd, &empty, 0, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	
	addr_size=sizeof(serverAddr);
	recvfrom(sockfd, &t, 32, 0, (struct sockaddr*)&serverAddr, &addr_size);
	
	buffer = ntohl(t);
	buffer = buffer - UNIX_EPOCH;
	char* s = calloc(128, 1);
	struct tm *info;
	info = localtime(&buffer);
	
	strftime(s, 32, "%c", info);
	
	fprintf(stderr, "Localtime: %s", s);
	close(sockfd);
	break;
	

	
}
}