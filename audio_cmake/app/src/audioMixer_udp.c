#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "192.168.7.2"
#define SERVER_PORT 12345
#define PREV_MESSAGE_SIZE 200
#define MAX_BUFFER_SIZE 1501                        // 1500 bytes and 1 bytes for null pointer
#define WRITE_UP_TO (MAX_BUFFER_SIZE - 2)           // Last - \0 & second last - \n

//flag
static int *isTerminated;

//Sokcet setup
static int serverSock;
static char previousMessage[PREV_MESSAGE_SIZE];
static int previousMessageSize;

//Response message
static const char *responseMessage;
//static char command_buffer[MAX_BUFFER_SIZE];

//Thread
static pthread_t udpSever_id;

//Declare functions
void *UDP_serverThread();
const char *UDP_commandBeat(void);
const char *UDP_commandVolume(void);
const char *UDP_commandTempo(void);
const char *UDP_commandDrum(void);
const char *UDP_commandTerminate(void);

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
    memset(previousMessage, 0, sizeof(previousMessage));
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

        //Add null terminate
        receiv_buffer[recv_len] = '\0'; 
        
        // Message is not empty -> update previous; otherwise leave it as is
        if(recv_len != 1)
        {
            memset((char *)&previousMessage, 0, sizeof(previousMessage));
            memcpy(previousMessage, receiv_buffer, recv_len - 1);
            previousMessageSize = recv_len;
            previousMessage[previousMessageSize - 1] = '\0';
        }

        // Execute command according to request from client
        if(strcmp("beat", previousMessage))
        {
            responseMessage = UDP_commandBeat();
        } 
        else if (strcmp("volume", previousMessage) == 0)
        {
            responseMessage = UDP_commandVolume();
        }
        else if (strcmp("tempo", previousMessage) == 0)
        {
            responseMessage = UDP_commandTempo();
        }
        else if (strcmp("drum", previousMessage) == 0)
        {
            responseMessage = UDP_commandDrum();
        }
        else if (strcmp("terminate", previousMessage) == 0)
        {
            responseMessage = UDP_commandTerminate();
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

const char *UDP_commandBeat(void)
{
    printf("Hi Beat");
    return "Hi Beat";
}

const char *UDP_commandVolume(void)
{
    printf("Hi Volume");
    return "Hi Volume\n";
}

const char *UDP_commandTempo(void)
{
    printf("Hi Tempo");
    return "Hi Tempo\n";
}

const char *UDP_commandDrum(void)
{
    printf("Hi Drum");
    return "Hi Drum\n";
}

const char *UDP_commandTerminate(void)
{
    *isTerminated = 1;
    return NULL;
}

