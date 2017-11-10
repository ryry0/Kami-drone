/*
  Author:Ryan - David Reyes
*/
#ifndef TCPIP_H_
#define TCPIP_H_

#include <stdint.h>
#include <stdbool.h>

/*
    The socket that you do data operations on if you're the client and
    have used connectToHost is servSock.

    If you're the server the socket you use for data operations is
    the one returned from acceptConnection().
*/

typedef struct tcp_connection_s* tcp_connection_t;

#define SOCKET_ERROR -1

tcp_connection_t tcp_create();
void tcp_destroy(tcp_connection_t tcp_conn);

bool tcp_connectToHost(tcp_connection_t tcp_conn, int portNo, const char *IPAddress);
bool tcp_listenToPort(tcp_connection_t tcp_conn, int portNo);

bool tcp_acceptConnection(const tcp_connection_t tcp_conn, tcp_connection_t
    tcp_accepted);

int tcp_sendData(const tcp_connection_t tcp_conn, const uint8_t *data, const int len);
int tcp_receiveData(const tcp_connection_t tcp_conn,
    uint8_t *data,
    int len);

/*
  The folowing functions send and receive data
  framed with a 4 byte packet that specifies
  the size of that data
*/

int tcp_sendFramedData(tcp_connection_t tcp_conn, const uint8_t *data, int len);
int tcp_receiveFramedData(tcp_connection_t tcp_conn, uint8_t *data);

int tcp_getSocket(tcp_connection_t tcp_conn);
int tcp_closeSocket(tcp_connection_t tcp_conn);

#endif // TCPIPNIX_H_
