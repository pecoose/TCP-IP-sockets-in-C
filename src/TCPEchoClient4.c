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
  // Test for correct number of arguments
  if (argc < 3 || argc > 4) {
    DieWithUserMessage("Parameter(s)",
                       "<Server Adress> <Echo Word> [<Server Port>]");
  }
  // First arg: server IP address (dotted quad)
  char *servIP = argv[1];
  // Second arg: string to echo
  char *echoString = argv[2];
  // Third arg (optional): server port (numeric).
  // 7 is well-known echo port
  in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 7;

  // Create a reliable, stream socket using TCP
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0) {
    DieWithSystemMessage("socket() failed");
  }
  // Construct the server address structure
  // Server Address
  struct sockaddr_in servAddr;
  // Zero out structure
  memset(&servAddr, 0, sizeof(servAddr));
  // IPv4 address family
  servAddr.sin_family = AF_INET;
  // Convert address
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (rtnVal == 0) {
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  } else if (rtnVal < 0) {
    DieWithSystemMessage("inet_pton() failed");
  }
  servAddr.sin_port = htons(servPort);
  // Establish the connection to the echo server
  if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
    DieWithSystemMessage("connect() failed");
  }
  // Determine input length
  size_t echoStringLen = strlen(echoString);
  // Send the string to the server
  ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
  if (numBytes < 0) {
    DieWithSystemMessage("send() failed");
  } else if (numBytes != echoStringLen) {
    DieWithUserMessage("send(), sent unexpected number of bytes");
  }
  // Receive the same string back from the server
  // Count of total bytes received
  unsigned int totalBytesRcvd = 0;
  // Setup to print the echoed string
  fputs("Received: ", stdout);
  while (totalBytesRcvd < echoStringLen) {
    // I/O buffer
    char buffer[BUFSIZE];
    /*
     * Receive up to the buffer size (minus 1 to leave space
     * for a null terminator) bytes from the sender.
     */
    numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
    if (numBytes < 0) {
      DieWithSystemMessage("recv() failed");
    } else if (numBytes == 0) {
      DieWithUserMessage("recv()", "connection closed prematurely");
    }
    // Keep tally of total bytes
    totalBytesRcvd += numBytes;
    // Terminate the string!
    buffer[numBytes] = '\0';
    // Print the echo buffer
    fputs(buffer, stdout);
  }
  // Print a final linefeed
  fputc('\n', stdout);
  close(sock);
  return 0;
}