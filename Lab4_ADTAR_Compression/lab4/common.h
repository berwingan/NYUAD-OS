#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define ISREG 'r'
#define ISDIR 'd'
#define ISCHR 'c'
#define ISBLK 'b'
#define ISFIFO 'f'
#define ISLNK 'l'
#define ISSOCK 's'


//void parse_directory_list();
struct meta_data{
	char name[256];
//	char file_path[256]; //filee in the first level of random should be just /random
	char type;
	//ino_t inode_num;
	int version;
	int count;
	// if 0, the file the block is refering to is a file else it is a dir and should have a size of one 
	//struct timespec time_stamp;
	mode_t permissions;
	char soft_link[256];
	uid_t user_id; 
	gid_t group_id;	
	long size;
	struct stat block_info; //stores a copy of the entire stat blcok
	struct stat link_info;
};
void print_ad_metadata(FILE * read_file);
/*struct node{
	struct file metdat;
	struct node* next;
	struct node* prev;
};

struct list{
	struct node* head;
	struct node* tail;

};

struct list *create_metadata_list();
struct node* create_node();
struct file * getmetdata(struct node * Node);
void display_node(struct node * Node);
void display_list(struct list * ListofNodes);
*/
void parse_directory_list(char* file_path, char* directory_name, FILE * write_file);
void write_to_ad(char* read_path, FILE* write_ptr, int ver);
void read_from_ad(char *read_path);
char file_type(mode_t a);
#endif
