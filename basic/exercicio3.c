#include <stdio.h>
int main(){
	int seq[10],c=0;
	seq[0]=1;
	seq[1]=1;
	for(c=0;c<10;c++){
		if(c>=2)
			seq[c]=seq[c-1]+seq[c-2];
		printf("%d ", seq[c]);
	}
	printf("\n");
}
