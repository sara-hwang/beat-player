#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

int initI2cBus(char* bus, int address);
void readI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char* buffer);
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
void* accelerometer_thread(void* args);

#endif