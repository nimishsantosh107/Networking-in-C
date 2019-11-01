#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SA struct sockaddr 
#define SERVER_PORT 4000
#define MAX 256

int error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char const *argv[])
{
	int server_fd, client_fd;
	struct sockaddr_in servaddr, clientaddr;
	int n;
	char buf[MAX];	

	//CREATE SOCKET
	if ((server_fd = socket(PF_INET,SOCK_STREAM,0)) <= 0) error("SOCKET FAILED");

	//RESET servaddr / ASSIGN IP, PORT, FAMILY 
	bzero(&servaddr, sizeof(servaddr)); 
	if ((inet_aton("127.0.0.1", &servaddr.sin_addr)) == 0) error("IP ERROR");
	servaddr.sin_port = htons(SERVER_PORT); 
	servaddr.sin_family = PF_INET; 

    //BIND TO PORT
	if ((bind(server_fd, (SA*)&servaddr, sizeof(servaddr))) != 0) error("BIND ERROR");

    //LISTEN ON PORT
	if ((listen(server_fd, 5)) != 0) error("LISTEN ERROR");
	printf("SERVER LISTENING ON PORT 4000\n\n");

    //ACCEPT CONNECTION
	unsigned int len = sizeof(clientaddr);
	if ((client_fd = accept(server_fd, (SA*)&clientaddr, &len)) <= 0) error("ACCEPT ERROR"); 

    //CLIENT
    bzero(buf, MAX);
	read(client_fd ,buf, sizeof(buf));
	printf("CLIENT: %s", buf);
	bzero(buf, MAX);
	//SERVER(THIS)
	printf("YOU: ");
	n=0;
	while ((buf[n++] = getchar()) != '\n')
		;
	write(client_fd, buf, sizeof(buf));
	bzero(buf, MAX);

	//CLOSE SOCKET
	close(server_fd);
	return 0;
}