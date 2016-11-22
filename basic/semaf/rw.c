#include <stdio.h>
#include <pthread.h>
//in this version only 1 reader at a time can read the shared variable and we will use mutexes
//done
#define million 5000000
#define tmillion 10000000
typedef struct{
	long *sharedc;  //shared counter
	int *checker;
	long writenr;	//nr of writes
	long readnr;	//nr of reads
	int nr;			//thread nr
	pthread_t tid; //thread id
} mystruct;
pthread_mutex_t mutex;
void *read(void *arg){
	mystruct *reader= (mystruct *) arg;
	while((*reader->sharedc)<(tmillion*2)){
		if(((*(reader->sharedc)%million)==0)&&(*(reader->checker))<2){
			pthread_mutex_lock(&mutex);
			(*(reader->checker))++;
			reader->readnr++;
			pthread_mutex_unlock(&mutex);
			printf("Thread nr %d reads %lu and checker has nr %d\n",reader->nr,*(reader->sharedc),(*(reader->checker)));
		}
	}	
	printf("OVERALL Thread nr %d read %lu times\n",reader->nr,reader->readnr);
	return NULL;
}

void *write(void *arg){
	mystruct *writer= (mystruct *) arg;
	while((writer->writenr)<tmillion){
		if((*(writer->sharedc)%million)!=0){
			pthread_mutex_lock(&mutex);
			writer->writenr++;
			(*(writer->sharedc))++;
			pthread_mutex_unlock(&mutex);
		}
		else if((*(writer->checker))>1){
			pthread_mutex_lock(&mutex);
			(*(writer->checker))=0;
			writer->writenr++;
			(*(writer->sharedc))++;
			pthread_mutex_unlock(&mutex);
		}
	}
	printf("OVERALL Thread nr %d wrote %lu times\n",writer->nr,writer->writenr);
	return NULL;
}
int main(){
	mystruct rw[4];
	int c=0;
	long count=0;
	int check=0;
	pthread_mutex_init(&mutex, NULL);
	for(c=0;c<4;c++){
		rw[c].sharedc=&count;
		rw[c].checker=&check;
		rw[c].writenr=0;
		rw[c].readnr=0;
		rw[c].nr=c+1;
		if(c<2)
			pthread_create(&rw[c].tid,NULL,&write,&rw[c]);
		else
			pthread_create(&rw[c].tid,NULL,&read,&rw[c]);
	}
	for(c=0;c<4;c++){
		pthread_join(rw[c].tid,NULL);
	}
	pthread_mutex_destroy(&mutex);
	printf("Number of iterations in total: %lu\n",count);
	return 0;
}