/*
 * 	WARNING!!!!
 * 	THIS PROGRAM SHOULD BE COMPILED WITH -m32 FLAG, OTHERWISE IT WON'T WORK!!!
 *
 *  If you are receiving "Error allocating shared memory", change the key value of shmget function used
 *  in line 85
 *
 *  And if you are receiving "Error creating semaphore set" change the key value of semget function used in line 74.
 *
 */

#include <stdlib.h>

#include <time.h> // For synchronizing the start of the chilren.

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <stdio.h>
#include <errno.h>

#include <signal.h>

#define THINKING 0
#define HUNGRY 1
#define EATING 2

#define PHILOSOPHER_COUNT 5

#define EAT_TIME 1
#define THINK_TIME 3

#define SELF process_number
#define LEFT (process_number==0) ? PHILOSOPHER_COUNT : process_number-1
#define RIGHT (process_number+1)%PHILOSOPHER_COUNT

#define DELAY 3

int semID; // To ease of access of children, these are global variables.
int shmID;
int process_number;
int pID;
volatile int eat_count;
char* process_states;
struct sembuf smbf;
time_t start; // Set in the parent function, this will be used to fire off all the children in the same moment.

union semun {
    int              val;
    struct semid_ds *buf;
    unsigned short  *array;
};


void childProcedure();
void parentProcedure();

void grab_forks();
void put_away_forks();

void test(int pNo);

void sigTermHandler();

int main(){

	int pID=1;
	time(&start);
	smbf.sem_flg=0; // Initializing flag to 0.
	semID=semget(41, PHILOSOPHER_COUNT+1, 0777 | IPC_CREAT);	// Creating semaphore set. Last semaphore is used for mutex.

	if (semID==-1){
		printf("Error creating semaphore set\n");
		exit(1);
	}

	{	// To deallocate temporary variables after we are done.
		union semun value;
		value.val=0;
		int i;
		for (i=0; i<PHILOSOPHER_COUNT; i++){
			semctl(semID, i, SETVAL, value ); // Initializing process semaphores to 0.
		}
		value.val=1;
		semctl(semID, i, SETVAL, value); // Initializing mutex to 1.
	}

	shmID=shmget(11141, PHILOSOPHER_COUNT, 0777 | IPC_CREAT);	// Shared memory for checking each others' status.
	process_states=(char*) shmat(shmID, NULL, 0);

	if ((long)shmID==-1 ||(long) shmat==-1){
		printf("Error allocating shared memory\n.");
		exit(1);
	}

	{
		int i;
		for (i=0; i<PHILOSOPHER_COUNT; i++){
			process_states[i]=THINKING;
		}
	}

	for(process_number=0; process_number<PHILOSOPHER_COUNT; process_number++){ // Creating children.
		pID=fork();
		if (pID==0) break;
	}

	if (pID==0) childProcedure();
	else if (pID<0){
		printf("Error in creating children.\n");
		kill(getpgid()*-1, SIGTERM); // To terminate children.
		exit(1);}
	else parentProcedure();
}


void childProcedure(){

	eat_count=0;
	signal(SIGINT, sigTermHandler);
	//printf("Phil[%d] has created.\n", process_number);


	while(1){

		printf("Phil[%d] is thinking.\n", process_number);
		sleep(THINK_TIME); // Think.
		grab_forks();
		printf("Phil[%d] is eating.\n", process_number);
		eat_count+=1;
		sleep(EAT_TIME); // Eat.

		put_away_forks();
		//printf("Phil[%d] has dropped forks.\n", process_number);
	}
}

void parentProcedure(){
	while(1) wait();
}

void grab_forks(){
	smbf.sem_num=PHILOSOPHER_COUNT; // Last semaphore on the semaphore set was mutex.
	smbf.sem_op=-1;
	semop(semID, &smbf, 1); // Down operation on mutex.
	printf("Phil[%d] HUNGRY\n", process_number);

	//printf("Phil[%d] has acquired mutex.\n", process_number);

	process_states[SELF]=HUNGRY;
	test(SELF);

	smbf.sem_num=PHILOSOPHER_COUNT;
	smbf.sem_op=1;
	semop(semID, &smbf, 1); // Up operation on mutex.

	//printf("Phil[%d] has released mutex.\n", process_number);


	smbf.sem_num=process_number;
	smbf.sem_op=-1;
	semop(semID, &smbf, 1); // Down operation on process semaphores.

	//printf("Phil[%d] has made a down operation on its self semaphore.\n", process_number);
}

void put_away_forks(){
	smbf.sem_num=PHILOSOPHER_COUNT; // Last semaphore on the semaphore set was mutex.
	smbf.sem_op=-1;
	semop(semID, &smbf, 1); // Down operation on mutex.

	//printf("Phil[%d] has acquired mutex..\n", process_number);

	process_states[SELF]=THINKING;
	test(LEFT);
	test(RIGHT);

	smbf.sem_num=PHILOSOPHER_COUNT;
	smbf.sem_op=1;
	semop(semID, &smbf, 1); // Up operation on mutex.

	//printf("Phil[%d] has released mutex.\n", process_number);
}


#undef LEFT
#undef RIGHT

#define LEFT (pNo==0) ? PHILOSOPHER_COUNT : pNo-1
#define RIGHT (pNo+1)%PHILOSOPHER_COUNT

void test(int pNo){
	if (process_states[pNo]==HUNGRY &&
			process_states[LEFT]!=EATING &&
			process_states[RIGHT]!=EATING){
		process_states[pNo]=EATING;

		smbf.sem_num=pNo;
		smbf.sem_op=1;
		semop(semID, &smbf, 1); // Up operation on process semaphores.

		//printf("Phil[%d] has performed an up operation on semaphore no: %d.\n", process_number, pNo);
	}
}

void sigTermHandler(){
	printf("Philosopher[%d] has eaten %d times.\n", process_number, eat_count);
	exit(0);
}







