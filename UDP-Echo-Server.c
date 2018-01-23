#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

int main(int argc, char const *argv[]) {
  int server_socket, client_socket, n;
  char server_message[256] = "You have reached the UDP server!";
  char msg[1024];
  socklen_t len;
  //create the server socket
  server_socket = socket(AF_INET, SOCK_DGRAM, 0);

  //define the server address
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9007);
  server_address.sin_addr.s_addr = INADDR_ANY;

  //bind the socket to our specified IP and port
  bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

  //sendto(server_socket, server_message, strlen(server_message), 0, (struct sockaddr*) &client_address, sizeof(client_address));
  while(1) {
    len=sizeof(client_address);
    n = recvfrom(server_socket, msg, sizeof(msg), 0 ,(struct sockaddr*) &client_address, &len);
    printf("message: %s\n", msg);

    sendto(server_socket, msg, n, 0, (struct sockaddr*) &client_address, len);
  }
  close(server_socket);
  return 0;
}
