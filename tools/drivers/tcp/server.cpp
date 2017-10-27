#include <iostream>
#include "tcpipnix.h"
using namespace std;

int main()
{
  TCP tcpConn;
  unsigned int commSock = 0;
  char data;


  tcpConn.listenToPort(12345);
  commSock = tcpConn.acceptConnection();

  while(1)
  {
    tcpConn.receiveData(commSock, (char *) &data, 1);
    cout << data << endl;
  }

  return 0;
}
