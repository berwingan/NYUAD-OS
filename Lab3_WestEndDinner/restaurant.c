#include <stdio.h>
#include <ctype.h>
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
#include <fcntl.h>

#define SHARESIZE 4096 //page size
#define SEGMENTSIZE sizeof(sem_t)
#define PERM 0666
#define LINE 2000

char client_path[LINE];
char cashier_path[LINE];
char server_path[LINE];

void real_pwd(char* a){
        char b[LINE];
        getcwd(b,sizeof(b));
        strcpy(a,b);
	return;
}
int main(int argc, char *argv[]){
	real_pwd(client_path);
	real_pwd(cashier_path);
	real_pwd(server_path);
	strcat(server_path,"/server");
	strcat(cashier_path,"/cashier");
	strcat(client_path,"/client");


	printf("Opening WestEnd Dinner ...\n");
	printf("Mr Server is opening the kitchen and weating his shoes ... \n");
	int cashierN;
	printf("How many cashiers are coming to work today?\n>>");
	scanf("%d",&cashierN);
	printf("How long should our queue be today ?\n>>");
	int maxqueN, totalcustomerN;
	scanf("%d",&maxqueN); //fill maxqueue here

	printf("How many customer will visit us today ?\n>>");
	scanf("%d",&totalcustomerN);
	const char *serve_number = "serve_number";
	const char *serve_by_cashier = "serve_by_casier";
	const char *cashier_lock = "cashier_lock";
	const char *maxque; maxque = "maxque"; 
	const char *totalcustomer; totalcustomer = "totalcustomer";
	const char *client_write_lock; client_write_lock = "client_write_lock";
	const char *client_read; client_read ="client_read";
	const char *people_in_q; people_in_q ="people_in_q"; 
	const char *client_insert; client_insert = "client_insert";
	const char *serve_by_server; serve_by_server ="server_by_server";
	const char *cook_done; cook_done ="cook_done";
	const char *cook_write; cook_write ="cook_write"; //-->
	const char *client_last_write; client_last_write ="client_last_write";
	const char *butt; butt = "butt";
	sem_t * buttSEM = sem_open(butt,O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,0);
	sem_t * client_last_writeSEM = sem_open(client_last_write,O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,1); //lock for client writing
	sem_t * cook_doneSEM = sem_open(cook_done, O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,1); //use for cook to write
	sem_t * cook_writeSEM =sem_open(cook_write,O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,0); //for server to wait for write
	sem_t * serve_by_serverSEM =sem_open(serve_by_server, O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,0); //server know which is where 
	sem_t * serve_numberSEM = sem_open(serve_number,O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,0);
	sem_t * cashier_lockSEM =sem_open(cashier_lock, O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,1); //LOCK FOR CASHIER
	sem_t * serve_by_cashierSEM = sem_open(serve_by_cashier, O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,0);
	sem_t * client_insertSEM = sem_open(client_insert, O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,0);
	sem_t * people_in_qSEM =sem_open(people_in_q, O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,0); //initially there is no one in the q
	sem_t * client_readSEM = sem_open(client_read, O_CREAT|O_EXCL,S_IRUSR|S_IWUSR,0);
	sem_t * client_write_lockSEM = sem_open(client_write_lock, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR,1); //for client write
	sem_t * totalcustomerSEM = sem_open(totalcustomer, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR,totalcustomerN);
	sem_t * maxqueSEM = sem_open(maxque, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR,maxqueN);

	int v_last = shmget(IPC_PRIVATE,sizeof(int),PERM);
	char last_key[20]; snprintf(last_key,20,"%d",v_last);
//	int ok; sem_getvalue(maxqueSEM,&ok); printf("here is %d\n",ok); fflush(stdout);

//	sem_t * maxqueSEM = sem_open(maxque, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR,maxqueN); << this right here my friends is black magic
	if(maxqueSEM != SEM_FAILED){
		printf("The Max Queue for WestEnd Today is %d\n",maxqueN);}
	if(totalcustomerSEM != SEM_FAILED){
		printf("We will expect %d customers today !\n",totalcustomerN);}

	int key_list[totalcustomerN]; int lol; for(lol=0;lol<totalcustomerN;lol++){key_list[lol]=0;}
	/*Make Share memory for clients to place their keys*/
	int shmid_client_key= shmget(IPC_PRIVATE,sizeof(int),PERM);
	if(shmid_client_key == -1){perror("Creation");} //key is shmid
 	char shmid_client_key_string[20]; snprintf(shmid_client_key_string,20,"%d",shmid_client_key);
	int *ptr; int *ptr2;
	/*attach*/
	ptr2 =(int *)shmat(shmid_client_key,(void*)0,0);
	ptr = (int *)shmat(shmid_client_key,(void*)0,0);
	*ptr = 0; //initial initialization of 0
//==initialize the server
	if(fork()==0){
		char *arck[] ={server_path,shmid_client_key_string,NULL};
		execvp(server_path,arck);
		exit(0);}
	
//================================cashiers
	int cashierid = 0;
	while(cashierid!=cashierN){
		if(fork() ==0){
			int cashier_st = 4; int cashier_bt = 3; //parameters for the cashier  
			char cashier_st_string[2]; snprintf(cashier_st_string,2,"%d",cashier_st);
			char cashier_bt_string[2]; snprintf(cashier_bt_string,2,"%d",cashier_bt);
			char cashieridstring[3]; snprintf(cashieridstring,3,"%d",cashierid);
			char *arrc[]={cashier_path,cashier_st_string,cashier_bt_string,shmid_client_key_string,cashieridstring,NULL};
			execvp(cashier_path,arrc);
			exit(0);
			}
		cashierid ++;
		} 
//=========================================================== clients
	int customerid = 0; 
	while(customerid!=totalcustomerN){
		int randomwait =rand()%(10+1+0)+0; //can changthis[0,10] seconds
		int foodid = rand()%(19+1+0)+0; //[0,19]
		int eat_time = rand()%(20+1+0)+1; //[1,20] 
		if(fork()==0){
			char waitstring[2];snprintf(waitstring,2,"%d",randomwait);
			char customeridstring[3];snprintf(customeridstring,3,"%d",customerid);
			char eat_time_string[2]; snprintf(eat_time_string,2,"%d",eat_time);
			char foodidstring[2]; snprintf(foodidstring,2,"%d",foodid);
			char*arr[]={client_path,foodidstring,eat_time_string,shmid_client_key_string,customeridstring,waitstring,last_key,NULL};
			execvp(client_path,arr);	
			exit(0);
			}
		customerid ++;
		}
		
	int val; sem_getvalue(totalcustomerSEM,&val);
	int client_count = 0; //use this to count how many people is in
	int USA = 0;
	int item_f[LINE];
	int id_f[LINE];
	int time_f[LINE];
	while(val!=0){ // while not all client have pass 
		int brexit = 1; sem_getvalue(client_last_writeSEM,&brexit);
		if(brexit == 0){
			sem_wait(buttSEM);
			int *ptr5 =(int *) shmat(v_last,(void*)0,0);
			item_f[USA] = *ptr5; ptr5++;
			id_f[USA] = *ptr5; ptr5++;
			time_f[USA] = *ptr5;
			sem_post(client_last_writeSEM);
			USA++; int kuku = shmdt((void*)ptr5);
			}	
	
		sem_getvalue(totalcustomerSEM,&val);}
	sleep(4);
	printf("Mr Server is cleaning up and closing down and will be right out with the report for the day!\n");
	int pls = 0;
	float price[20] = {8.95,9.15,4.75,7.25,6.75,9.15,9.75,6.35,10.25,9.35,11.75,10.38,8.05,3.20,2.75,1.25,1.05,2.15,3.25,3.75};
	char *item[]={"BBQ-Chicken-Salad","Spinach-Power","Garden-Salad","Steak-Blue-Cheese","Ceasars-Salad","Chicken-Salad","Mongolian-BBQ-Plate","Club-Sandwhich","Belgian-Cheese-Sub","Rio-Grande-Beef-Sub","Argentine-Asado-Club","Sierra-Sub","Avocado-BLT","Soup-de-Egion","Soup-de-Sur","Coffee","Hot-Tea","Hot-Chocolate","Mocha","Cafe-Late"};
	float total = 0.0;
	float total_time = 0.0;
	int score[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	while(pls<USA){
		int i_item = item_f[pls];
		score[i_item] = score[i_item] + 1;
		printf("Customer %d was in the shopp for %d seconds and spent %f dollars\n",id_f[pls],time_f[pls],price[i_item]);
		total += price[i_item];
		total_time += time_f[pls];
		pls++;
		}
	int a = 0; int you = 0; int me = 0;
	while(a<20){
		if(score[a] > you){ me= score[a];
			you = a;}
		a++;}
	if(a!=0){ printf("The most sold food is %s with the total revenus of %f\n" ,item[you],(price[you]*me)); score[you] = 0;}
	a = 0; you = 0; me = 0;		
	while(a<20){
		if(score[a] > you){ me= score[a];
			you = a;}
		a++;}
	if(a!=0){ printf("The second most sold food is %s with the total revenus of %f\n" ,item[you],(price[you]*me)); score[you] =0;}
	a = 0; you = 0; me = 0;		
	while(a<20){
		if(score[a] > you){ me= score[a];
			you = a;}
		a++;}
	if(a!=0){ printf("The third most sold food is %s with the total revenus of %f\n" ,item[you],(price[you]*me)); score[you] =0;}
	a = 0; you = 0; me = 0;		
	while(a<20){
		if(score[a] > you){ me= score[a];
			you = a;}
		a++;}
	if(a!=0){ printf("The forth most sold food is %s with the total revenus of %f\n" ,item[you],(price[you]*me)); score[you] =0;}


	a = 0; you = 0; me = 0;		
	while(a<20){
		if(score[a] > you){ me= score[a];
			you = a;}
		a++;}
	if(a!=0){ printf("The fifth most sold food is %s with the total revenus of %f\n" ,item[you],(price[you]*me)); score[you] =0;}



	printf("The average customer spent %f seconds in the shop\n",(total_time/USA));
	printf("There was %d customers and they spent a total of %f \n",USA,total);
/*
	int u = 0; sem_getvalue(client_insertSEM,&client_count);
	while(u!=client_count){printf("%d\n",*ptr2);ptr2++; u++;}

*/	
	int as =shmctl(v_last,IPC_RMID,0);
	int remove_share_memory = shmctl(shmid_client_key,IPC_RMID,0); //remove segment
	sem_close(client_write_lockSEM); sem_unlink(client_write_lock);
	sem_close(client_readSEM); sem_unlink(client_read);
	sem_close(maxqueSEM);sem_unlink(maxque);
	sem_close(client_last_writeSEM); sem_unlink(client_last_write);
	sem_close(totalcustomerSEM);sem_unlink(totalcustomer);
	sem_close(people_in_qSEM); sem_unlink(people_in_q); //primarily for cashier
	sem_close(client_insertSEM); sem_unlink(client_insert);
	sem_close(serve_by_cashierSEM); sem_unlink(serve_by_cashier);
	sem_close(cashier_lockSEM); sem_unlink(cashier_lock);
	sem_close(serve_numberSEM); sem_unlink(serve_number);
	sem_close(serve_by_serverSEM); sem_unlink(serve_by_server);
	sem_close(cook_doneSEM); sem_unlink(cook_done);
	sem_close(buttSEM); sem_unlink(butt);
	sem_close(cook_writeSEM); sem_unlink(cook_write);
	return 0;
}
