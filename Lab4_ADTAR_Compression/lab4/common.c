#include <stdio.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "common.h"

#define LINE 256
#define LONG 8
#define PERM 0644
#define BUFFSIZE 5
//char filepath[256];
/*
struct list *create_metadata_list(){
	struct list *metdata_list = malloc(sizeof(struct list));
        metdata_list->head = malloc(sizeof(struct node));
	metdata_list->head->next = metdata_list->tail;
	metdata_list->head->prev = NULL;
	metdata_list->tail = malloc(sizeof(struct node));
	metdata_list->tail->prev = metdata_list->head;
	metdata_list->tail->next = NULL;
	return(metdata_list);
}
struct node* create_node(){
        struct node* new_node = malloc(sizeof(struct node));
        new_node->next = NULL;
	new_node->prev = NULL;
	return(new_node);
}


void add_node(struct list *metadata, struct node* NodeTobeAdded){
        NodeTobeAdded->prev = metadata->head;
	NodeTobeAdded->next = metadata->head->next;
	metadata->head->next = NodeTobeAdded;
	NodeTobeAdded->next->prev = NodeTobeAdded;
}

struct file * getmetdata(struct node * Node){
	return(&Node->metdat);
}

void display_node(struct node * Node){
	printf("Name of file: %s\n",Node->metdat.name);
	printf("File Type: %u\n", Node->metdat.type);
	printf("Count: %d\n", Node->metdat.count);	
	printf("Permissions: %3o\n",Node->metdat.permissions&0777);
	printf("Soft Link: %s\n",Node->metdat.soft_link);
	printf("Group ID: %d\n",Node->metdat.group_id);
	printf("User ID: %d\n", Node->metdat.user_id);
	printf("Size of File 1: %d\n", Node->metdat.size );
	printf("Size of File 2: %d\n", Node->metdat.st_size);
}

void display_list(struct list * ListofNodes){
	struct node * cur = ListofNodes->head;
	while(cur != ListofNodes->tail){
		display_node(cur);
		cur =  cur->next;
	}
}
*/
//----------------------------------using read to test
void real_pwd(char* a){
        char b[LINE];
        getcwd(b,sizeof(b));
        strcpy(a,b);
}


void print_dir_list(FILE * read_file){
	struct meta_data block;
	int count = 1;
	char name[256];
	
	printf("Print Directory Hierarchy:\n");
	while(!feof(read_file)){
		count =0;
		//printf("Start position: %d\n", ftell(read_file));
		if(fread(&block,sizeof(block),1,read_file)!=1){break;}//open the block and based on the type either mkdir or fopen
		//printf("Start position: %d\n", ftell(read_file));
		//fread(&block, sizeof(block), 1, read_file);
		//memset(&block, 0, sizeof(struct meta_data));	
		int i;

		for(i = 0; i< LINE;i++ ){
			if(block.name[i] == '/'){
		//		printf("%d\n", count);
				count = count +1; 
			}
			//printf("%d\n", count);	
		}	

		//printf("%d\n", count);	
		int k;
		for(k = 0; k < count; k++){
			printf("|\t");
		}
		printf("|\n");
		 for(k = 0; k < count; k++){
                        printf("\t");
                }

	//	strcpy(name, block.name);
	//	strtok(name, "/");
	//	while(strtok(NULL, "/") = !NULL){
			

	//	}
		
		printf("`-- %s\n",block.name);
		//printf("Type: %c\n", block.type);
		//printf("Owner: %d\n", block.user_id);
		//printf("Group: %d\n", block.group_id);
		//printf("Size: %d\n", block.size);
		//printf("Version:%d\n ", block.version);
		//printf("Rights: " );
 		//printf("Size of block: %d\n", sizeof(block));
 		/*
		printf( (S_ISDIR(block.permissions)) ? "d" : "-");
    		printf( (block.permissions & S_IRUSR) ? "r" : "-");
    		printf( (block.permissions & S_IWUSR) ? "w" : "-");
    		printf( (block.permissions & S_IXUSR) ? "x" : "-");
    		printf( (block.permissions & S_IRGRP) ? "r" : "-");
    		printf( (block.permissions & S_IWGRP) ? "w" : "-");
    		printf( (block.permissions & S_IXGRP) ? "x" : "-");
    		printf( (block.permissions & S_IROTH) ? "r" : "-");
    		printf( (block.permissions & S_IWOTH) ? "w" : "-");
    		printf( (block.permissions & S_IXOTH) ? "x" : "-");
    		printf("\n\n");
		*/
		//memset(&block, 0, sizeof(block));	
		fflush(read_file);
		fseek(read_file,block.size, SEEK_CUR);
		//printf("Move to %d\n", ftell(read_file));
		
	}
}


