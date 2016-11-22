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
	int n[1000];
	int t;
	pthread_t tid;
} mystruct;
int match_line(int fd, char *str);
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
void* sfun(void *arg){
	mystruct *sfile= (mystruct *) arg;
	int fd=open(sfile->name,O_RDONLY);
	sfile=(mystruct*)malloc(sizeof(mystruct));
	if(fd<0){
		printf("Erro a abrir ficheiro %s\n", sfile->name);
		pthread_exit(NULL);
	}
	sfile->t=0;
	while(sfile->n[sfile->t]!=0){
		sfile->n[sfile->t]=match_line(fd,sfile->search);
		sfile->t++;
	}
	close(fd);
	return NULL;
}

int main(int argc, char *argv[]){
	if(argc<3){
		printf("Erro, foram passados menos que 3 argumentos\n");
		exit(1);
	}
	mystruct file[100];//max 100 threads
	int c,c1;
	char help[1000][1000];
	for(c=0;c<argc-2;c++){
	strcpy(file[c].search,argv[1]);
	strcpy(file[c].name,argv[c+2]);
	strcpy(help[c],argv[c+2]);
	pthread_create(&file[c].tid,NULL,sfun,(void**)&file[c]);
	}
	for(c=0;c<argc-2;c++)
		pthread_join(file[c].tid,(void**)&file[c]);
	for(c=0;c<argc-2;c++){
		printf("Entrou no ciclo\n");
		for(c1=0;c1<file[c].t;c1++)
			printf("%s\t%d\n",help[c],file[c].n[c1]);
	}
		
	return 0;

}