/*
	Author: Ryan - David Reyes
*/
#ifndef SERIALNIX_H
#define SERIALNIX_H

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int open_port(char * serialAddr); //returns the file descriptor or -1;
int init_serial_port(int fd); //returns -1 if not successful

#endif
