#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char* argv[])
{
	char* wbuff;
	int fid;
	strcpy(wbuff, "Testing...\n");
	if( (fid = open("/dev/serp", O_RDWR,(S_IRUSR | S_IWUSR))) == -1){}
		fprintf(stderr, "ERROR\n");
		return 0;
	}
	write(fid, wbuff, strlen(wbuff));
	if(close(fid)==0)
		printf("I closed nr.%d file\n", fid);
	else{
		fprintf(stderr, "ERROR closing file!\n");
		return 0;
	}
	
	return 0;
}