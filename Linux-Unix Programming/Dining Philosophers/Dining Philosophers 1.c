#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <stdio.h>
#include <errno.h>
#include <signal.h>

#include <time.h>

#define PROCESS_COUNT 5
#define EXECUTION_DELAY 1
#define LEFT philosopher_number
#define RIGHT (philosopher_number+1)%5

int philosopher_number;
int semID;
time_t beginning_time;
int eatingCount;

void sigTermHandler(){
	printf("Philosopher %d has eaten %d times.\n", philosopher_number, eatingCount);
	sleep(1);
	exit(0);
}

struct sembuf semaphore_buffer;


void downmutex(){
	semaphore_buffer.sem_num=PROCESS_COUNT; // The last semaphore is mutex semaphore
	semaphore_buffer.sem_op=-1;
	semop(semID, &semaphore_buffer, 1);
}
void upmutex(){
	semaphore_buffer.sem_num=PROCESS_COUNT;
	semaphore_buffer.sem_op=1;
	semop(semID, &semaphore_buffer, 1);
}

void downfork(int fork_number){
	semaphore_buffer.sem_num=fork_number;
	semaphore_buffer.sem_op=-1;
	semop(semID, &semaphore_buffer, 1);
}
void upfork(int fork_number){
	semaphore_buffer.sem_num=fork_number;
	semaphore_buffer.sem_op=1;
	semop(semID, &semaphore_buffer, 1);
}

void childProcedure(){

	signal(SIGINT, &sigTermHandler);

	struct sembuf double_buffer[2];
	eatingCount=0;

	printf("Child[%d] waiting for execution\n", getpid());
	time_t current_time;
	time(&current_time);
	while(current_time<beginning_time+EXECUTION_DELAY) time(&current_time);

	while(1){
		downmutex();
		printf("Philosopher %d acquired mutex.\n", philosopher_number);
		if(semctl(semID, LEFT, GETVAL)==1){
			downfork(LEFT);}
		else{
			printf("Philosopher %d failed to acquire left fork. Alternating mutex.\n", philosopher_number);
			while(1){
				printf("Philosopher %d has released mutex.\n", philosopher_number);
				upmutex();
				downmutex();
				printf("Philosopher %d acquired mutex.\n", philosopher_number);
				if(semctl(semID, LEFT, GETVAL)==1){
					downfork(LEFT);
					break;}
				else{
					printf("Philosopher %d has failed to acquire left fork again.\n", philosopher_number);
					}
				}
		}

		printf("Philosopher %d has acquired left fork\n", philosopher_number);

		if (semctl(semID, RIGHT, GETVAL)==1){
			downfork(RIGHT);
		}
		else{
			printf("Philosopher %d failed to acquire right fork. Alternating mutex.\n", philosopher_number);
			int count=0;
			while(1){
				printf("Philosopher %d has released mutex.\n", philosopher_number);
				upmutex();
				downmutex();
				printf("Philosopher %d acquired mutex.\n", philosopher_number);
				if(semctl(semID, RIGHT, GETVAL)==1){
					downfork(RIGHT);
					break;}
				else{
					count++;
					printf("Philosopher %d has failed to acquire right fork for the %d time.\n", philosopher_number, count);
					if (count>=10){
						printf("DEADLOCK!!!\n");
						sleep(3);
					}
				}

			}

		}

		printf("Philosopher %d has acquired right fork. Proceeding to eat.\n", philosopher_number);
		eatingCount++;
		upmutex();


		printf("Philosopher %d has finished eating. Trying to acquire mutex to release forks.\n", philosopher_number);

		downmutex();
		printf("Philosopher %d acquired mutex.\n", philosopher_number);

		upfork(LEFT);
		upfork(RIGHT);

		printf("Philosopher %d has put down the forks. Yawn :O.\n", philosopher_number);
		upmutex();
	}
	
	exit(0);
}

void parentProcedure(){
	while(1) wait();
}

int main(){
	time(&beginning_time);
	semID=semget(IPC_PRIVATE, PROCESS_COUNT+1, 0777 | IPC_CREAT );
	
	if (semID==-1){
		printf("Error in creating semaphore set.\n");
		exit(1);
	}

	union semun {
	    int val;               /* used for SETVAL only */
	    struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
	    ushort *array;         /* used for GETALL and SETALL */
	}seva;
	seva.val=1;

	for (philosopher_number=0; philosopher_number<PROCESS_COUNT+1; philosopher_number++){
		semctl(semID, philosopher_number, SETVAL, seva);
	}
	philosopher_number=0;

	int pID=1;
	for (philosopher_number=0; philosopher_number<PROCESS_COUNT; philosopher_number++){
		pID=fork();
		if (pID==0) break;
	}

	if (pID<0){
		printf("Error creating children\n");
		exit(1);}
	else if (pID==0){
		childProcedure();}
	else{
		parentProcedure();
	}
	
	semctl(semID, 0, IPC_RMID);
}










