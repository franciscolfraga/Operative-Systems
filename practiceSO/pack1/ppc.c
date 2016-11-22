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
typedef struct{
	char name[100];
	int nr;
	int fd;
	long long bytes;
	pthread_t tid;
} mystruct;
void* thrfunc(void *arg){
	mystruct *tfile=(mystruct *) arg;
	tfile->bytes=du(tfile->fd);
	//tfile=malloc(sizeof(mystruct));
	pthread_exit(NULL);
}
int main(){
	mystruct file[100];//assumindo que não há mais que 100 ficheiros
	int nf=0;
	while(scanf("%s",(file[nf].name))!=EOF){
		nf++;
	}
	char help[100][100];
	int c=0;
	for(c=0;c<nf;c++){
		file[c].fd=open(file[c].name,O_RDONLY);
		file[c].nr=c;
		strcpy(help[c],file[c].name);
		pthread_create(&file[c].tid,NULL,&thrfunc,&file[c]);
	}
	for(c=0;c<nf;c++){
		pthread_join(file[c].tid,NULL);
		close(file[c].fd);
	}
	for(c=0;c<nf;c++){
		printf("%s:\t%llu\n",help[c],file[c].bytes);
	}

}