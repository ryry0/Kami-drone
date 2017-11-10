#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "tcpip.h"

int main()
{
  //This client server implements a simple "chat" program.
  //It sends one letter at a time.
  //To talk to a server you need to do the following:
  //Create the TCP Object
  tcp_connection_t tcpConn = tcp_create();

  char input;

  //Specify a port to connect to
  int port = 12345;

  char addr[100] = "127.0.0.1";


  //Specify an IP address such as the above
  printf("ip address:");
  scanf("%s", addr);

  //run the connectToHost function, and pass in the port, and the address
  //the function returns true when you connect, and false if you don't
  printf("connecting\n");
  bool result = tcp_connectToHost(tcpConn, port, addr);
  if (result == false)
    return 1;

  printf("connected\n");

  while (1)
  {
    scanf("%c", &input);
    //run the Senddata function, using tcpConn.getSocket() as the first argument
    //a character pointer to the data for the second argument,
    //and the size of the data to send (in bytes) (1char = 1byte)
    tcp_sendData(tcpConn, (char *) &input, 1);

  }

  return 0;
}
