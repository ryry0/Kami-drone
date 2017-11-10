/*
  Author:Ryan - David Reyes
*/

#include <tcpip.h>
#ifdef __linux__

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>


const int INVALID_SOCKET = -1;


struct tcp_connection_s {
  ssize_t socket;
  ssize_t port;
};

tcp_connection_t tcp_create()
{
  tcp_connection_t temp = malloc(sizeof(struct tcp_connection_s));
  temp->socket = SOCKET_ERROR;
  return temp;
}

void tcp_destroy(tcp_connection_t tcp_conn)
{
  if (tcp_conn) {
    if (tcp_conn->socket)
      close(tcp_conn->socket);

    free(tcp_conn);
  }
  //dtor
}

bool tcp_connectToHost(tcp_connection_t tcp_conn, int portNo, const char* IPAddress)
{
  struct sockaddr_in target;

  //fill out info needed to initialize a socket
  //address internet
  target.sin_family = AF_INET;

  //port to conect on (host to network)
  target.sin_port = htons(portNo);
  target.sin_addr.s_addr = inet_addr(IPAddress);  //target ip

  //create the socket
  tcp_conn->socket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
  tcp_conn->port = portNo;
  if (tcp_conn->socket == INVALID_SOCKET) {
    printf("couldn't create the socket");
    return false;
  }


  if ( connect( tcp_conn->socket,
      (struct sockaddr*)&target,
      sizeof(target)) == SOCKET_ERROR) {
    printf("couldnt connect");
    return false;
  }

  else
    return true;

}

bool tcp_listenToPort(tcp_connection_t tcp_conn, int portNo)
{
  struct sockaddr_in addr;

  addr.sin_family = AF_INET;

  addr.sin_port = htons(portNo);
  addr.sin_addr.s_addr = htonl (INADDR_ANY);
  //assign port to this socket

  //accept a connection from any IP using INADDR_ANY
  //or just pass inet_addr("0.0.0.0")
  //specify a specific ip address to watch for only that address

  tcp_conn->socket = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
  tcp_conn->port = portNo;

  if (tcp_conn->socket == INVALID_SOCKET)
    return false;

  if (bind( tcp_conn->socket,
      (struct sockaddr*) &addr,
      sizeof(addr)) == SOCKET_ERROR)

    return false;
  // will happen if you try to bind to same socket multiple times

  //listen to the port with max connections possible
  //function does not return until a connection request is made
  // u_long iMode=1;
  //ioctlsocket(s,FIONBIO,&iMode);

  listen(tcp_conn->socket, SOMAXCONN);
  return true;
}

bool tcp_acceptConnection(const tcp_connection_t tcp_conn, tcp_connection_t
    tcp_accepted)
{
  unsigned int TempSock = SOCKET_ERROR;
  TempSock = accept(tcp_conn->socket, NULL, NULL);
  if (TempSock) {
    tcp_accepted->socket = TempSock;
    return true;
  }
  return false;
}

int tcp_sendData(const tcp_connection_t tcp_conn, const uint8_t *data, const int len)
{
  return send(tcp_conn->socket, data, len, 0);
}

int tcp_receiveData(const tcp_connection_t tcp_conn, uint8_t *buffer, const int len)
{
  return recv(tcp_conn->socket, buffer, len, 0);
}

//Sends a 4 byte packed integer. Sending one byte at a time
//is more reliable than sending an int in one shot.
int tcp_sendFramedData(const tcp_connection_t tcp_conn, const uint8_t *data, const int len)
{
  char lenBuff[4] = {0};

  lenBuff[0] = (unsigned char) len;
  lenBuff[1] = (unsigned char) (len >> 8);
  lenBuff[2] = (unsigned char) (len >> 16);
  lenBuff[3] = (unsigned char) (len >> 24);

  tcp_sendData(tcp_conn, lenBuff, 4);
  return tcp_sendData(tcp_conn, data, len);
}


int tcp_receiveFramedData(const tcp_connection_t tcp_conn, uint8_t *data)
{
  char lenBuff[4];
  int lenPrefix;
  int totalRead;
  int currentRead;

  //priming read:
  currentRead = totalRead = tcp_receiveData(tcp_conn, lenBuff, 4);

  //read if and while not enough data received, until all data arrives
  while (currentRead > 0 && totalRead < 4)
  {
    currentRead = tcp_receiveData(tcp_conn,
              (lenBuff + totalRead),
              (4 - totalRead));
    totalRead += currentRead;
  }

  //dereferenced casted int pointer to char array
  lenPrefix = *(int *) lenBuff;
  //reset current and total read
  currentRead = 0;
  totalRead = 0;


  //priming read:
  currentRead = totalRead = tcp_receiveData(tcp_conn, data, lenPrefix);

  //read if and while not enough data received, until all data arrives
  while (currentRead > 0 && totalRead < lenPrefix)
  {
    currentRead = tcp_receiveData(tcp_conn,
              (data + totalRead),
              (lenPrefix - totalRead));
    totalRead += currentRead;
  }


  //dtor
  return totalRead;
}

int tcp_getSocket(tcp_connection_t tcp_conn) {
  return tcp_conn->socket;
}

int tcp_closeSocket(tcp_connection_t tcp_conn) {
  int retval = close(tcp_conn->socket);
  tcp_conn->socket = SOCKET_ERROR;
  return retval;
}

#endif
