#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#define SERVER_PORT 1234
#define BUF_SIZE 2048

struct socket{  int socketfd;};
typedef struct{
char *ext;
char *mediatype;
}extn;

extn extentions[] = {
{"gif", "image/gif"},
{"jpg", "image/jpg"},
{"jpeg", "image/jpeg"},
{"png", "image/png"},
{"txt", "text/plain"},
{"htm", "text/htm"},
{"html", "text/html"},
{"ico", "image/ico"},
{"0", "0"}};

void Init_Chanel(struct sockaddr_in *channel);
void Create_Socket();
void Wait_Client();
char GetFile(int *sa, char *method, char *uri, char *version);
char* CheckType(char* uri);
char* GetSize(int fd);
char* Response(char* type, char* version, char* StatusCode, int fd);
void fatal(const char *msg, int s);
void Sigint(int sig, siginfo_t *info, void *ucontext);