void print_ad_metadata(FILE * read_file){
	struct meta_data block;
	while(!feof(read_file)){
		//printf("Start position: %d\n", ftell(read_file));
		if(fread(&block,sizeof(block),1,read_file)!=1){break;}//open the block and based on the type either mkdir or fopen
		//printf("Start position: %d\n", ftell(read_file));
		//fread(&block, sizeof(block), 1, read_file);
		//memset(&block, 0, sizeof(struct meta_data));	
		printf("Name: %s\n", block.name);
		printf("Type: %c\n", block.type);
		printf("Owner: %d\n", block.user_id);
		printf("Group: %d\n", block.group_id);
		printf("Size: %d\n", block.size);
		printf("Version:%d\n ", block.version);
		printf("Rights: " );
 		//printf("Size of block: %d\n", sizeof(block));
		printf( (S_ISDIR(block.permissions)) ? "d" : "-");
    		printf( (block.permissions & S_IRUSR) ? "r" : "-");
    		printf( (block.permissions & S_IWUSR) ? "w" : "-");
    		printf( (block.permissions & S_IXUSR) ? "x" : "-");
    		printf( (block.permissions & S_IRGRP) ? "r" : "-");
    		printf( (block.permissions & S_IWGRP) ? "w" : "-");
    		printf( (block.permissions & S_IXGRP) ? "x" : "-");
    		printf( (block.permissions & S_IROTH) ? "r" : "-");
    		printf( (block.permissions & S_IWOTH) ? "w" : "-");
    		printf( (block.permissions & S_IXOTH) ? "x" : "-");
    		printf("\n\n");
		//memset(&block, 0, sizeof(block));	
		fflush(read_file);
		fseek(read_file,block.size, SEEK_CUR);
		//printf("Move to %d\n", ftell(read_file));
	}
}

void read_from_ad(char* read_path){ //reading --everything should be contained in block struct
        struct meta_data block;
        FILE *read_file;
        read_file = fopen(read_path,"rb");
        while(!feof(read_file)){
                if(fread(&block,sizeof(block),1,read_file)!=1){break;}//open the block and based on the type either mkdir or fopen
                printf("size is %ld\n",block.size);
                char buffer[256+1]; //1 for \0
                memset(buffer,'\0',257);
                size_t nread;

//------------------------------------------------------------------------------------------ with ownership
                if(block.type!=ISDIR){ //remake file, chmod type 
                        long file_size =block.size; //size to while is here
                        FILE * r_file;
                        char name_file[256]; memset(name_file,'\0',256);
                        name_file[0] = 'a'; //replace with block.name for the same name with when extracing
                        strcat(name_file,block.name); //with regards to comment of the name 
                        printf("This is a file name:%s:\n",name_file);
                        r_file = fopen(block.name,"w+b"); //for now use name_file,later change all name_file with block.name
                        while(file_size>=256){
                                nread = fread(buffer,1,256,read_file);
                                fwrite(buffer,1,nread,r_file);
                                memset(buffer,'\0',257);
                                file_size -=256; //per read
                                }
                        if(file_size!=0){
                                nread=fread(buffer,1,file_size,read_file);
                                fwrite(buffer,1,file_size,r_file);}


                        fclose(r_file);

//--------------------------------------aksjbdalksjdnlas
			if(S_ISLNK(block.link_info.st_mode)){
				char p1[256];
				char p2[256];
				real_pwd(p1); real_pwd(p2);
				printf("LINK FROM READ\n");
//				printf("%s\n",block.name); //remove before last / and cat ij
				char ij[256]; memset(ij,'\0',256);
				int u = readlink(block.name,ij,256);
				printf("Link to: |%s|\n",ij); //need path toi
				char name_of_block[256]; memset(name_of_block,'\0',256);
				name_of_block[0] = 'a'; // ----------------------------------------remind to rm a;ll a
				strcpy(name_of_block,block.name);
				char * ptr = strrchr(name_of_block,'/'); ptr++;
				*ptr = '\0';
				strcat(name_of_block,ij);
				strcat(p1,"/"); 	
				strcat(p2,"/");
				strcat(p2,name_of_block);
				strcat(p1,block.name); strcat(p2,block.soft_link);
				printf("link from |%s| to :|%s|\n",p1, p2);

				int ok = symlink(p1,p2); //sec
				printf("something is sylink: %s \n",strerror(errno));	
				} //make slink here 
//--------------------------------------asdjkasnkdjas
			
                        int a =chmod(block.name,block.block_info.st_mode); //chmod here,change name file to block.name
//                        if(chown(name_file,block.block_info.st_uid,block.block_info.st_gid)==-1){ //why is this not working
 //                              printf("chown fail\n");} //only fails for the one that requires change 
}
                else if(block.type==ISDIR){
                        char name_of_dir[256];memset(name_of_dir,'\0',256); //replace with block.name when final
                        name_of_dir[0] = 'a'; //to differentiate random, take out at the end 
                        strcat(name_of_dir,block.name); //to add the / to the name 
                        printf("This is a dir with name:%s:\n",block.name); //mkdir with permission and ownership 
                        struct stat holder= {0};
                        if(stat(name_of_dir,&holder) == -1){
                                printf("making a dir here\n");
                                int mmk_dir = mkdir(block.name,block.block_info.st_mode); //chmod
                                }
                        }

//-----------------------------------------------------------------------

        }
        fclose(read_file);
}

