#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

//#define MAXLINE 1024

int main(int argc, char const *argv[]) {
  int net_Socket, error, recvFrom, status, n;
  int portNum, maxLine;
  char server_response[256], sendline[1024];

  maxLine = atoi(argv[3]);
  portNum = atoi(argv[2]);
  printf("%d\n", portNum);
  strcpy(sendline, "");
  printf("\n Enter the message: ");
  fgets(sendline, maxLine, stdin);
  //create a socket
  if ((net_Socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("cannot create socket\n");
    return 1;
  }

  //specify an address for the socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(portNum);
  inet_aton(argv[1], &server_address.sin_addr);

  if (sendto(net_Socket, sendline, maxLine, 0, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
    perror("send failed");
    return 1;
  }

  printf("data has been sent to the server. \nDestination IP: %s\n Destination
    Port:%s\n Length of string to be send %s\n string to be sent %s\n", argv[1],
    argv[2], argv[3], sendline);

  n = recvfrom(net_Socket, server_response, sizeof(server_response), 0, NULL, NULL);
  if (n > 0) {
    server_response[n] = 0;
    printf("\nServers Echoing back: %s\n", server_response);

  }

  close(net_Socket);
  return 0;
}
