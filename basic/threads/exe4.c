#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define max 1000
extern char **environ;
struct inteiros{
	int um;
	int dois;
	int counter;
	int vetorf[99999];
	pthread_t id;
};
struct babys{
	pthread_t id;
	int vetor[max];	
	/* data */
};
void *mainfunc(void *arg);
void *babyfunc(void *arg);
int main(int argc, char *argv[]){
	struct inteiros maint;
	int c=0;
   	maint.um=atoi(argv[1]);
    maint.dois=atoi(argv[2]);
    pthread_create(&maint.id,NULL,&mainfunc,&maint);
    void *status;
    int *vetorfinal;
    pthread_join(maint.id,&status);
    vetorfinal=status;
    printf("%d",vetorfinal[0]);
    while(vetorfinal[c]!=NULL){
    	printf("vetorfinal[%d]=%d",c,vetorfinal[c]);
    	c++;
    }
    return 0;
}
void *mainfunc(void *arg){
	struct inteiros maint;
	maint=*(struct inteiros *)arg;
	int c=0,c1=0;
	struct babys b[max];
	for(c=0;c<maint.dois;c++){
		pthread_create(&b[c].id,NULL,&babyfunc,&b[c]);
	}
	void *retorna;
	int *vetor1;
	int lim=5;
	maint.counter=0;
	for(c=0;c<maint.dois;c++){
		pthread_join(b[c].id, &retorna);
		vetor1=retorna;
		for(c1=0;c1<lim;c1++){
			maint.vetorf[maint.counter]=vetor1[c1];
			printf("guardado nr%d\n",vetor1[c]);
			maint.counter++;
		}
	}
	printf("counter no thread é %d\n",maint.counter);
	pthread_exit(maint.vetorf);
}
void *babyfunc(void *arg){
	struct babys b;
	b=*(struct babys *)arg;
	int c=1;
	for(c=1;c<6;c++){
		b.vetor[c]=c;
	}
	pthread_exit(b.vetor);
}
