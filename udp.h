// Module containing functions to initialize and utilize a UDP connection and
// communicate with the host through pre-set commands in a background thread.

#ifndef _UDP_H_
#define _UDP_H_

#include "wave_player.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT_NUMBER 8088
#define UPTIME_PATH "/proc/uptime"

struct arguements{
    int localPort;
    char* remoteMachineName;
    int remotePort;
    int socketDescriptor;
    char* command;
    struct sockaddr_in socket_address;
    socklen_t address_length;
};

// Returns the address of the local pthread.
pthread_t* get_udp_pthread(void);

// Returns the message associated with COMMAND as a string.
// Returns NULL if COMMAND not found in the library of commands.
char* call_command(char command[]);

// Initialize the UDP connection and socket. Returns socket information.
struct arguements* initialize(void);

// Background thread where socket continuously listens for commands, 
// formulates the response, and replies to the command with the response.
void* udp_listen(void* args);

#endif