#include "udp.h"

// char* command_vol_up(void)
// {    
//     AudioMixer_setVolume(AudioMixer_getVolume() + 5);
//     // compose reply message
//     char buffer[] = "Received volume up command\n\0";
//     char* reply = malloc(strlen(buffer) + 1);
//     strcpy(reply,buffer);
//     return reply;
// }

// char* command_vol_down(void)
// {    
//     AudioMixer_setVolume(AudioMixer_getVolume() - 5);
//     // compose reply message
//     char buffer[] = "Received volume down command\n\0";
//     char* reply = malloc(strlen(buffer) + 1);
//     strcpy(reply,buffer);
//     return reply;
// }

// char* command_get_vol(void)
// {
//     // compose reply message
//     char buffer[] = "Received get volume command\n\0";
//     char* reply = malloc(strlen(buffer) + 1);
//     strcpy(reply,buffer);
//     return reply;
// }

// char* command_tempo_up(void)
// {
//     set_bpm(get_bpm() + 5);
//     char buffer[] = "Received tempo up command\n\0";
//     char* reply = malloc(strlen(buffer) + 1);
//     strcpy(reply,buffer);
//     return reply;
// }

// char* command_tempo_down(void)
// {
//     set_bpm(get_bpm() - 5);
//     char buffer[] = "Received tempo down command\n\0";
//     char* reply = malloc(strlen(buffer) + 1);
//     strcpy(reply,buffer);
//     return reply;
// }


// char* command_beat(char* mode)
// {
//     set_beat(mode);
//     char buffer[] = "Received change beat command\n\0";
//     char* reply = malloc(strlen(buffer) + 1);
//     strcpy(reply,buffer);
//     return reply;
// }


// char* command_stop(void)
// {
//     char buffer[] = "Successfully terminated program.\n\0";
//     char* reply = malloc(strlen(buffer) + 1);
//     strcpy(reply, buffer);
//     // stop all background threads
//     stop_sampling();
//     stop_displaying();
//     stop_outputting();
//     stop_listening();
//     return reply;
// }


char* call_command(char buffer[MAX_LENGTH])
{
    char temp_reply[] = "Received command\n\0";
    char* reply = malloc(strlen(temp_reply) + 1);
    memset(reply, 0, strlen(temp_reply) + 1);
    strcpy(reply, temp_reply);
    // call the appropriate command based on BUFFER
    if(strcmp(buffer, "vol_up") == 0){
        AudioMixer_setVolume(AudioMixer_getVolume() + 5);
        free(reply);
        char temp[MAX_LENGTH];
        sprintf(temp, "%d", AudioMixer_getVolume());
        reply = malloc(strlen(temp) + 1);
        memset(reply, 0, strlen(temp) + 1);
        strcpy(reply, temp);
    }
    else if((strcmp(buffer, "vol_down") == 0)){
        AudioMixer_setVolume(AudioMixer_getVolume() - 5);
        free(reply);
        char temp[MAX_LENGTH];
        sprintf(temp, "%d", AudioMixer_getVolume());
        reply = malloc(strlen(temp) + 1);
        memset(reply, 0, strlen(temp) + 1);
        strcpy(reply, temp);
    }
    else if((strcmp(buffer, "tempo_up") == 0)){
        set_bpm(get_bpm() + 5);
        free(reply);
        char temp[MAX_LENGTH];
        sprintf(temp, "%d", get_bpm());
        reply = malloc(strlen(temp) + 1);
        memset(reply, 0, strlen(temp) + 1);
        strcpy(reply, temp);
    }
    else if((strcmp(buffer, "tempo_down") == 0)){
        set_bpm(get_bpm() - 5);
        free(reply);
        char temp[MAX_LENGTH];
        sprintf(temp, "%d", get_bpm());
        reply = malloc(strlen(temp) + 1);
        memset(reply, 0, strlen(temp) + 1);
        strcpy(reply, temp);
    }
    else if(strcmp(buffer, "rock") == 0 || strcmp(buffer, "mine") == 0 || strcmp(buffer, "none") == 0){
        set_beat(buffer);
    }
    else if((strcmp(buffer, "bass") == 0)){
        // pthread_mutex_lock(get_mutex());
        queue_drum();
        // pthread_mutex_unlock(get_mutex());
    }
    else if((strcmp(buffer, "snare") == 0)){
        // pthread_mutex_lock(get_mutex());
        queue_snare();
        // pthread_mutex_unlock(get_mutex());
    }
    else if((strcmp(buffer, "hihat") == 0)){
        queue_hihat();
    }
    else if((strcmp(buffer, "vol_get") == 0)){
        free(reply);
        char temp[MAX_LENGTH];
        sprintf(temp, "%d", AudioMixer_getVolume());
        reply = malloc(strlen(temp) + 1);
        memset(reply, 0, strlen(temp) + 1);
        strcpy(reply, temp);
    }
    else if((strcmp(buffer, "tempo_get") == 0)){
        free(reply);
        char temp[MAX_LENGTH];
        sprintf(temp, "%d", get_bpm());
        reply = malloc(strlen(temp) + 1);
        memset(reply, 0, strlen(temp) + 1);
        strcpy(reply, temp);
    }
    else if((strcmp(buffer, "uptime") == 0)){
        free(reply);
        char temp[MAX_LENGTH];
        file_read(UPTIME_PATH, temp);
        reply = malloc(strlen(temp) + 1);
        memset(reply, 0, strlen(temp) + 1);
        strcpy(reply, temp);
    }
    else{
        free(reply);
        reply = NULL;
    }
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
        // command_stop();
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

        printf("%s\n", command);
        
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
