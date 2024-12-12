#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED

#include <stdio.h>
#include <string.h>

#define cport_nr    6        /* COM number minus 1 */
#define bdrate      115200   /* 115200 baud */

int PrintBuffer(char* buffer);
int WaitForReply(void);
int WaitForDollar(void);
int CanRS232PortBeOpened(void);
void CloseRS232Port(void);
void SendCommands(char* buffer);

#endif