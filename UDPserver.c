#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define COMPUTER_IP "169.254.179.192"
#define WIFI_IP		"127.0.0.1"
#define PORT 4455
void main()
{

	int port = PORT;
	int sockfd;
	struct sockaddr_in Computer, FPGA;
	char buffer[1024];
	socklen_t addr_size;

	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	// Fill Structure with 0s
	memset(&Computer, '\0', sizeof(Computer));
	
	Computer.sin_family=AF_INET;
	Computer.sin_port=htons(port);
	Computer.sin_addr.s_addr=inet_addr(COMPUTER_IP);
/*
	FPGA.sin_family=AF_INET;
	FPGA.sin_port=htons(port);
	FPGA.sin_addr.s_addr=inet_addr(INADDR_ANY);
*/
	bind(sockfd, (struct sockaddr*)&Computer, sizeof(Computer));
	addr_size = sizeof(FPGA);
	recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&FPGA, &addr_size);
	printf("[+] Data Recieved %s\n", buffer);
}
