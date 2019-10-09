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
#define LINE 256
#define LONG 64
#define PERM 0644
#define BUFFSIZE 5

void write_to_ad(char* read_path, char* write_path);
void read_from_ad(char* read_path);
void real_pwd(char *a); //find dir this is in

struct meta_data{ //point it starts at, size, 
        char name[256];
        unsigned char type;
        //ino_t inode_num;
        int count;
        //struct timespec time_stamp;
        mode_t permissions;
        char soft_link[256];
        uid_t user_id;
        gid_t group_id;
        off_t st_size;
        long size;	
};

int main(){ //convert to ad

//----------writing all files to ad
	char read_path[LINE];
	char write_path[LINE];
	real_pwd(read_path);
	strcat(read_path,"/hello5.txt"); //file to copy from, 
	real_pwd(write_path);
	strcat(write_path,"/hello5.ad"); // file to write to

	write_to_ad(read_path,write_path);
	read_from_ad(write_path);
	return 0;
}

void read_from_ad(char* read_path){ //reading from ad, size and 
	struct meta_data block;
	FILE *read_file;
	read_file = fopen(read_path,"rb");
	while(!feof(read_file)){
		if(fread(&block,sizeof(block),1,read_file)!=1){break;} //im a genius
		printf("size is %ld\n",block.size);
		char buffer[block.size+1];
		memset(buffer,'\0',block.size+1);
		size_t nread;
		nread = fread(buffer,1,block.size,read_file);
		printf("content is:%s",buffer);
	} 
	fclose(read_file);
}

void write_to_ad(char* read_path, char* write_path){
	//need to check if its the first time 
	int exist;
	if(access(write_path,F_OK)!=-1){exist = 1;} //file exist
	else{ exist = 0;} //file does not exist 

	struct meta_data block;	
	struct stat info;
	char file_buffer[LONG]; // size of file
	if(stat(read_path,&info) != -1){ //size of file, entire struct of permission and other stuff is here 
		block.size = info.st_size;}
	printf("size of the file is %ld\n",block.size); //block size is long	

	FILE *read_file;
	FILE *write_file;
	char buffer[BUFFSIZE+1]; //need one more for the \0 when freading 
	memset(buffer,'\0',BUFFSIZE+1);
	write_file= fopen(write_path,"a+b");
	read_file = fopen(read_path,"rb");
	size_t nread;
	fwrite(&block,sizeof(block),1,write_file); //writing the file size here
	while(!feof(read_file)){ //read_file vs read_path
		memset(buffer,'\0',BUFFSIZE);
		nread = fread(buffer,1,BUFFSIZE,read_file);
		fwrite(buffer,1,nread,write_file);	
		}
	fclose(read_file);
	fclose(write_file);
//---------v

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
