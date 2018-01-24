#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAXLINE 1024

int main(int argc, char const *argv[]) {
  int net_Socket, error, recvFrom, status, n;
  int portNum;
  char server_response[256], sendline[1024];

  portNum = atoi(argv[2]);
  printf("%d\n", portNum);
  strcpy(sendline, "");
  printf("\n Enter the message: ");
  fgets(sendline, MAXLINE, stdin);
  //create a socket
  net_Socket = socket(AF_INET, SOCK_DGRAM, 0);

  //specify an address for the socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(portNum);
  inet_aton(argv[1], &server_address.sin_addr);

  printf("this is the send line1: %s\n", sendline);

  sendto(net_Socket, sendline, MAXLINE, 0, (struct sockaddr*) &server_address, sizeof(server_address));

  n = recvfrom(net_Socket, server_response, sizeof(server_response), 0, NULL, NULL);
  server_response[n] = 0;
  printf("this is the send line: %s\n", sendline);
  printf("\nServers Echoing back: %s\n", server_response);

  close(net_Socket);
  return 0;
}
