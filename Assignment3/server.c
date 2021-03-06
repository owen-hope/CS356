#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORTNUMBER 9007

int main(int argc, char const *argv[]) {
  int server_socket, client_socket;
  char server_message[256] = "You have reached the server!";

  //create the server socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);

  //define the server address
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORTNUMBER);
  server_address.sin_addr.s_addr = INADDR_ANY;

  //bind the socket to our specified IP and port
  bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
  listen(server_socket, 5);

  printf("The server is ready to receive on port: %i\n", PORTNUMBER);

while (1) {
  client_socket = accept(server_socket, NULL, NULL);
  
}

  //send the message
  send(client_socket, server_message, sizeof(server_message), 0);

  //close the socket
  close(server_socket);
  return 0;
}
