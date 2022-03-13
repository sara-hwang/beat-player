// Playback sounds in real time, allowing multiple simultaneous wave files
// to be mixed together and played without jitter.

#ifndef AUDIO_MIXER_H
#define AUDIO_MIXER_H
#include "wave_player.h"

typedef struct {
	int numSamples;
	short* pData;
} wavedata_t;

#define AUDIOMIXER_MAX_VOLUME 100
// Currently active (waiting to be played) sound bites
#define MAX_SOUND_BITES 30
#define DEFAULT_VOLUME 80
#define SAMPLE_RATE 44100
#define NUM_CHANNELS 1
#define SAMPLE_SIZE (sizeof(short)) 			// bytes per sample
// Sample size note: This works for mono files because each sample ("frame') is 1 value.
// If using stereo files then a frame would be two samples.

// Returns the address of the local mutex
pthread_mutex_t* get_mutex(void);

// init() must be called before any other functions,
// cleanup() must be called last to stop playback threads and free memory.
void AudioMixer_init(void);
void AudioMixer_cleanup(void);

// Function which the pthread loops through.
//
// Continuously loops through filling the output buffer from the queue and outputting sounds.
void* playbackThread(void* arg);

// Returns the address of the local pthread.
pthread_t* get_playback_pthread(void);

// Read the contents of a wave file into the pSound structure. Note that
// the pData pointer in this structure will be dynamically allocated in
// readWaveFileIntoMemory(), and is freed by calling freeWaveFileData().
void AudioMixer_readWaveFileIntoMemory(char *fileName, wavedata_t *pSound);
void AudioMixer_freeWaveFileData(wavedata_t *pSound);

// Queue up another sound bite to play as soon as possible.
void AudioMixer_queueSound(wavedata_t *pSound);

// Get/set the volume.
// setVolume() function posted by StackOverflow user "trenki" at:
// http://stackoverflow.com/questions/6787318/set-alsa-master-volume-from-c-code
int  AudioMixer_getVolume();
void AudioMixer_setVolume(int newVolume);

#endif
