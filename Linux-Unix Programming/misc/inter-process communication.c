/*
 * main.c
 *
 *  Created on: Nov 29, 2012
 *      Author: cem
 */

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h> // For memset.

#define NUM_CHILD 15

#define TRUE 1
#define FALSE 0

typedef struct childrennode{
	int PID;
	int returnCode;
}CHILDNODE;

typedef struct children{
	CHILDNODE childrenList[NUM_CHILD];
	int elementCount;
}CHILDREN;



#ifdef WITH_SIGNALS
volatile char interruptFlag;
#endif
CHILDREN chlist; // Global Variables

#ifdef WITH_SIGNALS
void emptyFunction(){ // Using for blocking signals with sigaction. I couldn't came up with anything else.
}

void sigIntHandler_P(){ // Parent's handler of SIGINT.
	interruptFlag=TRUE;
	printf("Parent[%d]: SIGINT has been received. Setting interrupt flag to true\n", getpid());
}

void sigTermHandler_C(){ // Children's handler of SIGTERM.
	printf("Child[%d]: SIGTERM has been received. Terminating process\n", getpid());
	exit(1);
}

#endif

void writeReturnCode(int PID, int returnCode){ // Saving return codes to print at the end of execution.
	int i;
	for (i=0; i<chlist.elementCount; i++){
		if (chlist.childrenList[i].PID==PID){
			chlist.childrenList[i].returnCode=returnCode;
			return;
		}
	}
	printf("PID %d is not on the process list.\n", PID);
}


void waitForChildren(){ // Routine for waiting for children and printing their status at the end of their execution.
	int status;
	int i;
	int returnedPID;

	while(errno!=ECHILD){
		returnedPID=wait(&status);
		if (returnedPID!=-1){
			writeReturnCode(returnedPID, WEXITSTATUS(status));
		}
	}

	printf("Parent[%d]: All children terminated.\n",getpid());

	for (i=0; i<chlist.elementCount; i++){
		printf("Parent[%d]: Child[%d] terminated with %d.\n", getpid(), chlist.childrenList[i].PID, chlist.childrenList[i].returnCode);
	}
}


void terminate(){ // Terminating all the children processes.
	printf("Parent[%d]: Terminating current children.\n", getpid());
	int j;
	for (j=0; j<chlist.elementCount; j++){
		kill(chlist.childrenList[j].PID, SIGTERM);
	}

	waitForChildren();

	printf("Parent[%d]: Children terminated. Exiting.\n", getpid());
	exit(1);
}

void childProcedure(){ // What children executes.

#ifdef WITH_SIGNALS
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, &sigTermHandler_C);
#endif

	printf("Child[%d]: of Parent[%d].\n", getpid(), getppid());
	sleep(10);
	printf("Child[%d]: Execution of Child[%d] has been completed.\n", getpid(), getpid());
}

void parentProcedure(){ // What parent executes.

#ifdef WITH_SIGNALS
	int SIGNAL;

	struct sigaction oldActList[31];
	struct sigaction act;


	for (SIGNAL=SIGHUP; SIGNAL<=SIGPWR; SIGNAL++){		// Ignoring signals. The empty procedure and storing action lists may be improved.
		if (SIGNAL==SIGKILL || SIGNAL==SIGCHLD || SIGNAL==SIGSTOP || SIGNAL==SIGINT) continue;

		struct sigaction act;
		memset(&act, 0, sizeof(act));
		act.sa_handler=&emptyFunction;
		sigaction(SIGNAL, &act, &oldActList[SIGNAL]);
	}

#endif

	waitForChildren();

#ifdef WITH_SIGNALS		// Restoring Old Signals

	for (SIGNAL=SIGHUP; SIGNAL<=SIGPWR; SIGNAL++){
		if (SIGNAL==SIGKILL || SIGNAL==SIGCHLD || SIGNAL==SIGSTOP || SIGNAL==SIGINT) continue;
		sigaction(SIGNAL, &oldActList[SIGNAL], NULL);
	}

#endif
}


int main(){

#ifdef WITH_SIGNALS
	interruptFlag=FALSE;

	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler=&sigIntHandler_P;
	sigaction(SIGINT, &act, NULL);

#endif


	int PID=1; // Initializing to something different than 0.
	int i;


	for (i=0;i<NUM_CHILD;i++){							// Create Children.
		PID=fork();
		if (PID==0) break;
		else if (PID<0) terminate();
		printf("Parent[%d]: Child[%d] has been created.\n", getpid(), PID);
		chlist.childrenList[chlist.elementCount++].PID=PID;
		sleep(1);

#ifdef WITH_SIGNALS
		if (interruptFlag==TRUE){
			printf("Parent[%d]: Interrupt flag has been set. Stopping the creation of children and terminating present children\n", getpid());
			terminate();
		}
#endif

	}

	if (PID==0) childProcedure();
	else parentProcedure();

    exit(0);
}
