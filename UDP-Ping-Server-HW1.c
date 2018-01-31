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
  //the array [0] is message type [1] is sequence number
  int server_socket, client_socket, n, randomNum;
  char msg[1024];
  int arrayConverted[2];
  int arrayReceived[2];
  int arrayToSend[2];
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
    n = recvfrom(server_socket, arrayReceived, sizeof(arrayReceived), 0, (struct sockaddr*)
    &client_address, &len);
    //convert from byte order to int;
    for (int i = 0; i < 2; i++) {
      arrayConverted[i] = ntohs(arrayReceived[i]);
    }

    //packet fake packet loss
    if (randomNum < 4) {
      printf("This is a simulated packet loss\n");
    }else if (arrayConverted[0] == 1) {
      arrayConverted[0] = 2;
      printf("%i\n", arrayConverted[0]);

      //convert array back to byte order to send
      for (int i = 0; i < 2; i++) {
        arrayToSend[i] = htons(arrayConverted[i]);
      }
      sendto(server_socket, arrayToSend, sizeof(arrayToSend), 0,
        (struct sockaddr*) &client_address, len);
    }
  }
  close(server_socket);
  return 0;
}
