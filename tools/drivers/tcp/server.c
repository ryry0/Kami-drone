#include <stdio.h>
#include "tcpip.h"

int main()
{
  tcp_connection_t tcpConn = tcp_create();
  tcp_connection_t tcpAccepted= tcp_create();
  unsigned int commSock = 0;
  char data;


  tcp_listenToPort(tcpConn, 12345);
  tcp_acceptConnection(tcpConn, tcpAccepted);

  while(1)
  {
    tcp_receiveData(tcpAccepted, (uint8_t *) &data, 1);
    printf("%c\n", data);
  }

  tcp_destroy(tcpConn);
  tcp_destroy(tcpAccepted);
  return 0;
}