char file_type(mode_t perm){
	if S_ISREG(perm){
		return(ISREG);
	}
	else if S_ISDIR(perm){
                return(ISDIR);
        }
	else if S_ISCHR(perm){
                return(ISCHR);
        }
	else if S_ISBLK(perm){
                return(ISBLK);
        }
	else if S_ISFIFO(perm){
                return(ISFIFO);
        }
	else if S_ISLNK(perm){
                return(ISLNK);
        }
	else if S_ISSOCK(perm){
                return(ISSOCK);
        }

}

void write_to_ad(char* read_path, FILE* write_ptr, int ver){
        //need to check if its the first time

        int exist;
	FILE * read_file;
	char buffer[BUFFSIZE+1];
	memset(buffer, '\0', BUFFSIZE+1);
        size_t nread;
        struct meta_data block;
        struct stat info; 
	fseek(write_ptr,0, SEEK_END);
        //GET STAT INFO
	if(stat(read_path,&info) != -1){ //size of file, entire struct of permission and other stuff is here
        block.size = info.st_size;		
	block.version = ver;
	//block.dir_flag = 0; //flag that refers this block as a file
	block.permissions = info.st_mode;
	//block.soft_link = ;
	//strcpy(block.file_path,read_path);
	printf("path is %s\n",read_path);
	block.user_id = info.st_uid;
	block.group_id = info.st_gid;
	//printf("Type: %d\n", rfile->d_type);
	block.type = file_type(info.st_mode); //file type 
	strncpy(block.name,read_path, LINE); //name of file 
	block.count = info.st_nlink;
	block.block_info = info; //copies the entire stat info struct
	//memset(block.soft_link, '\0',LINE);
//------------------------------------------------------------------------------asldkamsldkams
	printf("--------------------------------------------\n");
        if(lstat(read_path,&block.link_info) != -1){
        if(S_ISLNK(block.link_info.st_mode)){
		printf("LINK here\n");
		char p[256]; memset(p,'\0',256);
		int ok = readlink(block.name,p,256); //realpath of p
		strcpy(block.soft_link,p);
		printf("Link to:%s\n",p); }
                }
//----------------------------------------------------------------------------------softlink
}

        printf("Size of the file is (from struct) %ld\n",block.size); //block size is long
        //printf("Size of the file is %ld\n",block.st_size); //block size is long 
	//FILE *read_file;
        //char buffer[BUFFSIZE+1]; //need one more for the \0 when freading
        //memset(buffer,'\0',BUFFSIZE+1);
        //write_file= fopen(write_path,"a+b");
        read_file = fopen(read_path,"rb");
        //size_t nread;
        fwrite(&block,sizeof(block),1,write_ptr); //writing the file size here
        while(!feof(read_file)){ //read_file vs read_path
        	memset(buffer,'\0',BUFFSIZE);
        	nread = fread(buffer,1,BUFFSIZE,read_file);
		block.size = block.size + nread;
        	fwrite(buffer,1,nread,write_ptr);
        }
        //printf("Size of the file is (bytes) %ld\n",block.size); //block size is long 
	
       	fclose(read_file);
        //fclose(write_file);
                                                                                               //---------v
        //
}


