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
#define RX_SIZE		64	

//#define TESTING
#define READ_IMAGE 

struct header{
	uint8_t  NetworkCtl;
	uint8_t  DataCtl;
	uint8_t  Size;
	uint32_t Data;
};

int main()
{

	printf("\n\nSource IP: %s\nDestination IP: %s\nPort: %d\n\n", SOURCE, DESTINATION, PORT);
	FILE *fd;
	char name[1024];
	int buffer[64];
	int i, j;
	struct header packet;

	printf("___________________________________________\n");
	printf("|       Convolutional Neural Network	  |\n");
	printf("|              _---~~(~~-_.        	  |\n");
	printf("|            _{        )   )           	  |\n");
	printf("|         (  `-,_..`., )-- '_,)      	  |\n");
	printf("|        ( ` _)  (  -~( -_ `,  }    	  |\n");
	printf("|        (_-  _  ~_-~~~~`,  ,' )   	  |\n");
	printf("|          `~ -^(    __;-,((()))      	  |\n");
	printf("|               ~~~~ {_ -_(())       	  |\n");
	printf("|                       `|  }       	  |\n");
	printf("|                         { }          	  |\n");
	printf("___________________________________________\n");



	int sockfd;
	struct sockaddr_in LocalAddr;
	struct sockaddr_in RemoteAddr;
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

#ifdef READ_IMAGE
	printf("When you are finished type 'Done'\n");

	while(name != "Done")
	{

		printf("type the name of the file to read from: ");
		printf("\n	Image1\n	Image2\n	.\n	.\n	.\n	Image8\n");
		scanf("%s", &name);
		fd = fopen(name, "r");
		if(fd == NULL)
		{
			printf("Error Opening File\n");
			fclose(fd);
		}
		

		//  Send initialization packet
		//  Byte Definitions in Documentation 
		packet.NetworkCtl = 0xFF;
		packet.DataCtl = 0x01;
		packet.Size = 8;
		fscanf(fd, "%d", &packet.Data);
		//packet.Data = 0;
		sendto(sockfd, (void *)&packet, sizeof(packet), 0, (struct sockaddr *)&RemoteAddr, sizeof(RemoteAddr));

		// Read from image text file configure control bytes and send data
		for(i = 0; i < 8; i++)
		{
			for(j = 0; j < 8; j++)
			{
				packet.NetworkCtl = 0x01;
				packet.DataCtl = 0x01;
				packet.Size = 0;
				fscanf(fd, "%d", &packet.Data);
				sendto(sockfd, (void *)&packet, sizeof(packet), 0, (struct sockaddr*)&RemoteAddr, sizeof(RemoteAddr));
				//printf("[+] Data Send\n");
			}

			printf("\n\n");
		}
		
		// End of transmission send start network packet
		packet.NetworkCtl = 0xFF;
		packet.DataCtl = 0xFF;
		packet.Size = 0;
		packet.Data = 0;
		sendto(sockfd, (void *)&packet, sizeof(packet), 0, (struct sockaddr *)&RemoteAddr, sizeof(RemoteAddr));

		fclose(fd);
	}
#endif


#ifdef TESTING
		int iteration = 0;

		while(1)
		{
			/*

			   printf("Enter Network Control field\n  - 0xFF Network Setup\n  - 0x01 Network Data\n");
			   scanf("%x", &packet.NetworkCtl);
			   printf("Enter Data Control field\n  - 0xFF Start Network\n  - 0x01 Training Data\n  - 0x02 Real Time Analysis\n");
			   scanf("%x", &packet.DataCtl);
			   printf("Enter Image size\n");
			   scanf("%x", &packet.Size);
			   printf("Enter value to be sent\n");
			   scanf("%x", &packet.Data);
			   if(sendto(sockfd, (void *)&packet, sizeof(packet), 0, (struct sockaddr *)&RemoteAddr, sizeof(RemoteAddr)))
			   {
			   printf("[+] Data Sent Successfully\n");

			   }
			   */	

			int i, j; 
			if(iteration == 1)
			{
				packet.NetworkCtl = 0x01;
				packet.DataCtl = 0x01;
				packet.Size = 0;
			}
			if(iteration == 1)
			{
				for(i = 0; i < 8; i++)
				{
					for(j = 0; j < 8; j++)
					{
						if(i == j)
						{
							packet.Data = 1;
						}
						else
						{
							packet.Data = 0;
						}
						if(sendto(sockfd, (void *)&packet, sizeof(packet), 0, (struct sockaddr *)&RemoteAddr, sizeof(RemoteAddr)))
						{
							printf("[+] Data Sent Successfully\n");


						}
					}
				}
			}
			if(iteration == 0)
			{
				packet.NetworkCtl = 0xFF;
				packet.DataCtl = 0x01;
				packet.Size = 8;
				packet.Data = 0;
			}
			if(iteration == 2)
			{
				packet.NetworkCtl = 0xFF;
				packet.DataCtl = 0xFF;
				packet.Size = 0;
				packet.Data = 0;
			}

			if(iteration == 0 | iteration == 2)
			{
				if(sendto(sockfd, (void *)&packet, sizeof(packet), 0, (struct sockaddr *)&RemoteAddr, sizeof(RemoteAddr)))
				{
					printf("[+] Data Sent Successfully\n");

				}
			}
			iteration++;
		}


#endif

		//while(1)
		//{
		//		printf("What would you like to send to the FPGA?");
		//		bzero(buffer, 32);
		//		fgets(buffer, 32, stdin);
		/*		scanf("%d", &buffer);
				if(sendto(sockfd, buffer, 32, 0, (struct sockaddr*)&RemoteAddr, sizeof(RemoteAddr)));
				{
				printf("[+] Data Sent: %d\n", buffer);
				}	
				*/
		//		while(1)
		//		{

		if(recvfrom(sockfd, RxBuffer, 1024, 0, (struct sockaddr *)&LocalAddr, sizeof(LocalAddr)) > 0)
		{
			printf("[+] Data Received: %s\n", RxBuffer);
			return 0;
		}



		return 0;
	}





