#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#define PORTNUM 9007

int main(int argc, char const *argv[]) {
  int server_socket, client_socket, n, randomNum;
  char msg[1024];
  socklen_t len;

  //create the server sockt
  server_socket = socket(AF_INET, SOCK_DGRAM, 0);

  //define the server address;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORTNUM);
  server_address.sin_addr.s_addr = INADDR_ANY;

  //bind the socket to our specified IP and port
  bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

  //while loop that continuosly runs and sends message back to client
  while (1) {
    //Random number generator 0-10
    srand(time(NULL));
    randomNum = rand() % 11;

    len = sizeof(client_address);
    n = recvfrom(server_socket, msg, sizeof(msg), 0, (struct sockaddr*)
    &client_address, &len);
    printf("message: %s\n", msg);

    //packet fake packet loss
    if (randomNum < 4) {
      printf("This is a simulated packet loss\n");
    }else {
      sendto(server_socket, msg, n, 0, (struct sockaddr*) &client_address, len);
    }
  }
  close(server_socket);
  return 0;
}