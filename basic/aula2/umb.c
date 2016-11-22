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
	int fd = open(argv[1], O_RDONLY);
	int fdo = open(argv[2],O_CREAT|O_RDWR);
	char content[fd];
	if(fd < 0)
    {
        printf("File could not be opened.\n");
        perror("open");
        return 1;
    }
    else{
    ssize_t eof=10;
    while(eof>0){
		eof=read(fd, content, sizeof(content)-1);
        write(fdo, content, eof);
    }
    close(fd);
    close(fdo);
}
return 0;
}
