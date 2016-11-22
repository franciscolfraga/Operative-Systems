#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>

char str[20];

typedef struct
{
	int fd;
	char ficheiro[20];
	int *v;
} vetor;

int match_line(int fd, char *str);

void* search(void *arg)
{
	vetor *vet=(vetor*)arg;
	int n=0;
	
	vet->v=(int*)malloc(sizeof(int)*100);
	if(vet->v==NULL)
		return NULL;
		
	while(1)
	{
		vet->v[n]=match_line(vet->fd, str);
		if(vet->v[n]==0)
		{	
			return vet->v;
		}
		n++;
		if(n>100)
			vet->v=(int*)realloc(vet->v, sizeof(int)*1000);
	}
}

int main(int argc, char *argv[])
{	
	if(argc<3)
	{	
		printf("ERRO1\n");
		exit(1);
	}
	
	pthread_t t[argc-2];
	vetor v[argc-2];
	int i, n;
	
	strcpy(str, argv[1]);
	
	for(i=0; i<argc-2; i++)
	{
		v[i].fd=open(argv[2], O_RDONLY);
		if(v[i].fd<0)
		{	
			printf("ERRO2\n");
			exit(1);
		}
		strcpy(v[i].ficheiro, argv[i+2]);
		
		pthread_create(&t[i], NULL, search, &v[i]);
	}
	
	for(i=0; i<argc-2; i++)
	{	
		n=0;
		pthread_join(t[i], (void**)&v[i].v);
		while(v[i].v[n]!=0)
		{
			printf("%s:	%d\n", v[i].ficheiro, v[i].v[n]);
			n++;
		}
		close(v[i].fd);
		free(v[i].v);
	}	
	return 0;
}
