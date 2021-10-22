#include "Practical.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

// Maximum outstanding connection requests
static const int MAXPENDING = 5;

int main(int argc, char *argv[]) {
  // Test for correct number of arguments
  if (argc != 2) {
    DieWithUserMessage("Parameter(s)", "<Server Port>");
  }
  // First arg: local port
  in_port_t servPort = atoi(argv[1]);
  // Create socket for incoming connections
  int servSock;
  if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    DieWithSystemMessage("socket() failed");
  }
  // Construct local address structure
  // Local address
  struct sockaddr_in servAddr;
  // Zero out structure
  memset(&servAddr, 0, sizeof(servAddr));
  // IPv4 address family
  servAddr.sin_family = AF_INET;
  // Any incoming interface
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  // Local port
  servAddr.sin_port = htons(servPort);

  // Bind to the local address
  if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    DieWithSystemMessage("bind() failed");
  }
  // Mark the socket so it will listen for incoming connections
  if (listen(servSock, MAXPENDING) < 0) {
    DieWithSystemMessage("listen() failed");
  }
  // Run forever
  for (;;) {
    // Client address
    struct sockaddr_in clntAddr;
    // Set length of client address structure (in-out parameter)
    socklen_t clntAddrLne = sizeof(clntAddr);

    // Wait for a client to connect
    int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntAddrLne);
    if (clntSock < 0) {
      DieWithSystemMessage("accept() failed");
    }
    // ClntSock is connected to a client!
    // String to contain client address
    char clntName[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
                  sizeof(clntName)) != NULL) {
      printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
    } else {
      puts("Unable to get client address");
    }
    HandleTCPClient(clntSock);
  }
  // NOT REACHED
}
