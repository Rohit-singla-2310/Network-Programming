/*
Made By : Rohit Singla
Cmd line arguments: Server_IP_Address port_number [range (1025-65500)]
Ends: when server type "bye" text
*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void error(const char *msg){
	perror(msg);
	exit(1);
}
void program( int sockfd, struct sockaddr_in serv_addr){
	int n, len;
	char buffer[255];
	len = sizeof(serv_addr);
	while(1){
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = sendto(sockfd, buffer, strlen(buffer), 
        	MSG_CONFIRM, (const struct sockaddr *) &serv_addr,len);
		if(n < 0)
			error("Error on writing!\n");
		bzero(buffer, 255);
		
        	n = recvfrom(sockfd, (char *)buffer, 255, 
                MSG_WAITALL, ( struct sockaddr *) &serv_addr,
                &len);
		if( n < 0)
			error("Error on reading!\n");
		printf("Server : %s>> ", buffer);
		int t = strncmp("Bye", buffer, 3);
		if(t == 0)
			break;
	}
}

int main(int argc, char * argv[]){
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	if(argc < 3){
		fprintf(stderr, "usage %s hostname port. \n",argv[0]);
		exit(1);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
		error("Error in opening socket!\n");
	server = gethostbyname(argv[1]);
	if(server == NULL)
		fprintf(stderr, "Error, no such host");
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family  = AF_INET;
	bcopy((char*) server->h_addr, (char*) & serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	program(sockfd, serv_addr);
	close(sockfd);
	return 0;
}


