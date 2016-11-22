#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
extern char **environ;
int main(int argc, char *argv[]){
	int fdo = open(argv[1],O_CREAT|O_RDWR);
	if(fdo < 0)
    {
        printf("File could not be opened.\n");
        perror("open");
        return 1;
    }
    else{
    ssize_t eof=64;
    int i=0;
    while(environ[i]!=NULL){
        write(fdo, environ[i], sizeof(environ[i])-1);
        write(fdo, "\n", sizeof("\n")-1);
        i++;
}
    close(fdo);
}
return 0;
}
