#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>

#define PINGAMOUNT 10
#define PINGREQUEST 1

double RTTCalculation (time_t start, time_t end) {
  double RTTCalc;

  RTTCalc = ((double)(end - start));
  return RTTCalc;
}

int main(int argc, char const *argv[]) {
  //the array [0] is message type [1] is sequence number
  int client_socket, n, packetsRecieved = 0, packetsLost = 0;
  int percentPacketLoss;
  int portNum;
  int pingCount = 1;
  int messages[2];
  int serverMessages[2];
  int serverMessagesConverted[2];
  socklen_t addr_len;
  const char *IP;
  //clock_t time;
  time_t start_time, end_time;

  IP = argv[1];
  portNum = atoi(argv[2]);

  if (portNum != 9007) {
    perror("incorrect port number use 9007");
    return 1;
  }

  client_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (client_socket < 0) {
    printf("cannot create socket\n");
    return 1;
  }

  struct sockaddr_in client_address;
  addr_len = sizeof(client_address);

  //specify an address for the socket to be sent to
  struct sockaddr_in server_address;

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(portNum);
  inet_aton(IP, &server_address.sin_addr);

  //Set the timeout for the socket waiting for response
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*) &tv, sizeof(tv));

  printf("Pinging %s, %d:\n", IP, portNum);
  while (pingCount <= PINGAMOUNT) {

    //populate the first element in array message type & set in byte order
    messages[0] = htons(PINGREQUEST);
    //populates second element in array to the sequence number & set it in byte order
    messages[1] = htons(pingCount);

    //get start time to calculate RTT
    clock_t begin = clock();

    n = sendto(client_socket, messages, sizeof(messages), 0, (struct sockaddr*)
      &server_address, sizeof(server_address));
    if (n < 0) {
      perror("send failed");
      return 1;
    }
    //sleep(2);
    n = recvfrom(client_socket, serverMessages, sizeof(serverMessages), 0,
      (struct sockaddr*) &client_address, &addr_len);

    //get end time to calculate RTT
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    for (int i = 0; i < 2; i++) {
      serverMessagesConverted[i] = ntohs(serverMessages[i]);
    }

    if (n <= 0) {
      printf("Ping message number %d timed out\n", pingCount);
      packetsLost += 1;
    } else if (serverMessagesConverted[0] == 2) {
      printf("ping message number %d RTT: %f secs\n", pingCount,
        time_spent);
      packetsRecieved += 1;
    }

    pingCount += 1;
  }
  printf("packets lost %f\n", packetsLost/PINGAMOUNT);
  percentPacketLoss = ((packetsLost/PINGAMOUNT) * 100);
  printf("Number of packes sent: %i Recieved: %i Loss rate: %i%%\n",
    PINGAMOUNT, packetsRecieved, percentPacketLoss);

  close(client_socket);
  return 0;
}
