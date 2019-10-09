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
#define LINE 2000
char cook_path[LINE];

void real_pwd(char* a){
        char b[LINE];
        getcwd(b,sizeof(b));
        strcpy(a,b);
	return;
}

int main(int argc, char *argv[]){ //path[don't touch],shmid list ,server_
	real_pwd(cook_path);
	strcat(cook_path,"/cook");
	
	printf("Mr Server is here!\n"); fflush(stdout);
	int max_servicetime = 3;//max server time
	const char *people_in_q = "people_in_q";
	const char *totalcustomer; totalcustomer="totalcustomer"; //use this to tell when job is over
	const char *serve_by_cashier = "serve_by_casier"; //start from 0
	const char *cashier_lock = "cashier_lock";
	const char *serve_number; serve_number="serve_number";	
	const char *serve_by_server; serve_by_server ="serve_by_server";
	const char *cook_done; cook_done ="cook_done";
	const char *cook_write; cook_write = "cook_write";
	sem_t * cook_doneSEM = sem_open(cook_done,O_EXCL);
	sem_t * cook_writeSEM =sem_open(cook_write,O_EXCL);
	sem_t * serve_by_serverSEM = sem_open(serve_by_server,O_EXCL);
	sem_t * serve_numberSEM = sem_open(serve_number,O_EXCL);
	sem_t * serve_by_cashierSEM = sem_open(serve_by_cashier,O_EXCL);
	sem_t * cashier_lockSEM =sem_open(cashier_lock,O_EXCL);
	sem_t * people_in_qSEM=sem_open(people_in_q,O_EXCL);
	sem_t * totalcustomerSEM = sem_open(totalcustomer,O_EXCL);
	int end; sem_getvalue(totalcustomerSEM,&end);
	int tool = 0;

char *item[]={"BBQ-Chicken-Salad","Spinach-Power","Garden-Salad","Steak-Blue-Cheese","Ceasars-Salad","Chicken-Salad","Mongolian-BBQ-Plate","Club-Sandwhich","Belgian-Cheese-Sub","Rio-Grande-Beef-Sub","Argentine-Asado-Club","Sierra-Sub","Avocado-BLT","Soup-de-Egion","Soup-de-Sur","Coffee","Hot-Tea","Hot-Chocolate","Mocha","Cafe-Late"};

	int server_key = shmget(IPC_PRIVATE,sizeof(int),PERM);
	char server_key_string[20]; int *serverptr;
	snprintf(server_key_string,20,"%d",server_key); //SERVERKEY 
 	serverptr = (int *)shmat(server_key,(void*)0,0);
	*serverptr = 0;
	while(end!=0){
		int k = rand()%(max_servicetime+1+0)+0; //[0,max]
		int real_s_time = rand()%(max_servicetime+1+0)+1;
		int cal = 0; sem_getvalue(serve_numberSEM,&cal);
		if(cal!=0){
			int *ptr= (int *)shmat(atoi(argv[1]),(void*)0,0); // attach to list of people
			for(int i = 0; i<tool; i++){ptr++;} //correct person to take order from 
			tool ++;
			int person_order = *ptr; //stored here
			int detach = shmdt((void*)ptr); //detach from receipt list 
			char receipt[20]; snprintf(receipt,20,"%d",person_order);
	
			int *ptr2= (int*)shmat(person_order,(void*)0,0);
			printf("Server passing customer %d order of item",*ptr2); ptr2++;
			char foodstring[2]; snprintf(foodstring,2,"%d",*ptr2);
			printf(" %d to the cooks\n",*ptr2); ptr2++; ptr2++;
//			*ptr2 = 1; //move to if
			if(fork()==0){
				char * a[]={cook_path,receipt,foodstring,server_key_string,NULL};
				execvp(cook_path,a);
				exit(0);}
			
			int detach2 =shmdt((void*)ptr2);
			
			sem_wait(serve_numberSEM);}
		//check pointer and if it is d
		int value_cook_done; sem_getvalue(cook_doneSEM,&value_cook_done);
		if(value_cook_done == 0){
			sem_wait(cook_writeSEM); //take receipt do it and come back 
			int *ptr3 = (int*)shmat(*serverptr,(void*)0,0);
			printf("Servering customer %d some",*ptr3);	
			ptr3++; printf(" %s\n",item[*ptr3]);
			sleep(real_s_time); //serving time
			ptr3++; ptr3++; *ptr3 = 1; //here clients wait + serve time
			 
			int detach3 = shmdt((void*)ptr3);
			sem_post(cook_doneSEM);
			}
				
		sem_getvalue(totalcustomerSEM,&end);
//		printf("Cashier %d with end: %d \n",atoi(argv[4]),end); fflush(stdout);
		}   
	int detachserver = shmctl(server_key,IPC_RMID,0);
	sem_close(serve_by_serverSEM);
	sem_close(people_in_qSEM);
	sem_close(totalcustomerSEM);
	sem_close(cashier_lockSEM);
	sem_close(serve_numberSEM);
	sem_close(cook_doneSEM);
	sem_close(cook_writeSEM);
	sem_close(serve_by_cashierSEM); //print im leaving 
	printf("Mr Server is done for the day!\n");
	exit(1);
	return 0;
}
