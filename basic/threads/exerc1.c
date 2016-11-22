#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int int1,int2,res,err,c,c2;
double pot;
pthread_t tid[4];


void *func(void *arg){
	pthread_t id = pthread_self();
	if(pthread_equal(id,tid[0])){
	res=int1+int2;
	printf("Resultado=%d\n", res);
}
	else if(pthread_equal(id,tid[1])){
	res=int1-int2;
	printf("Resultado=%d\n", res);
}
	else if(pthread_equal(id,tid[2])){
	res=int1*int2;
	printf("Resultado=%d\n", res);
}
	else if(pthread_equal(id,tid[3])){
		res=int1;
	for(c2=1;c2<int2;c2++)
		res=res*int1;
	printf("Resultado=%d\n", res);
}
return NULL;
}
int main(void){
	scanf("%d %d", &int1,&int2);
	int err;
	for(c=0;c<4;c++){
	err=pthread_create(&tid[c], NULL, &func, NULL);
	if(err!=0)
		printf("Error creating thread nr.%d\n", c);
	else printf("Thread nr%d created successfully\n", c);
}
return 0;
}
