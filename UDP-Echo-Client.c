#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

int main(int argc, char const *argv[]) {
  int net_Socket, error, recvFrom, status;
  char *hello = "Hello from client";
  char server_response[256];
  //create a socket
  net_Socket = socket(AF_INET, SOCK_DGRAM, 0);

  //specify an address for the socket
  struct sockaddr_in client_address;
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9007);
  server_address.sin_addr.s_addr = INADDR_ANY;



  sendto(net_Socket, hello, strlen(hello), 0, (struct sockaddr*) &server_address, sizeof(server_address));

  //recvfrom(net_Socket, &server_response, sizeof(server_response), 0, &server_address, sizeof(server_address));

  close(net_Socket);
  return 0;
}
