#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct{
	char name[100];
	int fd;
} mystruct;
void* thrfunc(void *arg){
	mystruct *tfile=(mystruct *) arg;
	long long bytes=du(tfile->fd);
	printf("%s:\t%llu\n",tfile->name,bytes);
	return NULL;
}
int main(){
	mystruct file;
	scanf("%s",&(file.name));
	file.fd=open(file.name,O_RDONLY);
	pthread_t tid;
	pthread_create(&tid,NULL,&thrfunc,&file);
	pthread_join(tid,NULL);
	close(file.fd);
}