#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

long long du(int fd);
int close(int fd);
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hello=PTHREAD_COND_INITIALIZER;
typedef struct{
	char name[100];
	int nr;
	int fd,nt;
	int *cont;
	pthread_t tid;
	long long bytes;
} mystruct; 
void* thrfunc(void *arg){
	mystruct *tfile=(mystruct *) arg;
	tfile->bytes=du(tfile->fd);
	++*(tfile->cont);
	if(*(tfile->cont)>=tfile->nt)
		pthread_cond_signal(&hello);
	return NULL;
}
int main(){
	mystruct file[100];//assumindo que não há mais que 100 ficheiros
	int nf=0;
	while(scanf("%s",(file[nf].name))!=EOF){
		nf++;
	}
	char help[100][100];
	int c=0,conta=0;
	for(c=0;c<nf;c++){
		file[c].fd=open(file[c].name,O_RDONLY);
		file[c].nr=c;
		file[c].nt=nf;
		file[c].cont=&conta;
		strcpy(help[c],file[c].name);
		pthread_create(&file[c].tid,NULL,&thrfunc,&file[c]);
		pthread_detach(file[c].tid);
	}
	pthread_cond_wait(&hello,&mutex);
	for(c=0;c<nf;c++){
		close(file[c].fd);
	}
	for(c=0;c<nf;c++){
		printf("%s:\t%llu\n",help[c],file[c].bytes);
	}

}