void write_dir_to_ad(char* read_path ,FILE* write_ptr, int ver){ //for dirs 
        struct meta_data block;
        struct stat info; 
        //GET STAT INFOi
        fseek(write_ptr,0, SEEK_END);
	if(stat(read_path,&info) != -1){ //size of file, entire struct of permission and other stuff is here
        	block.size = 0; //0 for all dirs when creation
	//	block.dir_flag = 1; //flag that refers this block as a dir, use for remake
		block.permissions = info.st_mode;
		//block.soft_link = ;
		block.user_id = info.st_uid;
		block.group_id = info.st_gid;
		block.version = ver;
		//printf("Type: %d\n", rfile->d_type);
		block.type = file_type(info.st_mode); //file type 
		strncpy(block.name, read_path, LINE); //name of dir ------------------------------------re check
		block.count = info.st_nlink;
		block.block_info = info; //copies the entire stat info struc
	}

        printf("Size of the dir is (from struct) %ld\n",block.size);// should be 0
        fwrite(&block,sizeof(block),1,write_ptr); //writing the file size here

}

int compare(FILE* read_file, char* path){
	struct meta_data block;
	char * path_names[200];
	int count = 1;
	//fseek(read_file, 0, SEEK_SET);
	rewind(read_file);
	while(!feof(read_file)){
		//printf("Start position: %d\n", ftell(read_file));
		if(fread(&block,sizeof(block),1,read_file)!=1){break;}//open the block and based on the type either mkdir or fopen
		//printf("Start position: %d\n", ftell(read_file));
		//fread(&block, sizeof(block), 1, read_file);
		//memset(&block, 0, sizeof(struct meta_data));	
		printf("Name: %s\n", block.name);
		printf("Path: %s\n", path);
		if (!strcmp(block.name, path)){
			count = block.version + 1;
			printf("Version is is here \n");
			break;
		}
		//printf("Type: %c\n", block.type);
		//printf("Owner: %d\n", block.user_id);
		//printf("Group: %d\n", block.group_id);
		printf("Size: %d\n", block.size);
		//printf("Rights: " );
 		
			
		fflush(read_file);
		fseek(read_file,block.size, SEEK_CUR);
		//printf("Move to %d\n", ftell(read_file));
	}

	return(count);
}


