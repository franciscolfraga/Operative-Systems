#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int int1,int2,res,err,c,c2;
double pot;
pthread_t tid[2];
struct threads thread2;
struct threads thread3;
extern char **environ;
struct threads{
   pthread_t thread_id;
};

void *func(void *arg){
	pthread_t id = pthread_self();
	if(pthread_equal(id,tid[0])){
	res=int1+int2;
	printf("Resultado soma=%d\n", res);
}
	else if(pthread_equal(id,tid[1])){
	res=int1-int2;
	printf("Resultado sub=%d\n", res);
}
return NULL;
}
void *funcs(void *arg){
	pthread_t id = pthread_self();
	if(pthread_equal(id,thread2.thread_id)){
	res=int1*int2;
	printf("Resultado mult=%d\n", res);
}
	else if(pthread_equal(id,thread3.thread_id)){
		res=int1;
	for(c2=1;c2<int2;c2++)
		res=res*int1;
	printf("Resultado pot=%d\n", res);
}
return NULL;
}
int main(int argc, char *argv[]){
	int i;
    char **var;
    int1=atoi(argv[1]);
    int2=atoi(argv[2]);
    printf("Chosen numbers are %d and %d\n", int1,int2);
	for(c=0;c<2;c++){
	err=pthread_create(&tid[c], NULL, &func, NULL);
	if(err!=0)
		printf("Error creating thread nr.%d\n", c);
	else printf("Thread nr%d created successfully\n", c);
}
	err=pthread_create(&thread2.thread_id, NULL, &funcs, NULL);
	if(err!=0)
		printf("Error creating thread nr.3\n");
	else printf("Thread nr3 created successfully\n");
	err=pthread_create(&thread3.thread_id, NULL, &funcs, NULL);
	if(err!=0)
		printf("Error creating thread nr.4\n");
	else printf("Thread nr4 created successfully\n");

 pthread_exit(0);
}