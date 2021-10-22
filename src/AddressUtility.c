#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void PrintSocketAddress(const struct sockaddr *addr, FILE *stream) {
  // Test for address and stream
  if (addr == NULL || stream == NULL) {
    return;
  }
  // Pointer to binary address
  void *numericAddress;
  // Buffer to contain result (IPv6 sufficient to hold IPV4)
  char addrBuffer[INET6_ADDRSTRLEN];
  // Port to print
  in_port_t port;
  // Set pointer to address based on address family
  switch (addr->sa_family) {
  case AF_INET:
    numericAddress = &((struct sockaddr_in *)addr)->sin_addr;
    port = htons(((struct sockaddr_in *)addr)->sin_port);
    break;
  case AF_INET6:
    numericAddress = &((struct sockaddr_in6 *)addr)->sin6_addr;
    port = htons(((struct sockaddr_in6 *)addr)->sin6_port);
    break;
  default:
    // Unhandle type
    fputs("[unknown type]", stream);
    return;
  }
  // Convert binary to printable address
  if (inet_ntop(addr->sa_family, numericAddress, addrBuffer,
                sizeof(addrBuffer)) == NULL) {
    // Unable to convert
    fputs("[invalid address]", stream);
  } else {
    fprintf(stream, "%s", addrBuffer);
    // Zero not valid in any socket addr
    if (port != 0) {
      fprintf(stream, "-%u", port);
    }
  }
}

bool SockAddrsEqual(const struct sockaddr *addr1,
                    const struct sockaddr *addr2) {
  if (addr1 == NULL || addr2 == NULL)
    return addr1 == addr2;
  else if (addr1->sa_family != addr2->sa_family)
    return false;
  else if (addr1->sa_family == AF_INET) {
    struct sockaddr_in *ipv4Addr1 = (struct sockaddr_in *)addr1;
    struct sockaddr_in *ipv4Addr2 = (struct sockaddr_in *)addr2;
    return ipv4Addr1->sin_addr.s_addr == ipv4Addr2->sin_addr.s_addr &&
           ipv4Addr1->sin_port == ipv4Addr2->sin_port;
  } else if (addr1->sa_family == AF_INET6) {
    struct sockaddr_in6 *ipv6Addr1 = (struct sockaddr_in6 *)addr1;
    struct sockaddr_in6 *ipv6Addr2 = (struct sockaddr_in6 *)addr2;
    return memcmp(&ipv6Addr1->sin6_addr, &ipv6Addr2->sin6_addr,
                  sizeof(struct in6_addr)) == 0 &&
           ipv6Addr1->sin6_port == ipv6Addr2->sin6_port;
  } else
    return false;
}