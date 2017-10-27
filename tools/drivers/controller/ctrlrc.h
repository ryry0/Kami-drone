/*
  Author:Ryan - David Reyes
*/
#ifndef CTRLRNIX_H_
#define CTRLRNIX_H_

#include <signal.h>  //signal handling
#include <unistd.h> //file descriptor manipulation
#include <stdlib.h> //exit();
#include <fcntl.h>  //file descriptor manipulation
#include <pthread.h>
//The following are handy variables to use when referring to joystick data
const int XAXIS = 0;
const int YAXIS = 1;
const int XAXIS2 = 3;
const int YAXIS2 = 2;
const int XHAT = 4;
const int YHAT = 5;

//Configure these for the properties of the gamepad
const int NUM_ANALOG_STICKS = 3; //the logitech wireless has 4
const int NUM_AXES = 2;
const int NUM_BUTTONS = 11;



const int ANALOG_EVENT = 2;
const int BUTTON_EVENT = 1;
char * DEFAULTADDRESS="/dev/input/js0";

struct controllerEvent
{
  unsigned int time;  //time event happened
  short value;    //value of the analog stick/buttons
  unsigned char type; //type of event (button or analog)
  unsigned char number; //number of the button or stick
};

struct controllerState
{
  int buttons[NUM_BUTTONS];
  int stick[NUM_AXES*NUM_ANALOG_STICKS];
};

    int  openController(char *);
    void readControllerEvent();
    void writeControllerState();
    void update();

    int getStickvalue(int sticknum)
      {return contSt.stick[sticknum];}
    int getButton(int buttonnum)
      {return contSt.buttons[buttonnum];}

    controllerEvent contEv;
    controllerState contSt;
    int controllerfd;   //controller file descriptor
    char * joystickAddress;

void readControllerEvent()
{
  read(controllerfd, contEv, sizeof(contEv));
}

void writeControllerState()
{
  if(contEv.type == ANALOG_EVENT)
  {
    contSt.stick[contEv.number] = contEv.value;
  }
  else //if it is button event
  {
    contSt.buttons[contEv.number] = contEv.value;
  }
}

void update()
{
  readControllerEvent();
  writeControllerState();
}

bool openController(char * joyAddressArg)
{
  controllerfd = open(joyAddressArg, O_RDONLY | O_NONBLOCK);
  if (controllerfd < 0)
    return false;
}

#endif
