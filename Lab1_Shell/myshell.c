/* wqg203, Berwin Gan */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
//Global Var
char prompt[] = ">>" ; 
#define ARGU 100
#define LINE 2000
#define _POSIX_SOURCE


void pwd();
void eval(char *cmdline);
int parseline(const char *cmdline, char **arr);
int command(char **arr,int argu_number);
void record(char** arr, int argu_number);
int path_find(char**arr, int argu_number);
void expo(char **arr, int argu_number);
int cutpizza(const char *cmdline, char **arr);
void pizza(char **arr);
void hist(char **arr);
void change_director(char **arr,int argu_number);
 
//shell main
int main(int argc, char **argv){
	char cmdline[LINE];
	while(1){
		printf("%s",prompt);
		fgets(cmdline, LINE, stdin);
	//	printf("%s",cmdline);
		eval(cmdline);
	}
	
	exit(0); 
}

void eval(char *cmdline){
//	if (cmdline ==NULL){return;}  QUESTION: WHAT HAPPENS WHEN YOU PRESS ENTER WITHOUT ANYTHING ?
	char * arr[ARGU]; //printf s should yield first argument
	int argu_number = parseline(cmdline, arr); // should also yield the number of argument in (CHECK)
//	if(strcmp(arr[0], "exit")==0){exit(0);} put in cmd_check to record everything
//	printf("%d",argu_number);
	int cmd_check = command(arr,argu_number);//return a 0 if no such cmd, else 1(there is a cmd and do nothing), first ls must return 0
//	printf("%s",arr[0]);//currently NULL have to make it into the first word
}


//parsesing cmdline into argv and **argv
int parseline(const char *cmdline, char **arr){ //arr is the %s holder, last should be NULL
	int num = 0;//of argument 
	char * space;
	char local_holder[LINE]; //local cpy  DELETE
	char * ptr = local_holder; //pointer to first 
	strcpy(ptr,cmdline); //ptr is the first of holder which means 
	//ptr++;
	//replacing \n with ' '
	while (isspace(*ptr)){ptr++;} //front party gone,gets to first non white char, need take care of back
	ptr[strlen(ptr)-1] = ' ';
	//printf("%c",*ptr); // currently at first char after blank space (\n) is last
	//if(ptr[strlen(ptr)-1] == '\n'){printf("%s","back space check");}
	//printf("%s",ptr);
	space = strchr(ptr,' ');
	while(space){ // for pointer, while is only false at '\0'
		arr[num] = ptr;//ptr is on the left of the space
		num++;
		// to move over the space
		ptr = space;
		ptr++;
		while(isspace(*ptr)){ptr++;}
		*space = '\0';
		space = strchr(ptr,' '); //reuse for ':' in the path export thing 
		}
	arr[num] = NULL;// for last	
	if (num == 0){return (0);}
	else{ return (num);}
} //?????ASAK NANI

