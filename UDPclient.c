#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>


int main()
{
	int clientSocket, portNum, nBytes;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	// Create UDP socket
	clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

	// Configure setting in address structure
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Initialize size variable to be used later
	addr_size = sizeof serverAddr;

	while(1)
	{
		printf("Type a something to send to server\n");
		fgets(buffer, 1024, stdin);
		
		nBytes = strlen(buffer) + 1;

		// Send the message to the server
		sendto(clientSocket, buffer, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size);

		// Recieve message from server
		nBytes = recvfrom(clientSocket, buffer, 1024, 0, NULL,NULL);
	}
	return 0;
}
