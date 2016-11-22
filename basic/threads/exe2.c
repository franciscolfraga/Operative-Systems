#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int int1,int2,res,err,c2;
pthread_t tid[4];
pid_t pid;
void *funca(void *arg){
	int c=1;
	pthread_t id = pthread_self();
	for(c=1;c<4;c++){
	if(pthread_equal(id,tid[c])){
		pid = getpid();
		printf("Thread nr.%d: Tid is %lu Pid is %u\n",c,id,(unsigned int)pid);
	}
}
return NULL;
}
void *funcm(void *arg){
	int c=0;
	pthread_t id = pthread_self();
	if(pthread_equal(id,tid[0])){
		err=pthread_create(&tid[1], NULL, &funca, NULL);
		if(err!=0)
			printf("Error creating thread nr.1\n");
		else printf("Thread nr.1 created successfully\n");
		err=pthread_create(&tid[2], NULL, &funca, NULL);
		if(err!=0)
			printf("Error creating thread nr.2\n");
		else printf("Thread nr.2 created successfully\n");
		err=pthread_create(&tid[3], NULL, &funca, NULL);
		if(err!=0)
			printf("Error creating thread nr.3\n");
		else printf("Thread nr.3 created successfully\n");
	}
	for(c=1;c<4;c++)
		pthread_join(tid[c],NULL);
return NULL;
}

int main(void){

	err=pthread_create(&tid[0], NULL, &funcm, NULL);
	if(err!=0)
		printf("Error creating main thread\n");
	else printf("Main thread created successfully\n");
	pthread_join(tid[0],NULL);
return 0;
}
