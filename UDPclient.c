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
#define SOURCE		"169.254.130.144"	// Raspberru Pi IP Address
#define BUF_LEN		512			// Maximum Buffer Length
#define PORT		4455			// Target Port  

void die(char *s)
{
	perror(s);
	exit(1);
}

int main(char argc, char **argv)
{
	
	if(argc != 2)
	{
		printf("Usage: %s <port> \n", argv[0]);
	}

	printf("\n\nSource IP: %s\nDestination IP: %s\nPort: %d\n\n", SOURCE, DESTINATION, PORT);

	int sockfd;
	struct sockaddr_in Computer;
	struct sockaddr_in FPGA;
	char buffer[1024];
	socklen_t addr_size;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&Computer, '\0', sizeof(Computer));
	memset(&FPGA, '\0', sizeof(FPGA));
	
	Computer.sin_family=AF_INET;
	Computer.sin_port=htons(PORT);
	Computer.sin_addr.s_addr=inet_addr(SOURCE);
	
	FPGA.sin_family=AF_INET;
	FPGA.sin_port=htons(PORT);
	FPGA.sin_addr.s_addr=inet_addr(DESTINATION);




	strcpy(buffer, "Hello Server");
	sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)&FPGA, sizeof(FPGA));
	printf("[+] Data Send: %s\n", buffer);
	return 0;


}