/*
void print_dir_list(char* file_path, char* directory_name, FILE* write_file){ //char read_path
	DIR* root = NULL;  //type DIR 
	char name[LINE]; 
	char file_name[LINE];
	int null_flag = 0;
	char filepath[LINE];
	struct dirent *element;
	//printf("\tFound Directory - %s\n", directory_name);
	//printf("Here1\n");
	if(file_path !=NULL){
		null_flag = 0;
		//printf("Not null\n");
		//printf("1. %s\n", filepath);
		strcpy(filepath, file_path);
		//printf("Here2\n");
		//printf("2. %s\n", filepath);
        	strcat(filepath, "/");
		//printf("Here3\n");
		//printf("3. %s\n", filepath);
        	strcat(filepath, directory_name);
		//printf("here4\n");
		printf("|\n");
		printf("`--%s\n", filepath); //dir here 
		//write_dir_to_ad(filepath,write_file);
		root = opendir(filepath);
	//	flag = 0;
	}
	else{  //root here
		null_flag = 1;
		//printf("NULL\n");
		printf("|%s\n", directory_name);
		printf("|");
		//write_dir_to_ad(directory_name, write_file);
		root = opendir(directory_name);
	}
	
	printf("Hello - %d\n", root);	
	printf("Print %s\n", strerror(errno));
	if(root){
		element = readdir(root);
		while(element !=NULL){

			if((!strcmp(element->d_name,"."))||(!strcmp(element->d_name,".."))){
				//printf("stuck\n");
				element = readdir(root);
				continue;
			}	
		
			if(element->d_type == DT_DIR){ 
	

				strcpy(name, element->d_name);
//				if (file_path !=NULL){
//				printf("Passing file path - %s,  and file name - %s\n",filepath, name);
				if (null_flag == 0){ //folders inside the first level
	//				printf("Internal dir passing through\n");
	//				printf("Passing file path - %s,  and file name - %s\n",filepath, name);
					print_dir_list(filepath, name, write_file);
				}
				else{ //actually this is the first folder it encounters, not root dir
	//				printf("Root dir passing through\n");
	//				printf("Passing file path - %s,  and file name - %s\n",directory_name, name);

					print_dir_list(directory_name, name, write_file);
				}

			}
			else{
				strcpy(name, element->d_name);
				if(null_flag ==0){
	//				printf("Internal file passing through\n");
//					strcpy(file_name, filepath);
//					strcat(file_name, "/");
//					strcat(file_name,name );
					printf("\t|\n");
					printf("\t`--%s\n", name);
//					write_to_ad(write_file);
				}
				else{
	//				printf("Root file pass through\n");
//					strcpy(file_name, directory_name);
//					strcat(file_name, "/");
//					strcat(file_name, name);
					printf("\t|\n");
					printf("\t`--%s\n", name);
				}
//					write_to_ad(file_name, write_file);
				//printf("\t\t%s\n",element->d_name);
			}
			element = readdir(root);

		}	
		closedir(root);
	}

//

}
*/
void parse_directory_list(char* file_path, char* directory_name, FILE* write_file){ //char read_path
	DIR* root = NULL;  //type DIR 
	char name[LINE]; 
	char file_name[LINE];
	int ver = 0;
	int null_flag = 0;
	char filepath[LINE];
	struct dirent *element;
	//printf("\tFound Directory - %s\n", directory_name);
	//printf("Here1\n");
	if(file_path !=NULL){
		null_flag = 0;
		printf("Not null\n");
		printf("1. %s\n", filepath);
		strcpy(filepath, file_path);
		//printf("Here2\n");
		printf("2. %s\n", filepath);
        	strcat(filepath, "/");
		//printf("Here3\n");
		printf("3. %s\n", filepath);
        	strcat(filepath, directory_name);
		//printf("here4\n");
		printf("\tFound Directory - %s\n", filepath); //dir here 
		ver = compare(write_file,filepath);
		write_dir_to_ad(filepath,write_file, ver);
		root = opendir(filepath);
	//	flag = 0;
	}
	else{  //root here
		null_flag = 1;
		printf("NULL\n");
		printf("\tFound Root Directory - %s\n", directory_name);
		ver = compare(write_file, directory_name);
		write_dir_to_ad(directory_name, write_file, ver);
		root = opendir(directory_name);
	}
	
	printf("Hello - %d\n", root);	
	printf("Print %s\n", strerror(errno));
	if(root){
		element = readdir(root);
		while(element !=NULL){

			if((!strcmp(element->d_name,"."))||(!strcmp(element->d_name,".."))){
				//printf("stuck\n");
				element = readdir(root);
				continue;
			}	
				
			if(element->d_type == DT_DIR){ 
	

				strcpy(name, element->d_name);
//				if (file_path !=NULL){
//				printf("Passing file path - %s,  and file name - %s\n",filepath, name);
				if (null_flag == 0){ //folders inside the first level
					printf("Internal dir passing through\n");
					printf("Passing file path - %s,  and file name - %s\n",filepath, name);
					//compare(write_file,filepath );
					parse_directory_list(filepath, name, write_file);
				}
				else{ //actually this is the first folder it encounters, not root dir
					printf("Root dir passing through\n");
					printf("Passing file path - %s,  and file name - %s\n",directory_name, name);

					parse_directory_list(directory_name, name, write_file);
				}

			}
			else{
				strcpy(name, element->d_name);
				if(null_flag ==0){
					printf("Internal file passing through\n");
					strcpy(file_name, filepath);
					strcat(file_name, "/");
					strcat(file_name,name );
					printf("File path - internal directory files: %s\n", file_name);
//					write_to_ad(write_file);
				}
				else{
					printf("Root file pass through\n");
					strcpy(file_name, directory_name);
					strcat(file_name, "/");
					strcat(file_name, name);
					printf("File path - files in root directory: %s\n", file_name);
				}
					ver = compare(write_file, file_name);
					write_to_ad(file_name, write_file, ver);
				//printf("\t\t%s\n",element->d_name);
			}
			element = readdir(root);

		}	
		closedir(root);
	}

//

}
