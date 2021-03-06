#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

int main(int argc, char const *argv[]) {

  int net_Socket, connection_status;
  char server_response[256];

  //create a socket
  net_Socket = socket(AF_INET, SOCK_STREAM, 0);

  //specify an address for the socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9007);
  server_address.sin_addr.s_addr = INADDR_ANY;

  connection_status = connect(net_Socket, (struct sockaddr *) &server_address, sizeof(server_address));

  //check for error with connection
  if (connection_status == -1) {
    printf("There was an error making a connection to the remote socket\n");
  }

  //recieve data from the server
  recv(net_Socket, &server_response, sizeof(server_response), 0);

  //print out hte servers response
  printf("The server sent the data: %s\n", server_response);

  //close the socket
  close(net_Socket);

  return 0;
}
