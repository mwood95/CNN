/********************************************************************************************
* Name: 	Michael Wood
* Date: 	1/15/17
* File: 	UDPclient.c
* Description:	This file wil contain the program that will run the UDP client code
* 		that will be sending matrix data to the FPGA.
*
********************************************************************************************/

#include <stdio.h>		// printf()
#include <string.h>		// memset
#include <stdlib.h>		// exit(0)
#include <arpa/inet.h>
#include <sys/socket.h>

#define DESTINATION 	"192.168.1.10"		// FPGA IP Address
#define SOURCE		"192.168.1.15"		// PC IP Address
#define BUF_LEN		512			// Maximum Buffer Length
#define PORT		1900			// Target Port  

void die(char *s)
{
	perror(s);
	exit(1);
}

int main()
{
	
	printf("\n\nSource IP: %s\nDestination IP: %s\nPort: %d\n\n", SOURCE, DESTINATION, PORT);

	int sockfd;
	struct sockaddr_in LocalAddr;
	struct sockaddr_in RemoteAddr;
	char buffer[1024];
	char RxBuffer[1024];
	socklen_t addr_size;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&LocalAddr, '\0', sizeof(LocalAddr));
	memset(&RemoteAddr, '\0', sizeof(RemoteAddr));


	// Bind to a specific Local IP
	LocalAddr.sin_family=AF_INET;
	LocalAddr.sin_port=htons(PORT);
	LocalAddr.sin_addr.s_addr=INADDR_ANY;
	bind(sockfd, (struct sockaddr *)&LocalAddr, sizeof(LocalAddr));

	// Connect to a specific Remote IP
	RemoteAddr.sin_family=AF_INET;
	RemoteAddr.sin_port=htons(PORT);
	RemoteAddr.sin_addr.s_addr=inet_addr(DESTINATION);
	connect(sockfd, (struct sockaddr *)&RemoteAddr, sizeof(RemoteAddr));


	printf("Enter a message to send to the FPGA: ");
	bzero(buffer, 1024);
	fgets(buffer, 1024, stdin);

	sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&RemoteAddr, sizeof(RemoteAddr));
	printf("[+] Data Send: %s\n", buffer);
	while(1)
	{
		if(recvfrom(sockfd, RxBuffer, 1024, 0, (struct sockaddr *)&LocalAddr, sizeof(LocalAddr)) > 0)
		{
			printf("[+] Data Received: %s\n", RxBuffer);
			return 0;
		}
	}

	
	return 0;


}
