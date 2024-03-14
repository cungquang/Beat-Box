#include "../include/audioMixer_upd.h"

#define SERVER_IP "192.168.7.2"
#define SERVER_PORT 12345
#define MAX_BUFFER_SIZE 1501                        // 1500 bytes and 1 bytes for null pointer
#define WRITE_UP_TO (MAX_BUFFER_SIZE - 2)           // Last - \0 & second last - \n
#define MAX_PARTS 3                                 // Number of parts from request message

#define TARGET_IP "192.168.7.2"
#define TARGET_PORT 8087

//flag
static int isTerminated;

//Sokcet setup
static int serverSock;
static int targetSock;
static struct sockaddr_in target_addr;

//Response message
static const char *responseMessage;
static char tempBuffer[MAX_BUFFER_SIZE];

//Thread
static pthread_t udpSever_id;

//Mutex
static pthread_mutex_t sendMutex = PTHREAD_MUTEX_INITIALIZER;

//Declare functions
//void setupForSendingMessage();
void *UDP_serverThread();
void setupForSendingMessage();
static void splitStringToParts(char *input, char *intoParts[]);
static void UDP_commandBeat(int value);
static const char *UDP_commandVolume(const char* target);
static const char *UDP_commandTempo(const char* target);
static void UDP_commandDrum(int value);
static void UDP_commandShowError(void);
static void UDP_commandTerminate(int value);


/*
#############################
#           PUBLIC          #
#############################
*/



void UDP_join(void)
{
    pthread_join(udpSever_id, NULL);
}

void UDP_cleanup(void) 
{
    if(serverSock) {
        close(serverSock);
    }

    if(targetSock) {
        close(targetSock);
    }
}

void UDP_initServer()
{
    //Setup operational flag
    isTerminated = 0;

    //Setup for sending message
    setupForSendingMessage();

    //Run server thread
    pthread_create(&udpSever_id, NULL, UDP_serverThread, NULL);
}


void UDP_sendToTarget(char *message)
{
    //critical section
    pthread_mutex_lock(&sendMutex);

    //Check if error in initialization
    if (targetSock < 0) {
        fprintf(stderr, "Socket not initialized.\n");
        exit(EXIT_FAILURE);
    }

    //Send the message
    ssize_t bytes_sent = sendto(targetSock, message, strlen(message), 0, (struct sockaddr *)&target_addr, sizeof(target_addr));
    if (bytes_sent < 0) {
        perror("sendto");
        close(targetSock);
        exit(EXIT_FAILURE);
    }
    pthread_mutex_unlock(&sendMutex);
}



/*
#############################
#           PRIVATE         #
#############################
*/


//Setup for sending message
void setupForSendingMessage()
{
    // Create a UDP socket
    targetSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (targetSock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //Setup target socket
    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(TARGET_PORT);

    //Fail to setup
    if (inet_aton(TARGET_IP, &target_addr.sin_addr) == 0) {
        perror("inet_aton");
        close(targetSock);
        exit(EXIT_FAILURE);
    }
}


//Server side, receive: history, count, length, dips, help (or ?), stop, <Enter>
void *UDP_serverThread()
{
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    int recv_len;
    char receiv_buffer[MAX_BUFFER_SIZE];
    char *msgParts[MAX_PARTS];

    // Create a UDP socket
    if ((serverSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Setup receiver addr
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);            //receive from any ip - later need to specify with host IP

    // Bind the socket to Receiver address
    if (bind(serverSock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    //Print server start
    printf("Server starting...\n");

    while(!isTerminated)
    {
        // Receive message
        if ((recv_len = recvfrom(serverSock, receiv_buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        receiv_buffer[recv_len] = '\0'; 

        //Break the string into part
        //printf("msg: %s\n", receiv_buffer);
        splitStringToParts(receiv_buffer, msgParts);

        // Execute command according to request from client
        if(strcmp("beat", msgParts[0]) == 0)
        {
            UDP_commandBeat(atoi(msgParts[2]));
        } 
        else if (strcmp("volume", msgParts[0]) == 0)
        {
            responseMessage = UDP_commandVolume(msgParts[1]);
        }
        else if (strcmp("tempo", msgParts[0]) == 0)
        {
            responseMessage = UDP_commandTempo(msgParts[1]);
        }
        else if (strcmp("drum", msgParts[0]) == 0)
        {
            UDP_commandDrum(atoi(msgParts[2]));
        }
        else if (strcmp("show_error", msgParts[0]) == 0)
        {
            UDP_commandShowError();
        }
        else if (strcmp("terminate", msgParts[0]) == 0)
        {
            UDP_commandTerminate(atoi(msgParts[2]));
        }

        // Reply to the sender
        if(responseMessage)
        {
            if(sendto(serverSock, responseMessage, strlen(responseMessage), 0, (struct sockaddr *)&client_addr, client_len) == -1)
            {
                perror("Fail to send");
                exit(EXIT_FAILURE);
            }
            responseMessage = NULL;
        }
    }

    // Close the socket
    close(serverSock);
    return NULL;
}


static void UDP_commandBeat(int value)
{
    AudioMixerControl_controlBeat(value);
}

static const char *UDP_commandVolume(const char* target)
{
    memset(tempBuffer, 0, sizeof(tempBuffer));
    if(strcmp(target, "up") == 0)
    {
        AudioMixerControl_setVolume(AudioMixerControl_getVolume() + 5);
    }
    else 
    {
        AudioMixerControl_setVolume(AudioMixerControl_getVolume() - 5);
    }

    //Write message to send
    snprintf(tempBuffer, sizeof(tempBuffer), "volume_%s,%d", target, AudioMixerControl_getVolume());
    return tempBuffer;
}

static const char *UDP_commandTempo(const char* target)
{
    memset(tempBuffer, 0, sizeof(tempBuffer));
    if(strcmp(target, "up") == 0)
    {
        AudioMixer_setTempo(AudioMixer_getTempo() + 5);
    }
    else 
    {
        AudioMixer_setTempo(AudioMixer_getTempo() - 5);
    }

    //Write message to send
    snprintf(tempBuffer, sizeof(tempBuffer), "tempo_%s,%d", target, AudioMixer_getTempo());
    return tempBuffer;
}

static void UDP_commandDrum(int value)
{
    AudioMixerControl_addDrum(value);
}

static void UDP_commandShowError(void)
{
    memset(tempBuffer, 0, sizeof(tempBuffer));
    snprintf(tempBuffer, sizeof(tempBuffer), "show_error,hide");
    UDP_sendToTarget(tempBuffer);
}

static void UDP_commandTerminate(int value)
{
    isTerminated = value;
    //Stop audio mixer
    AudioMixer_stop();
    JoystickControl_terminate();
    AudioMixerControl_terminate();
    ProcessTime_terminate();
}


//////////////////////////////////////// Support ////////////////////////////////////////

// Source: chatGPT
static void splitStringToParts(char *input, char *intoParts[]) {
    char *token;
    int partNum = 0;
    for (int i = 0; i < MAX_PARTS; i++) {
        intoParts[i] = NULL;
    }

    // Get the first token
    token = strtok(input, ",");

    // Continue getting tokens until NULL or 3 parts are found
    while (token != NULL && partNum < 3) {
        intoParts[partNum] = token;
        token = strtok(NULL, ",");
        partNum++;
    }

    // If there are fewer than three parts, set the remaining parts as NULL
    while (partNum < 3) {
        intoParts[partNum] = NULL;
        partNum++;
    }
}
