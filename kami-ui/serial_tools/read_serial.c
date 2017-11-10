#include <serial.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <conio.h>

#define TEST_PACKET_LENGTH 3
#define DEFAULT_SERIAL "COM3"

sr_port_t serial_port;
bool print_hex = false;
bool raw_write = false;


void *readThread(void *args) {
  char input = 0;
  for (;;) {
    if (!sr_ReadPort(serial_port, &input, 1))
      continue;

    if (print_hex)
      printf("%x\n", input);
    else
      printf("%c", input);
  }
}

void sequentialWrite(sr_port_t serial_port, uint8_t *buff, size_t len) {

  for (size_t i = 0; i < len; ++i) {
      sr_WritePort(serial_port, &buff[i], 1);
  }
}

void writeThread() {
  char user_input = 0;
  user_input = getch();
  char buff[100] =  "testingtestingab";
  char buff2[100] = "abcdefgasfecegle";
  char buff3[100] = "\0\3\0s";
  uint8_t force_syn = 0xff;
  char buf_generic[10] = "\0\0\0\0";

  switch (user_input) {
    case 3:
      print_hex = !print_hex;
      break;

    case 2:
      raw_write = !raw_write;
      printf("raw write: %d", raw_write);
      break;

    case '@':
      sr_WritePort(serial_port, buff, 17);
      break;

    case 'f':
      sequentialWrite(serial_port, &force_syn, 1);
      break;

    case '1':
      sequentialWrite(serial_port, "\0\0\0p", 4);
      sequentialWrite(serial_port, buff, 16);
      break;

    case '2':
      sequentialWrite(serial_port, "\0\0\0p", 4);
      sequentialWrite(serial_port, buff2, 16);
      break;

    case 's':
      sequentialWrite(serial_port, "\0\0\0s", 4);
      break;

    default:
      if (raw_write) {
        sequentialWrite(serial_port, &user_input, 1);
      }
      else {
        buf_generic[3] = user_input;
        sequentialWrite(serial_port, buf_generic, 4);
      }
  }
}

//signal to handle
int main(int argc, char ** argv) {

  char buff1[100] = {0};
  char buff2[100] = {0};
  uint8_t input = 0;
  uint8_t option = 0;


  pthread_t read_thread;

  serial_port = sr_OpenPort(DEFAULT_SERIAL);
  if (serial_port == NULL) {
    printf("Input serial port address:");
    scanf("%s", buff1);
    serial_port = sr_OpenPort(buff1);
  }
  else {
    printf("Default serial opened\n");
  }

  if (serial_port == NULL)
    return 1;

  if (sr_InitPort(serial_port, 115200) == false)
    return 1;

  pthread_create(&read_thread, NULL, readThread, NULL);

  for (;;)
    writeThread();

  pthread_join(read_thread, NULL);

  return 0;
}
