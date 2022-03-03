// file operations, file descriptors, and operations on file descriptors are 
// declared here.

#ifndef FILES_H_
#define FILES_H_

#include "joystick.h"

// desc: returns the desired file descriptor from an array of active file 
//       descriptors
// param: index of the file descriptor to be returned
// returns: file descriptor
int get_fd(int);

// desc: set the value of a file descriptor
// param: index of the file descriptor to be modified, the new value of the 
//        file descriptor
void set_fd(int, int);

// desc: returns a GPIO file path from an array of GPIO file paths for joystick
//       directions: up, down, left, right, centre
// param: the index of file path to be returned
// returns: GPIO value file path
char* get_path(int);

// desc: read a line from a file
// param: name of the file to be read from, the buffer to store the value
// returns: number of bytes read
int readFromFile(char*, char*);

// desc: write a string to a file
// param: name of the file to be written to, the string to write
void writeToFile(char*, char*);

// desc: close all active file descriptors
void close_fd(void);

#endif