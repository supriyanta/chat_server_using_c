#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 5001;
const int BACKLOG_QUEUE_SIZE = 8;

void errorHandler(const char *msg)
{
  printf("%s", msg);
  exit(0);
}

void handleClient(int clientConnectionSocket, struct sockaddr_in *clientAddress)
{
  // Details of Client connected to
  printf("Server connected to %s on port %hu\n", inet_ntoa(clientAddress->sin_addr), ntohs(clientAddress->sin_port));

  char buffer[256];
  // clearing buffer
  bzero(buffer, sizeof buffer);

  // Receiving data from Client and storing in buffer
  recv(clientConnectionSocket, buffer, sizeof buffer, 0);

  // displaying data
  printf("Client: %s", buffer);
}

int main(int argc, char *argv[])
{
  // Initializing Socket for server
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (serverSocket == -1)
  {
    errorHandler("error creating Server socket");
  }

  // setting up structure for Socket Address which holds the info network address, which port to connect etc.
  struct sockaddr_in serverAddress;
  // clearing bytes
  bzero(&serverAddress, sizeof serverAddress);
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  // Binding Server Address with Server Socket
  if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof serverAddress) == -1)
  {
    errorHandler("error binding server socket");
  }

  // Listening for new Client Connection
  if (listen(serverSocket, BACKLOG_QUEUE_SIZE) == -1)
  {
    errorHandler("error listening for client");
  }

  // for accepting Client Address
  struct sockaddr_in clientAddress;
  socklen_t clientAddressLength = sizeof clientAddress;

  while (1)
  {
    // Waiting for accepting Client Connection
    int connectionFd = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);

    if (connectionFd == -1)
    {
      errorHandler("error accepting new client");
    }
    handleClient(connectionFd, &clientAddress);

    close(connectionFd);
  }

  // closing the socket
  close(serverSocket);

  return 0;
}