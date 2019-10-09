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

int main(int argc, char *argv[]){ //path[don't touch],servicetime, breaktime, shmid of lists, cashier id
	printf("Cashier %d is at work !\n",atoi(argv[4])); fflush(stdout);
	int max_servicetime = atoi(argv[1]);
	int max_breaktime = atoi(argv[2]);
	const char *people_in_q = "people_in_q";
	const char *totalcustomer; totalcustomer="totalcustomer"; //use this to tell when job is over
	const char *serve_by_cashier = "serve_by_casier"; //start from 0
	const char *cashier_lock = "cashier_lock";
	const char *serve_number; serve_number="serve_number";	
	sem_t * serve_numberSEM = sem_open(serve_number,O_EXCL);
	sem_t * serve_by_cashierSEM = sem_open(serve_by_cashier,O_EXCL);
	sem_t * cashier_lockSEM =sem_open(cashier_lock,O_EXCL);
	sem_t * people_in_qSEM=sem_open(people_in_q,O_EXCL);
	sem_t * totalcustomerSEM = sem_open(totalcustomer,O_EXCL);
	int end; sem_getvalue(totalcustomerSEM,&end);
	int cash = atoi(argv[4]);
	while(end!=0){
		int st = rand()%(max_servicetime+1+0)+1; //[1,serveice time]
		int bt = rand()%(max_breaktime+1+0)+1; //[1,breaktime];
		int king; sem_getvalue(people_in_qSEM,&king);
		if(king!= 0){ //there is people thats need serving
			int *ptr= (int *)shmat(atoi(argv[3]),(void*)0,0); // attach to list of people
			sem_wait(cashier_lockSEM); // lock
			int val; sem_getvalue(serve_by_cashierSEM,&val); // which one to access
			sem_post(serve_by_cashierSEM);
			sem_post(cashier_lockSEM); //unlock
			for(int i = 0; i<val; i++){ptr++;} //correct person to take order from 
			int person_order = *ptr; //stored here
			int detach = shmdt((void*)ptr); //detach from receipt list 
			/*Service time*/
			sleep(st);
			int *ptr2 =(int *)shmat(person_order,(void*)0,0); //person_list
//			printf("Cashier %d took %d seconds to get customer %d order\n",atoi(argv[4]),st,*ptr2);	
			printf("Cashier %d: Client %d wants an order of ",atoi(argv[4]),*ptr2); ptr2++; fflush(stdout);
			printf("%d \n",*ptr2); ptr2++;
			*ptr2 = 1; //order taken 
			int detach2= shmdt((void*)ptr2); // detach from person's
			sem_wait(people_in_qSEM);
			sem_post(serve_numberSEM); //server takes over from here 
			}
		else{/*break time*/
		sleep(bt);
		printf("Cashier %d took a break for %d seconds.\n",cash,bt); fflush(stdout);
			} 
		sem_getvalue(totalcustomerSEM,&end);
//		printf("Cashier %d with end: %d \n",atoi(argv[4]),end); fflush(stdout);
		}   
	sem_close(people_in_qSEM);
	sem_close(totalcustomerSEM);
	sem_close(cashier_lockSEM);
	sem_close(serve_numberSEM);
	sem_close(serve_by_cashierSEM); //print im leaving 
	printf("Cashiers are going home.\n"); fflush(stdout);
	exit(1);
	return 0;
}
