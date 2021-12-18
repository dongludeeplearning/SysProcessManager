#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../common/SysConfigReader.h"
#define BUFLEN 512


// simple routine to print an error and exit
void PrintErrorAndExit(char *lanErrorString)
{
    perror(lanErrorString);
    exit(1);
}


int client(char* addr, int port)
{
    // socket structure to define properties
    struct sockaddr_in lsSAOther;

    // place to store socket handle
    int lnSocketId = 0;
    socklen_t lnSockStructLen=sizeof(lsSAOther);
    char lanRecBuf[BUFLEN] = {0};
    char lanMessage[BUFLEN] = {0};

    // attempt to create a socket
    if ( (lnSocketId=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        PrintErrorAndExit("socket");
    }
// zero out the structure used to define where the
    // data is going (destination) on the socket
    memset((char *) &lsSAOther, 0, sizeof(lsSAOther));

    // initialize the destination properties
    lsSAOther.sin_family = AF_INET;  // IPV4
    lsSAOther.sin_port = htons(port); // convert port to network byte order
    lsSAOther.sin_addr.s_addr = inet_addr(addr);


    while(1)
    {
        //send the Message
        if (sendto(lnSocketId, lanMessage, strlen(lanMessage) , 0 ,
            (struct sockaddr *) &lsSAOther, lnSockStructLen)==-1)
        {
            PrintErrorAndExit("sendto()");
        }
		
		break;

        //receive a reply and print it
        //clear the buffer by filling null, it might have previously received data
        memset(lanRecBuf,'\0', BUFLEN);

        //try to receive some data, this is a blocking call
        if (recvfrom(lnSocketId, lanRecBuf, BUFLEN, 0,
            (struct sockaddr *) &lsSAOther, &lnSockStructLen) == -1)
        {
            PrintErrorAndExit("recvfrom()");
        }

        // print reply from server
        printf("Server Reply: %s\n",lanRecBuf);
    }

    // close our socket resource
    close(lnSocketId);
	return 0;
}


int prompt(Config* config)
{
	for(int i = 0; i < config->numPrograms; ++i) {
		printf("%d: %s %d\n", i, config->programs[i], config->ports[i]);
	}
	printf("\nPlease select a program to terminate:");
	
	int idx; scanf("%d", &idx);
	return idx;
}


int main()
{
	Config config;
    ReadConfig(&config);
	
	while(true) {
		int idx = prompt(&config);
		if(idx < 0 || idx >= config.numPrograms) {
			break;
		}
		client(config.lanServerAddress, config.ports[idx]);
	}
	
	return 0;
};