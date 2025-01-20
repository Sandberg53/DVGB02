#include "udp.h"

void main(){
	int port = 1234;
	int sockfd;
	int nbytes = 0;
	uint32_t t;
	struct sockaddr_in si_me, si_other;
	time_t buffer;
	socklen_t addr_size;

	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&si_me, '\0', sizeof(si_me));
	si_me.sin_family=AF_INET;
	si_me.sin_port=htons(port);
	si_me.sin_addr.s_addr = INADDR_ANY;


	if(bind(sockfd, (struct sockaddr*)&si_me, sizeof(si_me)) != 0)
		fprintf(stderr, "Error binding socket");
	addr_size=sizeof(si_other);
	
	while(1){
 	nbytes = recvfrom(sockfd, &buffer, 1024, 0, (struct sockaddr*)&si_other, &addr_size);	//client connected
 	if(nbytes < 0)
 	{
 		fprintf(stderr, "fail");
 		exit(0);
 	}
 	

 	t = time(NULL) + UNIX_EPOCH;
 	t = htonl(t);
        sendto(sockfd, &t, sizeof(t), 0, (struct sockaddr*)&si_other, addr_size);
	fprintf(stderr, "[+]Data send: %d\n", t);
	
}}