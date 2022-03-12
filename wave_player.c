#include "wave_player.h"

static pthread_t beatThreadId;
static pthread_t accelerometerThreadId;
static pthread_t udpThreadId;
static wavedata_t drum;
static wavedata_t snare;
static wavedata_t hihat;
static int bpm = DEFAULT_BPM;

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

pthread_t* get_accelerometer_pthread(void)
{
	return &accelerometerThreadId;
}

pthread_t* get_udp_pthread(void)
{
	return &udpThreadId;
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
	wavedata_t* rock_beat[8] = {&drum, NULL, &snare, NULL, &drum, NULL, &snare, NULL};
	wavedata_t* my_beat[8] = {&drum, &hihat, &hihat, &snare, &drum, &hihat, &hihat, &snare};
	while(1){
		for(int i = 0; i < 8 && (strncmp(get_beat(), "rock", BEAT_ID_LENGTH) == 0); i++){
			pthread_mutex_lock(get_mutex());
			AudioMixer_queueSound(&hihat);
			pthread_mutex_unlock(get_mutex());
			if(rock_beat[i] != NULL){
				pthread_mutex_lock(get_mutex());
				AudioMixer_queueSound(rock_beat[i]);
				pthread_mutex_unlock(get_mutex());
			}
			sleep_event(30000/bpm);
		}
		for(int i = 0; i < 8 && (strncmp(get_beat(), "mine", BEAT_ID_LENGTH) == 0); i++){
			pthread_mutex_lock(get_mutex());
			AudioMixer_queueSound(&drum);
			AudioMixer_queueSound(my_beat[i]);
			pthread_mutex_unlock(get_mutex());
			sleep_event(30000/bpm);
		}
		
	}
	return NULL;
}

void initialize_all(void)
{
	// Configure Output Device
	AudioMixer_init();
	initJoyStick();
	pthread_create(&beatThreadId, NULL, play_beat, NULL);			 
	pthread_create(&accelerometerThreadId, NULL, accelerometer_thread, NULL);
	pthread_create(&udpThreadId, NULL, udp_listen, NULL);					 
	// Load wave file we want to play:
	AudioMixer_readWaveFileIntoMemory(DRUM_FILE, &drum);
	AudioMixer_readWaveFileIntoMemory(SNARE_FILE, &snare);
	AudioMixer_readWaveFileIntoMemory(HIHAT_FILE, &hihat);
}

void stop(void)
{
	AudioMixer_cleanup();
	AudioMixer_freeWaveFileData(&drum);
	AudioMixer_freeWaveFileData(&snare);
	AudioMixer_freeWaveFileData(&hihat);
}
