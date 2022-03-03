// Elements critical to program initialization and execution are declared here.
// All necessary libraries are also #included here.

#ifndef INITIALIZE_H_
#define INITIALIZE_H_

#define NUM_BEATS 3
#define BEAT_ID_LENGTH 4
#define NUM_EVENTS 5
#define MAX_LENGTH 256
#define EPOLL_FD_INDEX NUM_EVENTS
#define NUM_TRIGGERS_IGNORED 2
#define INDEX_W_DATA 0
#define EXPORT_FILE "/sys/class/gpio/export"
#define NANOSECONDS_PER_MILLISECOND 1000000
#include "wave_player.h"

void initJoyStick(void);

pthread_t* get_joystick_pthread(void);
char* get_beat(void);
void set_beat(char* arg);

void sleep_event(int num_milliseconds);

// desc: returns the address of the first epoll_event struct in the array of 
//       epoll_event structs. That epoll_event struct is used to store details of
//       the edge trigger.
// returns: pointer to the epoll_event struct array
struct epoll_event* get_epollStruct_addr(void);

// desc: returns a epoll_event struct from an array of epoll_event structs.
// param: the index of the epoll_event struct to be returned
// returns: epoll_ event struct
struct epoll_event get_epollStruct(int);

// desc: creates an epoll instance, opens GPIO files, configures epoll
// returns: -1 if error, 0 if success
int init_epoll(void);

// desc: calls init_epoll() and waits for input from user.
// returns: -1 if init_epoll() returns an error, otherwise details about the event trigger
int epoll_wait_input(void);

void* poll_joystick(void* args);

#endif