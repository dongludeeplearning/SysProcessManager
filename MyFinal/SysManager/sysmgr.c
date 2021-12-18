# include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include "configreader.h"

typedef struct tp
{
        int lnSignal;
        siginfo_t * lpSigInfo;
}ThreadParams;

typedef struct pt
{
        char lanProcName[1024];
        int lnPid;
} ProcTableEntry;

ProcTableEntry gProcTable[MAX_NUM_CHILDREN]={0};
ThreadParams *gpThreadParams = NULL;
void StartChildren();
void *SignalThread (void *ptr);
void StartProc(ProcTableEntry * lpProcTableEntry);

void SigHandler(int sig, siginfo_t *info, void *context)
{
        gpThreadParams = (ThreadParams*) malloc (sizeof(ThreadParams));
        gpThreadParams->lnSignal = sig;
        gpThreadParams->lpSigInfo = info;
        context = context;
        pthread_t thread1;
        int inret1 = pthread_create( &thread1, NULL, SignalThread, (void*)gpThreadParams);
        if (inret1 !=0)
        {
                printf("Unable to create thread. Exiting");
                exit(-1);
        }
}
int main()
{
        SystemConfig lsSystemConfig;
        GetSystemConfig(&lsSystemConfig);
        strcpy(gProcTable[0].lanProcName,lsSystemConfig.lanChild1Proc);
        strcpy(gProcTable[1].lanProcName,lsSystemConfig.lanChild2Proc);
        strcpy(gProcTable[2].lanProcName,lsSystemConfig.lanChild3Proc);

        struct sigaction sa;
        sa.sa_flags = SA_SIGINFO;
        sa.sa_sigaction= SigHandler;
        sigaction(SIGCHLD,&sa,NULL);

        StartChildren(gProcTable,MAX_NUM_CHILDREN);

        while(1)
        {
                printf("%d: inside parent with PID=%d",getpid());
                sleep(1);
        }
        return 0;

}

