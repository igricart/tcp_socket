
#include <arpa/inet.h>  // defines in_addr structure
#include <errno.h>  //It defines macros for reporting and retrieving error conditions through error codes
#include <netinet/in.h>  //contains constants and structures needed for internet domain addresses
#include <netinet/tcp.h>
#include <stdio.h>       // standard input and output library
#include <stdlib.h>      // this includes functions regarding memory allocation
#include <string.h>      // contains string functions
#include <sys/socket.h>  // for socket creation
#include <sys/types.h>  //contains a number of basic derived types that should be used whenever appropriate
#include <time.h>    //contains various functions for manipulating date and time
#include <unistd.h>  //contains various constants

#include <cmath>  //modf
void generateTimeoutTimeval(const double timeout_elapsed_seconds,
                            timeval &timeout) {
  // Get integer and fractional part
  double timeout_integer_part_seconds, timeout_fractional_part_seconds;
  timeout_fractional_part_seconds =
      std::modf(timeout_elapsed_seconds, &timeout_integer_part_seconds);

  // Save parameters
  timeout.tv_sec = static_cast<size_t>(timeout_integer_part_seconds);
  timeout.tv_usec =
      static_cast<long int>(timeout_fractional_part_seconds) * 1000000;
};

int main() {
  time_t clock;
  char
      dataSending[1025];  // Actually this is called packet in Network
                          // Communication, which contain data and send through.
  int clintListn = 0, clintConnt = 0;
  struct sockaddr_in ipOfServer;
  struct timeval timeout;
  generateTimeoutTimeval(10, timeout);
  clintListn = socket(AF_INET, SOCK_STREAM, 0);  // creating socket
  setsockopt(clintListn, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
  setsockopt(clintListn, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
  // NODELAY turns off the Nagle algorithm in TCP
  int on = 1;
  setsockopt(clintListn, IPPROTO_TCP, TCP_NODELAY, (char *)&on, sizeof(int));
  memset(&ipOfServer, '0', sizeof(ipOfServer));
  memset(dataSending, '0', sizeof(dataSending));
  ipOfServer.sin_family = AF_INET;
  ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY);
  ipOfServer.sin_port =
      htons(2017);  // this is the port number of running server
  bind(clintListn, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer));
  listen(clintListn, 20);

  while (1) {
    printf(
        "\n\nHi,Iam running server.Some Client hit me\n");  // whenever a
                                                            // request from
                                                            // client came. It
                                                            // will be processed
                                                            // here.
    clintConnt = accept(clintListn, (struct sockaddr *)NULL, NULL);

    clock = time(NULL);
    snprintf(dataSending, sizeof(dataSending), "%.24s\r\n",
             ctime(&clock));  // Printing successful message
    write(clintConnt, dataSending, strlen(dataSending));

    close(clintConnt);
    sleep(1);
  }

  return 0;
}