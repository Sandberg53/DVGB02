#include "tcp.h"
int main(int argc, char * argv[]){
	struct sigaction sa;
	struct socket s;
	sa.sa_sigaction = Sigint;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	if(sigaction(SIGINT, &sa, NULL) == -1){
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
    struct sockaddr_in channel;
    memset(&channel, 0, sizeof(channel));

	
    Init_Chanel(&channel);      /*Socket initialized*/
    Create_Socket(&s.socketfd, &channel);     /*Socket created*/
    Wait_Client(&s.socketfd);
    
    close(s.socketfd);
    return 0;
}

void Init_Chanel(struct sockaddr_in *channel){
    channel->sin_family = AF_INET;
    channel->sin_port = htons(SERVER_PORT);
    channel->sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(channel->sin_zero, 0, sizeof(channel->sin_zero));
    printf("Socket init: \nIP:\t\t%s\nPort: \t\t%d\n", inet_ntoa(channel->sin_addr), ntohs(channel->sin_port));
}

void Create_Socket(int *socketfd, struct sockaddr_in *channel){
    *socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if(*socketfd < 0){perror("Socket creation failed"); exit(EXIT_FAILURE);}
    if(bind(*socketfd, (struct sockaddr *)channel, sizeof(*channel)) < 0){perror("Bind faied"); close(*socketfd); exit(EXIT_FAILURE);}
    if(listen(*socketfd, 5) < 0){perror("Listen failed"); close(*socketfd); exit(EXIT_FAILURE);}
    fprintf(stderr, "Socket & bind successfull, listening for connections\n");
}

void Wait_Client(int *socketfd){
    int socket_Accept;
    char buf[BUF_SIZE];
    while(1){
        socket_Accept = accept(*socketfd, NULL, NULL);
        if(socket_Accept < 0)  fatal("accept failed", socket_Accept);
        size_t n = read(socket_Accept, buf, BUF_SIZE);
        if(n < 0)   fatal("Read failed", socket_Accept);
        buf[n] = '\0';
        char method[5], uri[100], version[9], StatusCode[20];
        sscanf(buf, "%s %s %s", method, uri, version);
		memmove(uri, uri +1, strlen(uri));
		fprintf(stderr, "%s\t%s\t%s\t", method, uri, version);
		version[8] = '\0';
		
		char* type = CheckType(uri);
		
		int fd = open(uri, O_RDONLY);	

		if (fd < 0) {
			fprintf(stderr, "Open failed\n");//fatal("open failed", socket_Accept);
			strcpy(StatusCode, " 404 File Not Found");
			write(socket_Accept, StatusCode, sizeof(StatusCode)-1);
		}
		else{
			strcpy(StatusCode, " 200 OK");

		
		char resp[2048];

		strcpy(resp, Response(type, version, StatusCode, fd));
		write(socket_Accept, resp, strlen(resp));
		
		char x[2048];
		memset(x, '\0', 2047);
		memset(buf, '\0', 2048);
		
		int sz;
		sz = read(fd, buf, 2048);
		while(sz > 0) {
			send(socket_Accept, buf, sz, 0);
			sz = read(fd, buf, 2048);
		}

	}
		close(fd); /* close file */
		close(socket_Accept); /* close connection */
        
        
    }
}

char* CheckType(char* uri)
{
	char* s = strchr(uri, '.');
	for(int i = 0; extentions[i].ext != NULL; i++)
	{
		if(strcmp(s + 1, extentions[i].ext) == 0)
		{
			return extentions[i].mediatype;
		}
	}
}

char* GetSize(int fd)
{
	off_t currentPos = lseek(fd, 0L, SEEK_CUR);
	off_t endPos = lseek(fd, 0L, SEEK_END);
	int res = endPos - currentPos;
	char* l = malloc(24);
	sprintf(l, "%d", res);
	lseek(fd, currentPos, SEEK_SET);
	return l;//"350000";
}
char* Response(char* type, char* version, char* StatusCode, int fd)
{
	
	fprintf(stderr, " Type: %s\n\n\n", type);
	char* resp = calloc(2048, sizeof(char));
	strcpy(resp, version);
	strcat(resp, StatusCode);
	strcat(resp, "\r\nServer: Demo Web Server\r\n");
	strcat(resp, "Content-Length: ");
	strcat(resp, GetSize(fd));
	strcat(resp, "\r\nContent-Type:");
	strcat(resp, type);
	strcat(resp, "\r\n\r\n");
	fprintf(stderr, "%s", resp);
	return resp;	
}

void fatal(const char *msg, int s) {
    perror(msg);
    close(s);
    exit(EXIT_FAILURE);
}

void Sigint(int sig, siginfo_t *info, void *ucontext){
	if(info && info->si_value.sival_ptr){
		struct socket *s = (struct socket *)info->si_value.sival_ptr;
		close(s->socketfd);
		perror("Exit signal cought, closing socket");
	}
	else
		perror("Exit signal cought");
	exit(EXIT_FAILURE);
}