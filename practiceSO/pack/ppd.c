#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
typedef struct{
	char name[100]; //assumindo que nenhum nome de ficheiro tem mais que 100 chars
	char search[100];
	pthread_t tid;
	int *cont;
	int nt;
} mystruct;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sig;
void* sfun(void *arg){
	mystruct *sfile= (mystruct *) arg;
	int fd=open(sfile->name,O_RDONLY);
	if(fd<0){
		printf("Erro a abrir ficheiro %s\n", sfile->name);
		pthread_exit(NULL);
	}
	int n=1;
	pthread_mutex_lock(&mutex);
	while(n!=0){
		n=match_line(fd,sfile->search);
		if(n!=0)
			printf("%s:\t%d\n",sfile->name,n);
	}
	++*(sfile->cont);
	if(*(sfile->cont)==sfile->nt)
		pthread_cond_signal(&sig);
	pthread_mutex_unlock(&mutex);
	close(fd);
	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char *argv[]){
	if(argc<3){
		printf("Erro, foram passados menos que 3 argumentos\n");
		exit(1);
	}
	mystruct file[10];//max 10 threads
	int c;
	int conta=0;
	for(c=0;c<argc-2;c++){
	strcpy(file[c].search,argv[1]);
	strcpy(file[c].name,argv[c+2]);
	file[c].cont=&conta;
	file[c].nt=argc-2;
	pthread_create(&file[c].tid,NULL,sfun,&file[c]);
	pthread_detach(file[c].tid);
	}
	pthread_cond_wait(&sig,&condition);
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&condition);
	return 0;

}