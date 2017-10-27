#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <serialnix.h>
#include <getch.h>

void echoPrint(int fd);
void terminate(int signal);


int main(int argc, char **argv)
{
    char *path;
    char chtosend='\n';
    int fd;             //file descriptor
    signal(SIGINT, terminate);

    if (argc > 1)
    {
      path  = argv[1];
    }
    else
    {
      printf("\nAddress of serial device required as argument.\n");
      return 1;
    }

    printf("Opening serial port...\n");
    fd = open_port(path);

    printf("Initializing serial port...\n");
    if(init_serial_port(fd))
    {
        printf("Initialization failed!\n");
        return 1;
    }

    printf("Initialized!\n");
    printf("Press escape to quit.\n");
    printf("Press ENTER when ready:\n");

    chtosend = getch();               //pause until ready
    printf("Ready!\n");
    //echoPrint(fd);                      //read the buffer

    while(chtosend != 0x1B)           //while not escape read and send data
    {
        chtosend = getch();
        printf("0x%x \t %c\n", chtosend,chtosend);

        write(fd, (char*) &chtosend, sizeof(chtosend));
        //echoPrint(fd);
    }

    printf("\nExiting..\n");
    close(fd);
    return 0;
}

void terminate(int signal)  //SIGINT signal handler
{
    //exit(0);
}

void echoPrint(int fd)
{
    char outputchar;
    while(read(fd, &outputchar, sizeof(outputchar)) > 0)
        printf("%c", outputchar);
}
