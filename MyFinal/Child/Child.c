#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "../common/SysConfigReader.h"
#define BUFLEN 512    //Max length of buffer


// simple routine to print an error and exit
void PrintErrorAndExit(char *lanErrorString)
{
    perror(lanErrorString);
    exit(1);
}


int server(char* addr, int port)
{
	struct sockaddr_in lsSAMe, lsSAOther;

    int lnSocketId = 0; // contains socket handle
    int lnReceiveLen = 0;  // number of bytes received on socket
    socklen_t lnSockStructLen = sizeof(lsSAOther); // length of socket structure

    // clear receive buffer
    char lanRecBuf[BUFLEN] = {0};


    //create a UDP socket
    if ((lnSocketId=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        PrintErrorAndExit("socket");
    }

	setsockopt(lnSocketId, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	
    // zero out the structure
    memset((char *) &lsSAMe, 0, sizeof(lsSAMe));
    lsSAMe.sin_family = AF_INET; // use IPV4 network addressing
    lsSAMe.sin_port = htons(port); // convert to network byte order
    lsSAMe.sin_addr.s_addr = inet_addr(addr);


    //bind socket to our specific port
    if( bind(lnSocketId, (struct sockaddr*)&lsSAMe, sizeof(lsSAMe) ) == -1)
    {
        PrintErrorAndExit("bind");
    }

    //keep listening for data
    while(1)
    {
        //try to receive some data, this is a blocking call
        // -1 indicates an error. anything 0 or above is assumed to be the number of bytes received.
        if ((lnReceiveLen = recvfrom(lnSocketId, lanRecBuf, BUFLEN, 0,
            (struct sockaddr *) &lsSAOther, &lnSockStructLen)) == -1)
        {
            PrintErrorAndExit("recvfrom()");
        }
		
		break;

        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(lsSAOther.sin_addr), ntohs(lsSAOther.sin_port));
        printf("Data: %s\n" , lanRecBuf);

        //now reply the client with the same data
        if (sendto(lnSocketId, lanRecBuf, lnReceiveLen, 0, (struct sockaddr*) &lsSAOther, lnSockStructLen) == -1)
        {
            PrintErrorAndExit("sendto()");
        }

        // clear buffer
        memset(lanRecBuf,0,sizeof(lanRecBuf));
    }

    // close the socket resource
    close(lnSocketId);
    return 0;
}


int main(int argc, char** argv)
{
	//printf("%s\n", argv[0]);
	int p = atoi(argv[1]);
	printf("[child] %s:%d started: pid=%d\n", argv[0], p, getpid());
	
	
     // get the socket setup information
    Config config;
    ReadConfig(&config);
	
	int port = -1;
	for(int i = 0; i < config.numPrograms; ++i) {
		if(!strcmp(config.programs[i], argv[0]) && p == config.ports[i]) {
			port = config.ports[i];
		}
	}
	
	if(port < 0) {
		printf("cannot find program's name in config\n");
		exit(1);
	}
	
	server(config.lanServerAddress, port);
	
	
	printf("[child] %s:%d is terminating: pid=%d\n", argv[0], p, getpid());
    return 0;
}
