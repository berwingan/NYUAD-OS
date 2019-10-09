#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

#define _OPEN_SYS_ITOA_EXT
#define _POSIX_SOURCE 1
#define LINE 200
#define BUFFSIZE 5
#define LONG 64
#define PERM 0644

void real_pwd(char *a); //find dir this is in
struct meta_data{ //point it starts at, size, 
	
};

int main(){ //convert to ad

//----------writing all files to ad
	char read_path[LINE];
	char write_path[LINE];
	real_pwd(read_path);
	strcat(read_path,"/hello5.txt"); //file to copy from, 
	real_pwd(write_path);
	strcat(write_path,"/hello5.ad"); // file to write to
	//need to check if its the first time 
	int exist;
	if(access(write_path,F_OK)!=-1){exist = 1;} //file exist
	else{ exist = 0;}

	int header = 0; 
	char header_buffer[LONG];
	if(exist == 0){ // file does not exist
		snprintf(header_buffer,LONG,"%d",header); // header_buffer now have '0'
		FILE *head_write;
		head_write = fopen(write_path,"a+");
		header_buffer[strlen(header_buffer)] = ' ';
		fwrite(header_buffer,1,strlen(header_buffer)+1,head_write); //write 0 in the beginning
		fclose(head_write);
		}
	
	struct stat info;
	char file_buffer[LONG]; // size of file
	if(stat(read_path,&info) != -1){ //size of file, entire struct of permission and other stuff is here 
		snprintf(file_buffer,LONG,"%ld",info.st_size);}
	int sizeof_file_buffer = strlen(file_buffer); //1 =1, 10=2, 100=3,
	//obtain the header --> next thing here, 0  if its brand new
	FILE* a_file;
	a_file =fopen(write_path,"r");
	fscanf(a_file,"%d",&header);
	fclose(a_file); //obtained the offset to the meta data portion
	
	file_buffer[sizeof_file_buffer] = ' ';
	FILE *read_file;
	FILE *write_file;
	char c[2];
	write_file= fopen(write_path,"a+");
	read_file = fopen(read_path,"r");
	c[1] =fgetc(read_file);
	while(1){
		c[0] = c[1];
		c[1] = fgetc(read_file);
		printf("%c",c[0]);
		if(feof(read_file)){ c[0] = ' '; fwrite(c,1,sizeof(char),write_file);break;}
		fwrite(c,1,sizeof(char),write_file);
		}

	fwrite(file_buffer,1,sizeof_file_buffer+1,write_file); //inserting size of file
	fclose(read_file);
	fclose(write_file);
//---------

	return 0;
}

void real_pwd(char* a){
        char b[LINE];
        getcwd(b,sizeof(b));
        strcpy(a,b);
}


/*
	char head_buff[64];
	FILE *header_file;
	if(exist == 0){ //if it don't exist 
		header_file=fopen(write_path,"a+b");
		fwrite(head_buff,1,64,header_file); //64 bits for a long
		fclose(header_file);
		}

*/
