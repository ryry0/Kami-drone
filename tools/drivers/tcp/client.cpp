#include <iostream>
#include <string>
#include "tcpipnix.h"
using namespace std;

int main()
{
  //This client server implements a simple "chat" program.
  //It sends one letter at a time.
  //To talk to a server you need to do the following:
  //Create the TCP Object
  TCP tcpConn;

  char input;

  //Specify a port to connect to
  int port = 12345;

  string addr = "127.0.0.1";


  //Specify an IP address such as the above
  cout << "ip address:";
  cin >> addr;

  //run the connectToHost function, and pass in the port, and the address
  //the function returns true when you connect, and false if you don't
  cout << "connecting" << endl;
  if (!tcpConn.connectToHost(port, addr.c_str()))
    return 1;

  cout << "connected" << endl;

  while (1)
  {
    cin >> input;
    //run the Senddata function, using tcpConn.getSocket() as the first argument
    //a character pointer to the data for the second argument,
    //and the size of the data to send (in bytes) (1char = 1byte)
    tcpConn.sendData(tcpConn.getSocket(), (char *) &input, 1);

  }

  return 0;
}
