#include "wave_player.h"

int main(void)
{
    // spawn the pthreads
    initialize_all();
    // wait for pthreads to finish executing
    if(pthread_join(*get_joystick_pthread(), NULL) != 0 ||
       pthread_join(*get_playback_pthread(), NULL) != 0 ||
       pthread_join(*get_beat_pthread(), NULL) != 0 ||
       pthread_join(*get_accelerometer_pthread(), NULL) != 0 ||
       pthread_join(*get_udp_pthread(), NULL) != 0){
        perror("Error joining threads");
        return(-1);
    }
    return 0;
}