int command(char **arr,int argu_number){
	int cmd_check =0;
//	printf("%s",(arr[0]));
	char ex_check[strlen(arr[0])];
	strcpy(ex_check,arr[0]);ex_check[strlen(arr[0])] ='\0';
	if(ex_check[0]=='!'){
		int kek=strlen(arr[0])-1;
		char lol[kek];
		int part = 0;
		while(part!=kek){lol[part]=ex_check[part+1];part++;}
		lol[kek]='\0';
		int ytl=atoi(lol);//correct			
	char * c =(char *) calloc(1000,sizeof(char));
	int file = open("history.txt", O_RDONLY);
	int hotdog = read(file,c,1000);
	c[hotdog-1] = '\0 '; //formating so there aint a gap under the last print and the next >>
	//first char is \n need to get rid	
	int party = 0;
	int count = 2;
	char *turtle[ARGU];
	int penguin = parseline(c,turtle);
//	printf("%s",turtle[ytl-1]);		
	close(file);
	arr[0]=turtle[ytl]; // might have problem with path or any two argument type, MUST LOOK AT AGAIN
}	
	record(arr,argu_number);
	if(strcmp(arr[0],"exit")==0){exit(0);} //add removal of path file 
	else if(strcmp(arr[0],"history")==0){ hist(arr);cmd_check=1;}
	else if(strcmp(arr[0],"export")==0){expo(arr,argu_number);cmd_check=1;}
	else if(strcmp(arr[0],"pwd")==0){pwd();cmd_check=1;}
	else if(strcmp(arr[0],"cd")==0){change_director(arr,argu_number);cmd_check=1;}
	else {int horse= path_find(arr,argu_number); cmd_check=horse;}//1 means its accessable ? others means it is printed already
	
	
	if (cmd_check==0){
		printf("%s","command not found\n");return 0;}
		
	else if(cmd_check==2){printf("%s","there is such a command\n");return 1;}//for anything to look at 

		
}
void record(char** arr, int argu_number){
	int a = 0;
	int file = open("history.txt", O_RDWR|O_APPEND|O_CREAT, 0644);
	while(a != argu_number){ //write eveyrthing in history here man 

		write(file,arr[a],strlen(arr[a]));
		a++;
		write(file," ",1);
	}
	write(file,"\n",1); 
	close(file);
}

int path_find(char**arr, int argu_number){ //number of path+ put all the path in c
		int tick = 0;
		int file=open("export.txt",O_RDWR|O_APPEND|O_CREAT,0644);
		char * c =(char *) calloc(1000,sizeof(char)); //can't read form nothing
		int hotdog = read(file,c,1000);
		c[hotdog-1] = '\0 '; //formating so there aint a gap under the last print and the ne
		char *ugg[ARGU];
		int newly_wed =parseline(c,ugg);
		int go=0; int hula= 0;
		while(ugg[go]!=NULL){
			char word[strlen(ugg[go])];//word is always the new word
			strcpy(word,ugg[go]);//check for path 
			if(word[0] =='/'){
			tick++; //help me please
			int p =strlen(word)+1+strlen(arr[0]);
			char please[p];	
			memset(please,0,p);
			int start=0;
			while(start<strlen(word)){please[start]=word[start];start++;}
			please[start] ='/'; start++;int ai=0;
			char old_word[strlen(arr[0])];
			memset(old_word,0,strlen(arr[0]));
			strcpy(old_word,arr[0]);
			while(start<p){please[start]=old_word[ai];ai++;start++;}//please is the final path way
			please[start]='\0';	//comment for self(FOR THE LOVE OF YOU DON"T FORGET THIS_GOD DAM 
			int google=access(please,F_OK);
			if(google==0){hula= 2;}//1 means its real and have path
//			printf("%s",please);
			}go++;} //before write
		close(file);		
		return hula; //means file is not accesible
}
void expo(char **arr, int argu_number){ //1 means print, 2 means store// assuming all var is good
	//export + ' ' = 7 char// export PATH=/usr/local/bin:/usr/bin --> PATH= /usr/local/bin /usr/bin
	char *frr[ARGU];
	int trg =open("export.txt",O_WRONLY|O_CREAT,0644);
	write(trg,"PATH \n",6);
	close(trg);//nice #starthere
	if (argu_number == 2){//need cut the second part up to smaller pieces and check if 1st is same
		//have to check if the first is the same, if it is then gotta modify
		int count = cutpizza(arr[1],frr);//check first part against previos entry, how many pieces there are
		//frr[0] is the one to compare
		char * c =(char *) calloc(1000,sizeof(char));
		int tile = open("export.txt", O_RDONLY);
		int hotdog = read(tile,c,1000);
		c[hotdog-1] = '\0 '; //formating so there aint a gap under the last print and the next >>
		close(tile);
//		printf("%s",c);
		int a = 0;
		int file=open("export.txt",O_RDWR|O_APPEND|O_CREAT,0644);
		while (a!=count){
			write(file,frr[a],strlen(frr[a]));
			write(file," ",1);
		//	printf("%s",frr[a])i;
			a++;
		}write(file,"\n",1);
		close(file);			
}
	else if (argu_number == 1){pizza(arr);} //dam the damm char dam dam 
}

