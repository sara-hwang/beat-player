// Contains all functions necessary to operate the accelerometer.

#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_
#include "wave_player.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define NUM_BYTES_TO_READ 7
#define I2C_DEVICE_ADDRESS 0x1C
#define CTRL_REG1 0x2A
#define STATUS 0x00
#define OUT_X_MSB 0x01
#define OUT_X_LSB 0x02
#define OUT_Y_MSB 0x03
#define OUT_Y_LSB 0x04
#define OUT_Z_MSB 0x05
#define OUT_Z_LSB 0x06
#define X_TRHESHOLD 7000
#define Y_TRHESHOLD 7000
#define Z_TRHESHOLD 6000

// Returns the address of the local pthread.
pthread_t* get_accelerometer_pthread(void);

// Initialize I2C bus.
// Returns I2C file descriptor.
int initI2cBus(char* bus, int address);

// Read from REGADDR through file descriptor I2CFILEDESC and copy contents to BUFFER.
void readI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char* buffer);

// Write VALUE to REGADDR through file descriptor I2CFILEDESC.
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);

// Function which the pthread loops through.
//
// Calls initI2C and loops through reading accelerometer values and queueing sounds 
// when significant movement in one direction is detected.
void* accelerometer_thread(void* args);

#endif