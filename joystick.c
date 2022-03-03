#include "joystick.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>		// Errors
#include <time.h>
#include <sys/epoll.h>  // for epoll()
#include <fcntl.h>      // for open()
#include <unistd.h>     // for close()
#include <pthread.h>
#include "files.h"

static pthread_t joystickThreadId;

static char* pin_numbers[NUM_EVENTS] = {"65", "47", "26", "46", "27"};
static char* beats[NUM_BEATS] = {"none", "rock", "mine"};
static int beat_index = 0;

pthread_t* get_joystick_pthread(void)
{
	return &joystickThreadId;
}

char* get_beat(void)
{
	return beats[beat_index];
}

void set_beat(char* name)
{
	for(int i = 0; i < NUM_BEATS; i++){
		if(strncmp(beats[i], name, BEAT_ID_LENGTH) == 0){
			beat_index = i;
			return;
		}
	}
}

// Sleeps for NUMBER number of milliseconds.
void sleep_event(int num_milliseconds)
{
	long seconds = 0;
	long nanoseconds = NANOSECONDS_PER_MILLISECOND;
	struct timespec reqDelay = {seconds, nanoseconds*num_milliseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

void initJoyStick(void)
{
	// export pins
	for(int i = 0; i < NUM_EVENTS; i++){
		writeToFile(EXPORT_FILE, pin_numbers[i]);
	}
	sleep_event(330);
	char buffer[MAX_LENGTH];
	memset(buffer, '\0', MAX_LENGTH);
	for(int i = 0; i < NUM_EVENTS; i++){
		// set each GPIO's direction attribute to "in" for input
		strcpy(buffer, get_path(i));
		strcat(buffer, "/direction");
		writeToFile(buffer, "in");
		memset(buffer, '\0', MAX_LENGTH);
		// set each GPIO's edge attribute to "both"
		strcpy(buffer, get_path(i));
		strcat(buffer, "/edge");
		writeToFile(buffer, "both");
		memset(buffer, '\0', MAX_LENGTH);
	}
	pthread_create(&joystickThreadId, NULL, poll_joystick, NULL);			 
}

static struct epoll_event epollStruct[NUM_EVENTS];

struct epoll_event* get_epollStruct_addr(void)
{
	return epollStruct;
}

struct epoll_event get_epollStruct(int index)
{
	return epollStruct[index];
}

int init_epoll(void)
{
	// create an epoll instance
	// .. kernel ignores size argument; must be > 0
	set_fd(EPOLL_FD_INDEX, epoll_create(1));
	if (get_fd(EPOLL_FD_INDEX) == -1) {
		fprintf(stderr, "ERROR: epoll_create() returned = %s\n", strerror(errno));
		return -1;
	}

	char buffer[MAX_LENGTH];
	memset(buffer, '\0', MAX_LENGTH);

	for(int i = 0; i < NUM_EVENTS; i++){
		// open GPIO value files:
		strcpy(buffer, get_path(i));
		strcat(buffer, "/value");
		set_fd(i, open(buffer, O_RDONLY | O_NONBLOCK));
		memset(buffer, '\0', MAX_LENGTH);
		// check if GPIO value files opened successfully
		if (get_fd(i) == -1){
			printf("%d\n", i);
			fprintf(stderr, "ERROR: unable to open() GPIO value file (%d) = %s\n", get_fd(i), strerror(errno));
			return -1;	
		}
		// configure epoll to wait for events: read operation | edge triggered | urgent data
		epollStruct[i].events = EPOLLIN | EPOLLET | EPOLLPRI;
		epollStruct[i].data.fd = get_fd(i);
		// register file descriptor on the epoll instance, see: man epoll_ctl
		if (epoll_ctl(get_fd(EPOLL_FD_INDEX), EPOLL_CTL_ADD, get_fd(i), &epollStruct[i]) == -1){
			fprintf(stderr, "ERROR: epoll_ctl() failed on add control interface = %s\n", strerror(errno));
			return -1;
		}
	}
	return 0;
}

int epoll_wait_input(void)
{
	// initialize epoll
	if(init_epoll() == -1){
		return -1;
	}
	// ignore first 2 triggers
	for(int i = 0; i <= NUM_TRIGGERS_IGNORED; i++) {
		// information gets returned in the epoll struct passed in second param
		int waitRet = epoll_wait(get_fd(EPOLL_FD_INDEX), get_epollStruct_addr(), NUM_EVENTS, -1);
		if (waitRet == -1){
			fprintf(stderr, "ERROR: epoll_wait() failed (%d) = %s\n", waitRet, strerror(errno));
			close_fd();
			return -1;
		}
	}
	close_fd();
	return get_epollStruct(INDEX_W_DATA).data.fd;
}

void* poll_joystick(void* args)
{
	enum direction {left=0, right=1, up=2, down=3, in=4};
	while(1){
		// Wait for an edge trigger:
		enum direction joystick = epoll_wait_input();
		if (joystick == -1) {
			exit(EXIT_FAILURE);
		}
		else if(joystick == get_fd(in)){
			beat_index = (beat_index + 1) % NUM_BEATS;
		}
		else if(joystick == get_fd(left)) {
            set_bpm(get_bpm() - 5);
		}
		else if(joystick == get_fd(right)) {
			set_bpm(get_bpm() + 5);
		}
		else if(joystick == get_fd(up)){
			AudioMixer_setVolume(AudioMixer_getVolume() + 5);
		}
        else if(joystick == get_fd(down)){
			AudioMixer_setVolume(AudioMixer_getVolume() - 5);
        }
    }
	return NULL;
}