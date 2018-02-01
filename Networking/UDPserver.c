#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4455
void main()
{

	int port = PORT;
	int sockfd;
	struct sockaddr_in FPGA;
	char buffer[1024];
	socklen_t addr_size;

	printf("Remote Address: %s\nPort: %dn", "169.254.179.192", PORT);

	sockfd=socket(AF_INET, SOCK_DGRAM, 0);
	// Fill Structure with 0s
	memset(&FPGA, '\0', sizeof(FPGA));
	
	FPGA.sin_family=AF_INET;
	FPGA.sin_port=htons(port);
	FPGA.sin_addr.s_addr=inet_addr("169.254.179.192");

	if (bind(sockfd, (struct sockaddr*)&FPGA, sizeof(FPGA)) != 0)
	{
		printf("Bind Failed...\n");
		exit(0);
	}
	
	addr_size = sizeof(FPGA);
	recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&FPGA, &addr_size);
	printf("[+] Data Recieved %s\n", buffer);
}
