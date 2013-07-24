

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define PHILOSOPHER_COUNT 5
#define LEFT	( i + PHILOSOPHER_COUNT - 1 ) % PHILOSOPHER_COUNT
#define RIGHT	( i + 1 ) % PHILOSOPHER_COUNT

#define up pthread_mutex_unlock
#define down pthread_mutex_lock

#define THINKING	 0
#define HUNGRY 		 1
#define EATING 		 2

#define THINK_TIME 1
#define EAT_TIME 2

int states[PHILOSOPHER_COUNT];
int eaten_meal_count[PHILOSOPHER_COUNT];
pthread_mutex_t m;
pthread_mutex_t s[PHILOSOPHER_COUNT];
pthread_t pN[PHILOSOPHER_COUNT];

void grab_forks(int i);
void release_forks(int i);
void test(int i);
void *philosopher_procedure(void* i);



int main(){
	int i;
	pthread_mutex_init(&m, NULL);
	for (i=0; i<PHILOSOPHER_COUNT; i++){
		states[i]=THINKING;
		pthread_mutex_init(&s[i], NULL);
		down(&s[i]);
		eaten_meal_count[i]=0;
		sleep(1);
		if (pthread_create(&pN[i], NULL, philosopher_procedure, (void*) &i)!=0){
			printf("Error creating thread.\n");
			return(1);
		}
	}

	for (i=0; i<PHILOSOPHER_COUNT; i++){
		pthread_join(pN[i], NULL);
	}

	for (int i=0; i<PHILOSOPHER_COUNT; i++){
		printf("Philosopher No:%d has eaten %d times.\n", i, eaten_meal_count[i]);
		pthread_mutex_destroy(&s[i]);
	}
	pthread_mutex_destroy(&m);

}

void grab_forks(int i){
	down(&m);
	states[i]=HUNGRY;
	test(i);
	up(&m);
	down(&s[i]);
}

void release_forks(int i){
	down(&m);
	states[i]=THINKING;
	test(LEFT);
	test(RIGHT);
	up(&m);
}

void test(int i){
	if (states[i]==HUNGRY
			&& states[LEFT]!=EATING
			&& states[RIGHT]!=EATING){
		states[i]=EATING;
		up(&s[i]);
	}
}

void *philosopher_procedure(void *i){
	int me= *(int*) i;
	sleep(1);
	printf("Thread No:%d has arrived to the table.\n", me);
	while(1){
		printf("Thread No:%d is thinking.\n", me);
		sleep(THINK_TIME);
		grab_forks(me);
		printf("Thread No:%d is eating her %d. meal.\n", me, ++eaten_meal_count[me]);
		sleep(EAT_TIME);
		release_forks(me);
	}

	return (void*) 0;
}





