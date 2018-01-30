#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

#define PINGAMOUNT 10
#define PINGREQUEST 1

int main(int argc, char const *argv[]) {
  int client_socket, n;
  int portNum;
  int pingCount = 1;
  int messages[2];
  socklen_t addr_len;
  const char *IP;

  IP = argv[1];
  portNum = atoi(argv[2]);

  if (portNum != 9007) {
    perror("incorrect port number use 9007");
    return 1;
  }

  client_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (client_socket < 0) {
    printf("cannot create socket\n");
    return 1;
  }

  struct sockaddr_in client_address;
  addr_len = sizeof(client_address);

  //specify an address for the socket to be sent to
  struct sockaddr_in server_address;

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(portNum);
  inet_aton(IP, &server_address.sin_addr);

  while (pingCount <= PINGAMOUNT) {
    //populate the first element in array message type & set in byte order
    message[0] = htons(PINGREQUEST)
    //populates second element in array to the sequence number & set it in byte order
    message[1] = htons(pingCount);

    n = sendto(client_socket, message, sizeof(message), 0, (struct sockaddr*)
      &server_address, sizeof(server_address));
    if (n < 0) {
      perror("send failed");
      return 1;
    }
    pingCount += 1;
  }

  close(client_socket);
  return 0;
}
