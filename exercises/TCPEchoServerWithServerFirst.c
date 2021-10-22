#include "Practical.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    DieWithUserMessage("Parameter(s)", "<Server Port>");
  }
  in_port_t servPort = atoi(argv[1]);
  int servSock;
  if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    DieWithSystemMessage("socket() failed");
  }
  struct sockaddr_in servAddr = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = htonl(INADDR_ANY),
      .sin_port = htons(servPort),
  };
  if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    DieWithSystemMessage("bind() failed");
  }
  if (listen(servSock, 5) < 0) {
    DieWithSystemMessage("listen() failed");
  }
  while (1) {
    struct sockaddr_in clntAddr;
    socklen_t clntAddrLen = sizeof(clntAddr);
    int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntAddrLen);
    if (clntSock < 0) {
      DieWithSystemMessage("accept() failed");
    }
    char clntName[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
                  sizeof(clntName)) == NULL) {
      DieWithUserMessage("inet_ntop() failed", "unable to get client address");
    }
    fprintf(stdout, "Handling client %s/%d\n", clntName,
            ntohs(clntAddr.sin_port));
    char buffer[BUFSIZE];
    snprintf(buffer, BUFSIZE, "Welcome, %s!\n", clntName);
    ssize_t numBytesSent = send(clntSock, buffer, BUFSIZE, 0);
    if (numBytesSent < 0) {
      DieWithSystemMessage("send() failed");
    } else if (numBytesSent != sizeof(buffer)) {
      DieWithUserMessage("send() failed", "sent unexpected number of bytes");
    }
    memset(buffer, 0, sizeof(buffer));
    ssize_t numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);
    if (numBytesRcvd < 0) {
      DieWithSystemMessage("recv() failed");
    }
    while (numBytesRcvd > 0) {
      numBytesSent = send(clntSock, buffer, numBytesRcvd, 0);
      if (numBytesSent < 0) {
        DieWithSystemMessage("send() failed");
      } else if (numBytesSent != numBytesRcvd) {
        DieWithUserMessage("send() failed", "sent unexpected number of bytes");
      }
      numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);
      if (numBytesRcvd < 0) {
        DieWithSystemMessage("recv() failed");
      }
      fprintf(stdout, "%s\n", buffer);
    }
    close(clntSock);
  }
  return 0;
}
