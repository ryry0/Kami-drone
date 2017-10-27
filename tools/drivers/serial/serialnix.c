#include <serial.h>

#ifdef __linux__

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

struct sr_port_s {
  int fd;
};

sr_port_t sr_OpenPort(char *serial_addr) {
  sr_port_t port = malloc(sizeof(struct sr_port_s));

	port->fd = open(serial_addr, O_RDWR | O_NOCTTY | O_NDELAY);
	if (port->fd==-1) {
		perror("open_port: Unable to open serial port");
		free(port);
		return NULL;
	}
	else
		fcntl(port->fd, F_SETFL, 0);

	return port;
}

bool sr_InitPort(sr_port_t port, int32_t baud_rate) {
	struct termios options;
	tcgetattr(port->fd, &options);
	cfsetispeed(&options, baud_rate);
	cfsetospeed(&options, baud_rate);
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
	options.c_oflag &= ~OPOST;

	if (tcsetattr(port->fd, TCSANOW, &options) == 0)
	  return true;
  else
    return false;
}

size_t sr_ReadPort(sr_port_t port, uint8_t *buffer, size_t num) {
  return read(port->fd, buffer, num);
}

size_t sr_WritePort(sr_port_t port, uint8_t *buffer, size_t num) {
  return write(port->fd, buffer, num);
}

void sr_ClosePort(sr_port_t port) {
  if (port != NULL) {
    close(port->fd);
    free(port);
  }
}
#endif
