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
        write(1, content, eof);
    }
    close(fd);
}
return 0;
}
