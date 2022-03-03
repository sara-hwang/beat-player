#ifndef _UDP_H_
#define _UDP_H_

#include "wave_player.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_NUMBER 8088

struct arguements{
    int localPort;
    char* remoteMachineName;
    int remotePort;
    int socketDescriptor;
    char* command;
    struct sockaddr_in socket_address;
    socklen_t address_length;
};

char* command_vol_up(void);

char* command_vol_down(void);

char* command_tempo_up(void);

char* command_tempo_down(void);

char* command_get_vol(void);

char* command_beat(char* mode);
// Returns the message associated with COMMAND as a string.
// Returns NULL if COMMAND not found in the library of commands.
char* call_command(char command[]);

// Initialize the UDP connection and socket. Returns socket information.
struct arguements* initialize(void);

// Background thread where socket continuously listens for commands, 
// formulates the response, and replies to the command with the response.
// Loops until stop_listening() is called.
void* udp_listen(void* args);

// Helper function to correctly format responses to commands.
void format_reply(char* reply);

// Helper function for command_get_N() to extract the number of samples
// requested (N).
void extract_num(char buffer[]);

// Terminates the background thread udp_listen() after it finishes its
// current iteration.
void stop_listening(void);


#endif