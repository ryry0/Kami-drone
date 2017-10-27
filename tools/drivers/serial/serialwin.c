#include <serial.h>

#ifdef _WIN32
#include <windows.h>
#define WIN32_LEAN_AND_MEAN

struct sr_port_s {
  HANDLE h_port;
};

sr_port_t sr_OpenPort(char *serial_addr) {
  sr_port_t port = malloc(sizeof(struct sr_port_s));

  port->h_port = CreateFile(serial_addr,                       //Port device -> default COM1
      GENERIC_READ| GENERIC_WRITE ,    //Specify the mode the device is opened under
      0,                              //the device isn't shared
      0,                              //the object has default security
      OPEN_EXISTING,                  //Specify which action to use on file
      0,                              //default
      0);                             //"

  if (port->h_port == INVALID_HANDLE_VALUE) //check if handle exists
  {
    port->h_port = NULL;
    free(port);
    return NULL;
  }
  else
    return port;
}

bool sr_InitPort(sr_port_t port, int32_t baud_rate) {
  DCB dcb; //struct defined by windows that deals with serial communications
  //set timeouts
  COMMTIMEOUTS timeouts;

  timeouts.ReadTotalTimeoutConstant    = 100;
  timeouts.ReadTotalTimeoutMultiplier  = 0;
  timeouts.WriteTotalTimeoutMultiplier = 0;
  timeouts.WriteTotalTimeoutConstant   = 0;
  if(SetCommTimeouts(port->h_port, &timeouts)== FALSE)
    return false;


  dcb.DCBlength = sizeof(DCB);
  if(GetCommState(port->h_port, &dcb) == false)
    return false;

  /*
     dcb.BaudRate = BaudRate;
     dcb.StopBits = StopBits;
     dcb.Parity = Parity;
     dcb.ByteSize = ByteSize;
     */

  /*
     char buildstr[50];
     buildstr[0] = '\0';
     strcat(buildstr, "baud=9600");
     strcat(buildstr," parity=N data=8 stop=1");
     */

  //BuildCommDCB(buildstr, &dcb);
  dcb.BaudRate = baud_rate; // Baud rate for the CAN converter
  dcb.ByteSize = 8; // 8 bits / char
  dcb.StopBits = ONESTOPBIT; // One stop bit
  dcb.Parity = NOPARITY; // No parity
  dcb.fParity = false; // No parity functions
  dcb.DCBlength = sizeof(DCB); // Need to set this
  dcb.fBinary = true; // This must be true; Microsoft does not support non-binary mode transfers
  dcb.fOutX = false; // XON/XOFF disabled for transmit
  dcb.fInX = false; // XON/XOFF disabled for receive
  dcb.fOutxCtsFlow = false; // CTS disabled
  dcb.fOutxDsrFlow = false; // DSR disabled
  dcb.fParity = false; // No parity
  dcb.fDsrSensitivity = false; // DSR disabled
  dcb.XonLim = 100; // This shouldn't matter since no flow control is used
  dcb.XoffLim = 100; // This shouldn't matter since no flow control is used
  dcb.fDtrControl = DTR_CONTROL_DISABLE; // Enables DTR line when opened, and stays on
  dcb.fRtsControl = RTS_CONTROL_DISABLE; // Enables RTS line when opened, and stays on
  dcb.fInX = false; // Disable receive XON/XOFF control
  dcb.fOutX = false; // Disable transmit XON/XOFF control
  dcb.fNull = false; // Do not discard null characters
  dcb.fAbortOnError = false; // Do not abort on an error


  if (SetCommState (port->h_port,&dcb) == false)
    return false;

  return true;
}

size_t sr_WritePort(sr_port_t port, uint8_t *buffer, size_t num) {
  DWORD num_bytes_written;

  WriteFile(port->h_port,
      buffer,
      num,
      &num_bytes_written,
      0);

  return num_bytes_written;
}

size_t sr_ReadPort(sr_port_t port, uint8_t *buffer, size_t num) {
  DWORD num_bytes_read;

  ReadFile(port->h_port,
      buffer,
      num,
      &num_bytes_read,
      0);

  return num_bytes_read;
}

void sr_ClosePort(sr_port_t port) {
  if (port != NULL) {
    CloseHandle(port->h_port);
    free(port);
  }
}
#endif
