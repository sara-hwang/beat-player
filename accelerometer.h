#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_
#define X_TRHESHOLD 1000
#define Y_TRHESHOLD 1000
#define Z_TRHESHOLD 1000

int initI2cBus(char* bus, int address);
void readI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char* buffer);
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
void* accelerometer_thread(void* args);

#endif