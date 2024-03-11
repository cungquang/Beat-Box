#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "192.168.7.2"
#define SERVER_PORT 12345
#define MAX_BUFFER_SIZE 1501                        // 1500 bytes and 1 bytes for null pointer
#define WRITE_UP_TO (MAX_BUFFER_SIZE - 2)           // Last - \0 & second last - \n
#define MAX_PARTS 3                                 // Number of parts from request message

//flag
static int *isTerminated;

//Sokcet setup
static int serverSock;

//Response message
static const char *responseMessage;

//Thread
static pthread_t udpSever_id;

//Declare functions
void *UDP_serverThread();

static bool stringToBoolean(const char *input);
static void splitStringToParts(char *input, char *intoParts[]);
static const char *UDP_commandBeat(const char* target, bool value);
static const char *UDP_commandVolume(const char* target, int value);
static const char *UDP_commandTempo(const char* target, int value);
static const char *UDP_commandDrum(const char* target, bool value);
static const char *UDP_commandTerminate(const char* target, bool value);

/*-------------------------- Public -----------------------------*/

void UDP_join(void)
{
    pthread_join(udpSever_id, NULL);
}

void UDP_cleanup(void) 
{
    if(serverSock) {
        close(serverSock);
    }

    isTerminated = NULL;
}

void UDP_initServer(int *terminate_flag)
{
    isTerminated = terminate_flag;

    //Create thread
    pthread_create(&udpSever_id, NULL, UDP_serverThread, NULL);
}


/*-------------------------- Private -----------------------------*/

//Server side, receive: history, count, length, dips, help (or ?), stop, <Enter>
void *UDP_serverThread()
{
    printf("Setup server...\n");
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

    while(*isTerminated == 0)
    {
        // Receive message
        if ((recv_len = recvfrom(serverSock, receiv_buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        receiv_buffer[recv_len] = '\0'; 

        //Break the string into part
        splitStringToParts(receiv_buffer, msgParts);

        // Execute command according to request from client
        if(strcmp("beat", msgParts[0]) == 0)
        {
            responseMessage = UDP_commandBeat(msgParts[1], stringToBoolean(msgParts[2]));
        } 
        else if (strcmp("volume", msgParts[0]) == 0)
        {
            responseMessage = UDP_commandVolume(msgParts[2], atoi(msgParts[2]));
        }
        else if (strcmp("tempo", msgParts[0]) == 0)
        {
            responseMessage = UDP_commandTempo(msgParts[2], atoi(msgParts[2]));
        }
        else if (strcmp("drum", msgParts[0]) == 0)
        {
            responseMessage = UDP_commandDrum(msgParts[2], stringToBoolean(msgParts[2]));
        }
        else if (strcmp("terminate", msgParts[0]) == 0)
        {
            responseMessage = UDP_commandTerminate(msgParts[2], atoi(msgParts[2]));
        }


        // Reply to the sender
        if(responseMessage)
        {
            if(sendto(serverSock, responseMessage, strlen(responseMessage), 0, (struct sockaddr *)&client_addr, client_len) == -1)
            {
                perror("Fail to send");
                exit(EXIT_FAILURE);
            }

            //reset responseMessage
            responseMessage = NULL;
        }
    }

    // Close the socket
    close(serverSock);
    return NULL;
}

// Source chatGPT
static void splitStringToParts(char *input, char *intoParts[]) {
    char *token;
    int partNum = 0;

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

static bool stringToBoolean(const char *input) 
{
    if(strcmp(input, "true") == 0)
    {
        return true;
    } 
    else
    {
        return false;
    }
}

static const char *UDP_commandBeat(const char* target, bool value)
{
    // Call and upgrade audioMixer_template
    printf("Message: %s - %d\n", target, value);
    return "Hi Beat";
}

static const char *UDP_commandVolume(const char* target, int value)
{
    printf("Message: %s - %d\n", target, value);
    return "Hi Volume\n";
}

static const char *UDP_commandTempo(const char* target, int value)
{
    printf("Message: %s - %d\n", target, value);
    return "Hi Tempo\n";
}

static const char *UDP_commandDrum(const char* target, bool value)
{
    printf("Message: %s - %d\n", target, value);
    return "Hi Drum\n";
}

static const char *UDP_commandTerminate(const char* target, bool value)
{
    printf("Message: %s - %d\n", target, value);
    *isTerminated = 1;
    return NULL;
}



