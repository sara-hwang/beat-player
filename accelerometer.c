#include "accelerometer.h"
#include "wave_player.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

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

int initI2cBus(char* bus, int address)
{
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
    return i2cFileDesc;
}

void readI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char* buffer)
{
    // To read a register, must first write the address
    if (write(i2cFileDesc, &regAddr, sizeof(regAddr)) != sizeof(regAddr)) {
        perror("I2C: Unable to write to i2c register.");
        exit(1);
    }
    // read the values
    if (read(i2cFileDesc, buffer, NUM_BYTES_TO_READ) != NUM_BYTES_TO_READ) {
        perror("I2C: Unable to read from i2c register");
        exit(1);
    }
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    if (write(i2cFileDesc, buff, 2) != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

void* accelerometer_thread(void* args)
{
    int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    writeI2cReg(i2cFileDesc, CTRL_REG1, 1);
    unsigned char* buffer = malloc(NUM_BYTES_TO_READ);
    int16_t prev_x = 0;
    int16_t prev_y = 0;
    int16_t prev_z = 0;    
    int x_threshold = 1000;
    int y_threshold = 1000;
    int z_threshold = 1000;
    while(1){
        // Read a register:
        memset(buffer, 0, NUM_BYTES_TO_READ);
        readI2cReg(i2cFileDesc, STATUS, buffer);
        int16_t x = (buffer[OUT_X_MSB] << 8) | (buffer[OUT_X_LSB]);
        int16_t y = (buffer[OUT_Y_MSB] << 8) | (buffer[OUT_Y_LSB]);
        int16_t z = (buffer[OUT_Z_MSB] << 8) | (buffer[OUT_Z_LSB]);
        if(abs(prev_x - x) > x_threshold){
            //queue_hihat();
        }
        if(abs(prev_y - y) > y_threshold){
            //queue_snare();
        }
        if(abs(prev_z - z) > z_threshold){
            //queue_drum();
        }
        prev_x = x;
        prev_y = y;
        prev_z = z;
        sleep_event(10);
    }
    // Cleanup I2C access;
    close(i2cFileDesc);
    return NULL;
}