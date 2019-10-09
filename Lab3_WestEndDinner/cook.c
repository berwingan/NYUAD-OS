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

int main(int argc, char *argv[]){ //path, receipt , item, server key
	const char *cook_done; cook_done ="cook_done";
	const char *cook_write; cook_write = "cook_write";
	sem_t * cook_doneSEM = sem_open(cook_done,O_EXCL);
	sem_t * cook_writeSEM =sem_open(cook_write,O_EXCL);

	int *place_w;
	place_w = (int *)shmat(atoi(argv[3]),(void*)0,0); //attached to server write place
	char *item[]={"BBQ-Chicken-Salad","Spinach-Power","Garden-Salad","Steak-Blue-Cheese","Ceasars-Salad","Chicken-Salad","Mongolian-BBQ-Plate","Club-Sandwhich","Belgian-Cheese-Sub","Rio-Grande-Beef-Sub","Argentine-Asado-Club","Sierra-Sub","Avocado-BLT","Soup-de-Egion","Soup-de-Sur","Coffee","Hot-Tea","Hot-Chocolate","Mocha","Cafe-Late"};

	int mincookingtime[]={18,12,10,12,13,15,21,13,15,18,23,12,12,11,6,2,1,1,2,5};
	int maxcookingtime[]={24,16,13,15,15,21,31,18,19,20,30,15,13,15,9,4,4,2,3,7};
	int cookingtime =rand()%(maxcookingtime[atoi(argv[2])]+1+0)+mincookingtime[atoi(argv[2])];

	printf("Making %s and it will take %d seconds\n", item[atoi(argv[2])],cookingtime);	
	sleep(cookingtime); //change to cooking tim in the end --->>>>>>>>>>.......................................................
	printf("%s is ready \n",item[atoi(argv[2])]);

	sem_wait(cook_doneSEM); //lock for all 
	*place_w = atoi(argv[1]);//write 
	sem_post(cook_writeSEM);

	int detach = shmdt((void*)place_w);
	sem_close(cook_doneSEM);
	sem_close(cook_writeSEM);
	exit(1);
	return 0;
}
