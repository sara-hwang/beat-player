#include "wave_player.h"

static pthread_t beatThreadId;
static wavedata_t drum;
static wavedata_t snare;
static wavedata_t hihat;
static int bpm = DEFAULT_BPM;

void initialize_all(void)
{
	AudioMixer_init();
	initJoyStick();
	pthread_create(&beatThreadId, NULL, play_beat, NULL);			 
	pthread_create(get_accelerometer_pthread(), NULL, accelerometer_thread, NULL);
	pthread_create(get_udp_pthread(), NULL, udp_listen, NULL);					 
	// Load wave files we want to play:
	AudioMixer_readWaveFileIntoMemory(DRUM_FILE, &drum);
	AudioMixer_readWaveFileIntoMemory(SNARE_FILE, &snare);
	AudioMixer_readWaveFileIntoMemory(HIHAT_FILE, &hihat);
}

void set_bpm(int new_bpm)
{
	if(new_bpm <= MAX_BPM && new_bpm >= MIN_BPM){
		bpm = new_bpm;
	}
	else{
		printf("BPM must be in range [%d, %d]\n", MIN_BPM, MAX_BPM);
	}
}

int get_bpm(void)
{
	return bpm;
}

pthread_t* get_beat_pthread(void)
{
	return &beatThreadId;
}

void queue_hihat(void)
{
	pthread_mutex_lock(get_mutex());
	AudioMixer_queueSound(&hihat);
	pthread_mutex_unlock(get_mutex());
}

void queue_snare(void)
{
	pthread_mutex_lock(get_mutex());
	AudioMixer_queueSound(&snare);
	pthread_mutex_unlock(get_mutex());
}

void queue_drum(void)
{
	pthread_mutex_lock(get_mutex());
	AudioMixer_queueSound(&drum);
	pthread_mutex_unlock(get_mutex());
}

void* play_beat(void* args)
{
	// Store the sounds in an array so we can loop through and play them 
	wavedata_t* rock_beat[8] = {&drum, NULL, &snare, NULL, &drum, NULL, &snare, NULL};
	wavedata_t* my_beat[8] = {&drum, &hihat, &hihat, &snare, &drum, &hihat, &hihat, &snare};
	while(1){
		// if the selected mode is rock
		for(int i = 0; i < 8 && (strncmp(get_beat(), "rock", BEAT_ID_LENGTH) == 0); i++){
			pthread_mutex_lock(get_mutex());
			AudioMixer_queueSound(&hihat);
			pthread_mutex_unlock(get_mutex());
			if(rock_beat[i] != NULL){
				pthread_mutex_lock(get_mutex());
				AudioMixer_queueSound(rock_beat[i]);
				pthread_mutex_unlock(get_mutex());
			}
			// sleep for half a beat
			sleep_event(30000/bpm);
		}
		// if the selected mode is the custom beat
		for(int i = 0; i < 8 && (strncmp(get_beat(), "mine", BEAT_ID_LENGTH) == 0); i++){
			pthread_mutex_lock(get_mutex());
			AudioMixer_queueSound(&drum);
			AudioMixer_queueSound(my_beat[i]);
			pthread_mutex_unlock(get_mutex());
			// sleep for half a beat
			sleep_event(30000/bpm);
		}
	}
	return NULL;
}
