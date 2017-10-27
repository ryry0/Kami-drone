#include <iostream>
#include <iomanip>
#include <string>
#include "ctrlrnix.h"

using namespace std;

int main()
{
  string joypath;
  
  cout << "Please enter the joystick path:";
  //cin >> joypath;   //example: /dev/input/js0
  joypath = "/dev/input/js0";
  
  Ctrlr Gamepad(joypath.c_str());   //creating a joystick object
  cout << left;
  while (1)
  {
    Gamepad.update();   //Read events
    

    //for logitech wireless gamepad - example
    cout << "Stick 0 X axis: " << Gamepad.getStickvalue(XAXIS);
    cout << "\tStick 0 Y axis: " << Gamepad.getStickvalue(YAXIS);
    cout << "\tStick 1 X axis: " << Gamepad.getStickvalue(XAXIS2);
    cout << "\tStick 1 Y axis: " << Gamepad.getStickvalue(XHAT);
    cout << "\tL2: " << Gamepad.getStickvalue(YAXIS2);
    cout << "\tR2: " << Gamepad.getStickvalue(YHAT);
    //cout << "\tstart button: " << Gamepad.getButton(10) << endl;
    /*
    for( int i = 0; i < 11; i ++)
    {
      cout << i << " " << setw(8) << Gamepad.getButton(i) << " " ;
    }
    */
    cout << endl;
    /*
    for (int i = 0; i < 12; i ++ )
      cout << Gamepad.getStickvalue(i) << "\t";
    cout << endl;
    */
  }
  return 0;
}
