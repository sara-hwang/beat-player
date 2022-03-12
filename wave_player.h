/*
 *  Small program to read a 16-bit, signed, 44.1kHz wave file and play it.
 *  Written by Brian Fraser, heavily based on code found at:
 *  http://www.alsa-project.org/alsa-doc/alsa-lib/_2test_2pcm_min_8c-example.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <alloca.h>
#include "audioMixer_template.h"
#include "joystick.h"
#include "accelerometer.h"
#include "udp.h"
#include "files.h"

#define DRUM_FILE "./beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"
#define SNARE_FILE "./beatbox-wav-files/100059__menegass__gui-drum-snare-soft.wav"
#define HIHAT_FILE "./beatbox-wav-files/100053__menegass__gui-drum-cc.wav"
#define DEFAULT_BPM 120
#define MAX_BPM 300
#define MIN_BPM 40
#define NANOSECONDS_PER_MILLISECOND 1000000

void set_bpm(int new_bpm);
int get_bpm(void);
void queue_hihat(void);
void queue_snare(void);
void queue_drum(void);
void* play_beat(void*);
pthread_t* get_beat_pthread(void);
pthread_t* get_accelerometer_pthread(void);
pthread_t* get_udp_pthread(void);
void initialize_all(void);

void stop(void);