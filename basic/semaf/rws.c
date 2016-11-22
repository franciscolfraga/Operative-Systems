#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
//in this version only 1 reader at a time can read the shared variable and we will use semaphores
//done
#define million 5000000
#define tmillion 10000000
sem_t empty;
sem_t full;
sem_t mutex;
typedef struct{
	long *sharedc;  //shared counter
	long writenr;	//nr of writes
	long readnr;	//nr of reads
	int nr;			//thread nr
	pthread_t tid; //thread id
} mystruct;
void *read(void *arg){
	mystruct *reader= (mystruct *) arg;
	while((*reader->sharedc)<(tmillion*2)){
			sem_wait(&full);
			sem_wait(&mutex);
			reader->readnr++;
			sem_post(&mutex);
			if((*reader->sharedc)<(tmillion*2))
				sem_post(&empty);
			else
				sem_post(&full);
			printf("Thread nr %d reads %lu\n",reader->nr,*(reader->sharedc));
		}
	printf("OVERALL Thread nr %d read %lu times\n",reader->nr,reader->readnr);
	return NULL;
}

void *write(void *arg){
	mystruct *writer= (mystruct *) arg;
	while((writer->writenr)<tmillion){
		//busy waiting
			if((*(writer->sharedc)%million)==0){
				sem_post(&full);
			}
			else
				sem_post(&empty);
			sem_wait(&empty);
			sem_wait(&mutex);
			writer->writenr++;
			(*(writer->sharedc))++;
			sem_post(&mutex);
	}
	sem_post(&full);
	//clean the read threads
	printf("OVERALL Thread nr %d wrote %lu times and the shared is %lu\n",writer->nr,writer->writenr,(*(writer->sharedc)));
	return NULL;

}
int main(){
	mystruct rw[4];
	int c=0;
	long count=0;
	sem_init(&empty, 0, 1);
    sem_init(&full, 0, 1);  
    sem_init(&mutex, 0, 1);
	for(c=0;c<4;c++){
		rw[c].sharedc=&count;
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
	sem_destroy(&empty);
	sem_destroy(&full);
	sem_destroy(&mutex);
	printf("Number of iterations in total: %lu\n",count);
	return 0;
}