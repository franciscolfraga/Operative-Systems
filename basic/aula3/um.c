#include <stdio.h>

extern char **environ;
int main(int argc, char *argv[])
{
    int i;
    char **var;

    printf("argumentos passados: ");
    for(i=0; i<argc; i++) 
    	printf("%s ", argv[i]);
    printf("\nenvironmental variables:\n");
	i=0;
    while(environ[i]!=NULL)
  		printf("%s\n", environ[i++]);
	
    printf("\n");

    return 0;
} 