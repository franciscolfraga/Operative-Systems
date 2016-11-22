#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
char search[100];
typedef struct{
	char name[100]; //assumindo que nenhum nome de ficheiro tem mais que 100 chars
} mystruct;

void* sfun(void *arg){
	mystruct *sfile= (mystruct *) arg;
	int fd=open(sfile->name,O_RDONLY);
	if(fd<0){
		printf("Erro a abrir ficheiro %s\n", sfile->name);
	}
	int n=1;
	while(n!=0){
		n=match_line(fd,search);
		printf("%s:\t%d\n",sfile->name,n);
	}
	close(fd);
	return NULL;
}

int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Erro, não foram passados 3 argumentos\n");
		exit(1);
	}
	pthread_t tid;
	mystruct file;
	strcpy(search,argv[1]);
	strcpy(file.name,argv[2]);
	if(pthread_create(&tid,NULL,sfun,&file)==0){
		pthread_join(tid,NULL);
	}
	else{
		printf("Erro a criar o thread de pesquisa\n");
	}
	return 0;

}