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

// Returns the address of the local pthread.
pthread_t* get_beat_pthread(void);

// Spawns all the other threads necessary for program execution: thread for queue sounds, 
// accelerometer thread, joystick thread, and UDP thread. Also initializes the audio mixer 
// and loads the needed wav files.
void initialize_all(void);

// Set the BPM to NEW_BPM if NEW_BPM is within the range [MIN_BPM, MAX_BPM].
void set_bpm(int new_bpm);

// Returns the current BPM.
int get_bpm(void);

// Queue a hihat sound to be played.
void queue_hihat(void);

// Queue a snare sound to be played.
void queue_snare(void);

// Queue a drum sound to be played.
void queue_drum(void);

// Function that the pthread loops through. Plays a rhythm depending on the mode that is selected.
void* play_beat(void*);
