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
#define MAXCOUNT 3
#define PINGAMOUNT 10
//#define MAXLINE 1024

int main(int argc, char const *argv[]) {
  struct timeval tv;
  int net_Socket, error, recvFrom, status, pingCount;
  int count = 0;
  ssize_t n;
  int portNum, maxLine;
  char server_response[256], sendline[1024];
  const char *IP;

  IP = argv[1];
  portNum = atoi(argv[2]);
  maxLine = atoi(argv[3]);

  if (portNum != 9007) {
    perror("incorrect portnuber use 9007");
    return 1;
  }

  printf("%d\n", portNum);
  strcpy(sendline, "");
  printf("\n Enter the message: ");
  fgets(sendline, maxLine, stdin);
  //create a socket
  net_Socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (net_Socket < 0){
    printf("cannot create socket\n");
    return 1;
  }

  struct sockaddr_in client_address;
  socklen_t addr_len = sizeof(client_address);

  //specify an address for the socket
  struct sockaddr_in server_address;


  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(portNum);
  inet_aton(IP, &server_address.sin_addr);

  n = sendto(net_Socket, sendline, maxLine, 0, (struct sockaddr*) &server_address, sizeof(server_address));
  if ( n < 0) {
    perror("send failed");
    return 1;
  }
  //printf("n is: %d\n", n);
  printf("data has been sent to the server. \nDestination IP: %s\n Destination"
    "Port:%s\n Length of string to be send %s\n string to be sent %s\n", argv[1],
    argv[2], argv[3], sendline);

    //set recvfrom timeout to 1 second
    tv.tv_sec = 1;
    tv.tv_usec = 0;


  while (count < 3) {
    setsockopt(net_Socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
    n = recvfrom(net_Socket, server_response, sizeof(server_response), 0, (struct sockaddr*)&client_address, &addr_len);
    if (n <= 0) {
      printf("has not recieved message in last second %d more attempts\n", MAXCOUNT-count);
      int test = sendto(net_Socket, sendline, maxLine, 0, (struct sockaddr*) &server_address, sizeof(server_address));
      if (test < 0) {
        perror("send failed");
      }
    } else {
      server_response[n] = 0;
      printf("Got packet from%s\n", inet_ntoa(client_address.sin_addr));
      printf("\nServers Echoing back: %s\n", server_response);
      break;
    }

    count++;
  }

  close(net_Socket);
  return 0;
}
