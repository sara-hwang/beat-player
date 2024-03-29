#include "accelerometer.h"


static pthread_t accelerometerThreadId;


pthread_t* get_accelerometer_pthread(void)
{
	return &accelerometerThreadId;
}

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
    // initialize I2C
    int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    // set accelerometer to active mode
    writeI2cReg(i2cFileDesc, CTRL_REG1, 1);
    unsigned char* buffer = malloc(NUM_BYTES_TO_READ);
    memset(buffer, 0, NUM_BYTES_TO_READ);
    // initialize x, y, and z values
    readI2cReg(i2cFileDesc, STATUS, buffer);
    int16_t prev_x = (buffer[OUT_X_MSB] << 8) | (buffer[OUT_X_LSB]);
    int16_t prev_y = (buffer[OUT_Y_MSB] << 8) | (buffer[OUT_Y_LSB]);
    int16_t prev_z = (buffer[OUT_Z_MSB] << 8) | (buffer[OUT_Z_LSB]);
    while(1){
        // Read a register:
        memset(buffer, 0, NUM_BYTES_TO_READ);
        readI2cReg(i2cFileDesc, STATUS, buffer);
        int16_t x = (buffer[OUT_X_MSB] << 8) | (buffer[OUT_X_LSB]);
        int16_t y = (buffer[OUT_Y_MSB] << 8) | (buffer[OUT_Y_LSB]);
        int16_t z = (buffer[OUT_Z_MSB] << 8) | (buffer[OUT_Z_LSB]);
        bool movement_detected = false;
        // check if significant movement in one direction
        if(abs(prev_x - x) > X_TRHESHOLD){
            queue_hihat();
            movement_detected = true;
        }
        else if(abs(prev_y - y) > Y_TRHESHOLD){
            queue_snare();
            movement_detected = true;
        }
        else if(abs(prev_z - z) > Z_TRHESHOLD){
            queue_drum();
            movement_detected = true;
        }
        if(movement_detected){
            sleep_event(30000/get_bpm());
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
