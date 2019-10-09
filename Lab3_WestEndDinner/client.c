#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>

#define SHARESIZE 100
#define SEGMENTSIZE sizeof(sem_t)
#define PERM 0666

int main(int argc, char *argv[]){ //path[don't touch], item id, eat time, shmid, client id,randomwait
	sleep(atoi(argv[5]));	
	clock_t start, end;
	int process_time;

//	printf("%d here\n",atoi(argv[1]));

	int maxqueN; maxqueN =2; //fill maxqueue here
	const char *people_in_q = "people_in_q";
	const char *maxque; maxque = "maxque";
	const char *totalcustomer; totalcustomer="totalcustomer";
	const char *client_write_lock; client_write_lock = "client_write_lock";
	const char *client_read; client_read = "client_read";	
	const char *client_insert; client_insert = "client_insert";
	const char *client_last_write; client_last_write = "client_last_write";
	const char *butt; butt ="butt";
	sem_t * buttSEM = sem_open(butt,O_EXCL);
	sem_t * client_insertSEM = sem_open(client_insert,O_EXCL);
	sem_t * people_in_qSEM=sem_open(people_in_q,O_EXCL);
	sem_t * client_readSEM =sem_open(client_read,O_EXCL);
	sem_t * client_write_lockSEM = sem_open(client_write_lock,O_EXCL);
	sem_t * maxqueSEM = sem_open(maxque,O_EXCL);
	sem_t * totalcustomerSEM = sem_open(totalcustomer,O_EXCL);
	sem_t * client_last_writeSEM = sem_open(client_last_write,O_EXCL);
	int *ptr; int shmid = atoi(argv[3]); int detach;
	/*attach here*/
	ptr = (int *) shmat(shmid,(void*)0,0); //both ptr are pointing to the same 

	if(maxqueSEM == SEM_FAILED){
		printf("Cannot connect \n");}


	if(sem_trywait(maxqueSEM) != 0){ //the queue is full
		printf("The queue is full and client %d is leaving\n", atoi(argv[4]));  fflush(stdout);
		sem_wait(totalcustomerSEM);
		sem_close(maxqueSEM);
		sem_close(totalcustomerSEM);
		sem_close(client_write_lockSEM); sem_close(buttSEM);
		sem_close(client_readSEM); sem_close(client_last_writeSEM);
		sem_close(people_in_qSEM); sem_close(client_insertSEM);
		detach = shmdt((void*)ptr); //detach shared memory
		exit(1);	
		}
	else{ //currently waiting in queue 
		start = clock();
		/*Make a share memory space to put its order for cashier to take*/
		int individual = shmget(IPC_PRIVATE,sizeof(int),PERM);
		int *tag; int*cashier_check; int *server_check;
		cashier_check = (int*)shmat(individual,(void*)0,0); //use to wait for cashier 
		server_check = (int*)shmat(individual,(void*)0,0); 
		tag = (int*)shmat(individual,(void*)0,0); //attach  (cliend id,food id, cashier, server)
		*tag = atoi(argv[4]); //client id
		tag++; *tag = atoi(argv[1]); // food id
		tag++; *tag = 0; /*cashier*/ tag++; *tag = 0; /*server*/
		cashier_check ++;cashier_check ++; server_check++; server_check++; server_check++; // all in corrrect position
		/*write id unto q*/
		sem_wait(client_write_lockSEM); //LOCK
		int move_fwd; sem_getvalue(client_insertSEM,&move_fwd); int roll;
		for(roll = 0; roll<move_fwd;roll++){ptr++;}
		*ptr = individual; //use to write into receipt list in the restauaunt
//		printf("wrote %d\n", individual); //individual key
		sem_post(client_insertSEM); //--> how many there is 
		sem_post(client_write_lockSEM); //UNLOCK
		/*lets cashier know that there is people in q*/
		sem_post(people_in_qSEM);
		printf("Client %d is waiting in queue\n", atoi(argv[4])); fflush(stdout);
		/*waits for order to be taken by cashier*/
		while(*cashier_check == 0){} 
		printf("I am customer %d and my order of item %d  have been taken and I will exit the queue\n",atoi(argv[4]),atoi(argv[1]));
		sem_post(maxqueSEM);//put back position from the queue when order is taken
		printf("I am customer %d and I am waiting for my food\n",atoi(argv[4]));
		while(*server_check == 0){} //getting served
		
		/*EATING*/
		sleep(atoi(argv[2]));
		printf("Customer %d ate for %d seconds and is leaving our restaurant\n",atoi(argv[4]),atoi(argv[2]));

 
		sem_close(maxqueSEM);  //never unlink in a fork program	
		sem_close(client_write_lockSEM);
		detach = shmdt((void*)ptr); //detach shared memory
		sem_close(client_readSEM);
		sem_close(people_in_qSEM); sem_close(client_insertSEM);
		int indi_detach = shmctl(individual,IPC_RMID,0); //removed individual segment when done
		start = clock() - start;
		double tim = ((double)start/CLOCKS_PER_SEC);
		process_time = tim;
		 //insert info here before going away
//		char *record[3]; record[0] = argv[1]; record[1] = argv[4]; //item //id
//		printf("item %s by customer %s taking %s seconds\n",record[1],record[4],tt);
		
		sem_wait(client_last_writeSEM); //lock
		int *ptr9 = (int*)shmat(atoi(argv[6]),(void*)0,0);
		*ptr9 = atoi(argv[1]); ptr9++; *ptr9 = atoi(argv[4]); ptr9++; *ptr9 =process_time;
		int juice = shmdt((void*)ptr9);
		sem_post(buttSEM);
		sem_close(buttSEM);
		sem_wait(totalcustomerSEM); //decrease total customer count
		sem_close(totalcustomerSEM);
		exit(1);
		}

	return 0;
}
