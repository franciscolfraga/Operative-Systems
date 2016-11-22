#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct{
	char name[100];
	int fd;
	pthread_t tid;
} mystruct;
void* thrfunc(void *arg){
	mystruct *tfile=(mystruct *) arg;
	long long bytes=du(tfile->fd);
	printf("%s:\t%llu\n",tfile->name,bytes);
	return NULL;
}
int main(){
	mystruct file[100];//assumindo que não há mais que 100 ficheiros
	strcpy(file[0].name,"eof");
	int nf=0;
	while(scanf("%s",&(file[nf].name))!=EOF){
		nf++;
	}
	int c=0;
	for(c=0;c<nf;c++){
		file[c].fd=open(file[c].name,O_RDONLY);
		pthread_create(&file[c].tid,NULL,&thrfunc,&file[c]);
	}
	for(c=0;c<nf;c++){
		pthread_join(file[c].tid,NULL);
		close(file[c].fd);
	}
}