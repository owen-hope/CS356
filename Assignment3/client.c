#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

int main(int argc, char const *argv[]) {

  int pos = 0;
  char *token;
  char getInfo[100];
  char* infoSplit[100];
  int net_Socket, connection_status;
  char server_response[256];

  //localhost:9007/filename.html
  const char *webUrl[] = argv[1];
  strcpy(getInfo, webUrl);

  token = strtok(webUrl, ":/");

  while (token != NULL) {
    token = strtok(NULL, ":/");

    infoSplit[pos] = malloc(strlen(token) + 1);
    strcpy(infoSplit[pos], token);
    pos++;
  }

  printf("%s\n", infoSplit[0]);



  //creating a socket
  net_Socket = socket(AF_INET, SOCK_STREAM, 0);

  //specify an address for the socket
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(9007);
  server_address.sin_addr.s_addr = INADDR_ANY;

  connection_status = connect(net_Socket, (struct sockaddr *) &server_address, sizeof(server_address));

  //check for error with connection
  if (connection_status == -1) {
    perror("There was an error making a connection to the remote socket\n");
  }

  recv(net_Socket, &server_response, sizeof(server_response), 0);

  //print out the servers response
  printf("The server sent the data: %s\n", server_response);

  //close the socket
  close(net_Socket);

  return 0;
}
