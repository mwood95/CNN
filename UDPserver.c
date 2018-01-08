#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


// Take these as command lien arguments because port and IP will change 
void main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(0);
	}

	// Create arguments for UDP server
	int port = atoi(argv[1]);
	int sockfd;
	struct sockaddr_in serverAddr, clientAddr;
	char buffer[1024];
	socklen_t addr_size;

	// Create Socket	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	// Configure settings in address structure (for server)	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Bind socket with address struct
	bind(sockfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	addr_size=sizeof(clientAddr);	
	recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&clientAddr, &addr_size);
	printf("[+] Data Recieved: %s\n", buffer);
	
	

}
