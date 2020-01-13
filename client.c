#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

void errorHandler(const char *msg)
{
  printf("%s", msg);
  exit(0);
}

int main(int argc, char *argv[])
{
  // getting port from arguments
  const int PORT = atoi(argv[2]);

  // Initializing Client Socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (clientSocket == -1)
  {
    errorHandler("error creating Client socket");
  }

  // Getting server info
  struct hostent *server = gethostbyname(argv[1]);

  if (server == NULL)
  {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }

  // Setting up Server info from server
  struct sockaddr_in serverAddress;
  // clearing bytes
  bzero(&serverAddress, sizeof serverAddress);

  bcopy((char *)server->h_addr_list[0], (char *)&serverAddress.sin_addr.s_addr, server->h_length);
  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_family = AF_INET;

  // Connecting to Server
  if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof serverAddress) == -1)
  {
    errorHandler("error connectiing to server");
  }

  // Details of Server
  printf("Conected to Server %s on port %hu\n", inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port));

  char buffer[256];
  // clearing buffer for sending data
  bzero(buffer, sizeof buffer);
  // Taking input from user
  fgets(buffer, sizeof buffer, stdin);

  // Sending data to Server
  if (send(clientSocket, buffer, sizeof buffer, 0) == -1)
  {
    errorHandler("sending failed");
  }

  // Closing the socket
  close(clientSocket);

  return 0;
}