int cutpizza(const char *cmdline, char **arr){ //arr is the %s holder, last should be NULL(NOTHING WRONG)
	int num = 0;//of argument 
	char * space;
	char local_holder[LINE]; //local cpy  DELETE
	char * ptr = local_holder; //pointer to first 
	strcpy(ptr,cmdline); //ptr is the first of holder which means 
	while (isspace(*ptr)){ptr++;} //front party gone,gets to first non white char, need take care of back
	ptr[strlen(ptr)] = ':';
	space = strchr(ptr,'=');
	while(space){ // for pointer, while is only false at '\0'
		arr[num] = ptr;//ptr is on the left of the space
		num++;
		// to move over the space
		ptr = space;
		ptr++;
		while(isspace(*ptr)){ptr++;}
		*space = '\0';
		space = strchr(ptr,':'); //reuse for ':' in the path export thing 
		}
	arr[num] = NULL;// for last	
	if (num == 0){return (0);}
	else{ return (num);}
}
void pizza(char **arr){ //just need to EXCLUDE THE FIRST EVER CALL (LATER) 
	char * c =(char *) calloc(1000,sizeof(char));
	int file = open("export.txt", O_RDONLY);
	int hotdog = read(file,c,1000);
	c[hotdog] = '\0'; //formating so there aint a gap under the last print and the next >>
	//first char is \n need to get rid	
	int party = 0;
	int first = 0;//use for =
	int ko = 0;
//	printf("%s",c);
	while (c[party]!= '\0'){ //this is the most lmao code for print ever dam 
		if (c[party]=='\n'){printf("%c",'\n'); first=0;}
		else if(isspace(c[party]) && (first==0)){printf("%c",'=');first++;}
		else if(isspace(c[party]) &&! isspace(c[party+1])){printf("%c",':');}
		else{printf("%c",c[party]);}
//		if(c[party] =='\n'){printf("%s","its here");}
		party++;
	}	//print correctly and change the \n counter	
//	printf("%s","\n");
	close(file);
}

void hist(char **arr){ //just need to EXCLUDE THE FIRST EVER CALL (LATER) 
	char * c =(char *) calloc(1000,sizeof(char));
	int file = open("history.txt", O_RDONLY);
	int hotdog = read(file,c,1000);
	c[hotdog-1] = '\0 '; //formating so there aint a gap under the last print and the next >>
	//first char is \n need to get rid	
	int party = 0;
	int count = 2;
	printf("%s","1 ");
	while (c[party]!= '\0'){
		printf("%c",c[party]);
		if (c[party] == '\n'){printf("%d ",count);count++;}
		party++;
	}		
	printf("%s","\n");
	close(file);
}	
void change_director(char **arr,int argu_number){ // ! thing
//	printf("%d",argu_number);
	if (argu_number == 1){chdir("..");}
	else if (argu_number==2){
		char d[200];
		getcwd(d,sizeof(d));
		char final[strlen(d)+1+strlen(arr[1])];
		int count = 0;
		while(count!=strlen(d)){
			final[count] = d[count];
			count++;}
		final[count] = '/'; count++;
		char word[strlen(arr[1])];
		strcpy(word,arr[1]);	
		while(count!=(strlen(d)+1+strlen(arr[1]))){
			final[count] = word[count-(1+strlen(d))];
			count++;}
		chdir(final);
}	
}
void pwd(){
//	printf("%s","pwd checki\n");
//	char *c = (char*)calloc(1000,sizeof(char));
	char b[2000];
	getcwd(b,sizeof(b));
	printf("%s",b);
	printf("%c",'\n');
}









