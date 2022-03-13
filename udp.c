#include "udp.h"

static pthread_t udpThreadId;


pthread_t* get_udp_pthread(void)
{
	return &udpThreadId;
}

char* call_command(char command[MAX_LENGTH])
{
    char* reply = NULL;
    char buffer[MAX_LENGTH] = {0};
    // call the appropriate command based on COMMAND
    if(strcmp(command, "vol_up") == 0){
        AudioMixer_setVolume(AudioMixer_getVolume() + 5);
        sprintf(buffer, "%d", AudioMixer_getVolume());
    }
    else if((strcmp(command, "vol_down") == 0)){
        AudioMixer_setVolume(AudioMixer_getVolume() - 5);
        sprintf(buffer, "%d", AudioMixer_getVolume());
    }
    else if((strcmp(command, "tempo_up") == 0)){
        set_bpm(get_bpm() + 5);
        sprintf(buffer, "%d", get_bpm());
    }
    else if((strcmp(command, "tempo_down") == 0)){
        set_bpm(get_bpm() - 5);
        sprintf(buffer, "%d", get_bpm());
    }
    // set the rhythm
    else if(strcmp(command, "rock") == 0 || strcmp(command, "mine") == 0 || strcmp(command, "none") == 0){
        set_beat(command);
        sprintf(buffer, "%s", get_beat());
    }
    else if(strcmp(command, "beat_get") == 0){
        sprintf(buffer, "%s", get_beat());
    }
    // queue a sound
    else if((strcmp(command, "bass") == 0)){
        queue_drum();
    }
    else if((strcmp(command, "snare") == 0)){
        queue_snare();
    }
    else if((strcmp(command, "hihat") == 0)){
        queue_hihat();
    }
    else if((strcmp(command, "vol_get") == 0)){
        sprintf(buffer, "%d", AudioMixer_getVolume());
    }
    else if((strcmp(command, "tempo_get") == 0)){
        sprintf(buffer, "%d", get_bpm());
    }
    // return the running time of the program in hours minutes and seconds
    else if((strcmp(command, "uptime") == 0)){
        file_read(UPTIME_PATH, buffer);
        char* ptr = buffer;
        while(!isspace(*ptr)){
            ptr++;
        }
        *ptr = '\0';
        int time = atof(buffer);
        int seconds = time % 60;
        time = time / 60;
        int minutes = time % 60;
        int hours = time / 60;
        sprintf(buffer, "%02d:%02d:%02d (H:M:S)", hours, minutes, seconds);
    }
    reply = malloc(strlen(buffer) + 1);
    memset(reply, 0, strlen(buffer) + 1);
    strcpy(reply, buffer);
    return reply;
}

struct arguements* initialize(void)
{
    // initialize structs in order to use functions getaddrinfo() and getnameinfo()
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    // UDP uses SOCK_DGRAM and IPPROTO_UDP
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE;
    struct arguements* args = malloc(sizeof(*args));
    args->localPort = PORT_NUMBER;
    // create the socket
    args->socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if(args->socketDescriptor == -1){
        perror("Error creating socket");
        return NULL;
    }
    // initialize the socket to receive incoming messages
    memset(&(args->socket_address), 0, sizeof(args->socket_address));
    args->socket_address.sin_family = AF_INET;
    args->socket_address.sin_addr.s_addr = INADDR_ANY;
    args->socket_address.sin_port = htons(args->localPort);
    if(bind(args->socketDescriptor, (struct sockaddr*)&(args->socket_address),
            sizeof(args->socket_address)) == -1){
        perror("Error binding socket");
        return NULL;
    }
    args->address_length = sizeof(args->socket_address);
    return args;
}

void* udp_listen(void* args)
{
    struct arguements* myArgs = initialize();
    // check if any of the operations in initialize() failed
    if(myArgs == NULL){
        free(myArgs);
        pthread_exit(NULL);
    }
    while(1){
        // wait for message
        char buffer[MAX_LENGTH];
        memset(&buffer, 0, MAX_LENGTH);
        recvfrom(myArgs->socketDescriptor, buffer, MAX_LENGTH, 0, 
                (struct sockaddr *) &(myArgs->socket_address), &(myArgs->address_length));
        char* command = malloc(strlen(buffer) + 1);
        strcpy(command, buffer);

        // get the response to the message
        char* reply = call_command(command);
        if(reply == NULL){
            char temp[] = "Error receiving command\n\0";
            reply = malloc(strlen(temp) + 1);
            strcpy(reply, temp);
        }
        // send the reply to the original sender.
        sendto(myArgs->socketDescriptor, reply, strlen(reply), 0, 
              (struct sockaddr *) &(myArgs->socket_address), myArgs->address_length);
        free(command);
        free(reply);
    }
    // clean up files and free allocated memory
    shutdown(myArgs->socketDescriptor, SHUT_RDWR);
    close(myArgs->socketDescriptor);
    free(myArgs);
    pthread_exit(NULL);
}
