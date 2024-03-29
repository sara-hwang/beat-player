#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int fd[NUM_EVENTS+1];
static char* paths[NUM_EVENTS] = {"/sys/class/gpio/gpio65", // left
								  "/sys/class/gpio/gpio47", // right
								  "/sys/class/gpio/gpio26", // up
								  "/sys/class/gpio/gpio46", // down
								  "/sys/class/gpio/gpio27"}; // centre

int get_fd(int index)
{
    return fd[index];
}

void set_fd(int index, int file_descriptor)
{
    fd[index] = file_descriptor;
}

char* get_path(int index)
{
	return paths[index];
}

void file_write(char *fileName, char* value)
{
    // Use fopen() to open the file for write access.
    FILE *pFile = fopen(fileName, "w");
    if (pFile == NULL) {
        printf("ERROR: Unable to open export file.\n");
        exit(1);
    }
    // Write to data to the file using fprintf():
	fprintf(pFile, "%s", value);
    // Close the file using fclose():
    fclose(pFile);
}

int file_read(char* fileName, char* buff)
{
    // open file
    FILE* pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }
    // Read string (line)
    unsigned int max_length = MAX_LENGTH;
	int bytes_read = getline(&buff, &max_length, pFile);
    // Close file
    fclose(pFile);
	return bytes_read;
}

void close_fd(void)
{
	for(int i = 0; i <= NUM_EVENTS; i++){
		close(fd[i]);
	}
}
