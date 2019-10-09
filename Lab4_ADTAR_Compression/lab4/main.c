#include <stdio.h>
#include "common.h"
#include <string.h>
#include <errno.h>
int main(int argc, char** argv){
	int i;
	char* archive_name;
	char* directory_name;
	FILE* archive_tar;
	int archive = 0;
	char option;
	//FILE* write_file;
	if (argc < 3){
		printf("Invalid number of arguments\n");
		return(0);
	}
	for(i = 1; i< argc; i++){
		if(!strcmp(argv[i], "-c")){
			option = 'c';	
			printf("Store in archive file\n");
		}
		else if(!strcmp(argv[i], "-a")){
			option = 'a';
			printf("Append filesystem\n");
		}
		else if(!strcmp(argv[i], "-x")){
			option = 'x';
			printf("Extract all files\n");
		}
		else if(!strcmp(argv[i], "-m")){
			option = 'm';
			printf("Print out the metadats\n");
		}
		else if(!strcmp(argv[i], "-p")){
			option = 'p';
			printf("Display the hierarchies of the files\n");
		}
		else if(argv[i][0] != '-'){
			if(archive == 0){
				archive_name = argv[i];	
				printf("Archive filename obtained\n");
				archive = 1; 
			}			
			else{
				directory_name = argv[i];
				printf("Directory name ontained\n");
			}	
		}
	} //close for loop
//	printf("Outside loop: %s\n",directory_name);
       if(option=='c'){
                        archive_tar = fopen(archive_name, "w+b"); //write
                        struct stat name;
			stat(directory_name, &name);
			if (file_type(name.st_mode) == ISDIR){
				parse_directory_list(NULL, directory_name, archive_tar);
                        }
			else{
				write_to_ad(directory_name, archive_tar, 1);
			}
			fclose(archive_tar);}

	else if(option=='a'){
                        archive_tar = fopen(archive_name, "a+b"); //append
                        struct stat name;
			int ver;
                        ver = compare(archive_tar, directory_name);
                        
			stat(directory_name, &name);
                        if (file_type(name.st_mode) == ISDIR){
                                
				parse_directory_list(NULL, directory_name, archive_tar);
                        }
                        else{
                                //ver = compare(archive_tar, directory_name);
				write_to_ad(directory_name, archive_tar,ver);
                        }

                        fclose(archive_tar);}

	else if(option=='x'){
          //              archive_tar = fopen(archive_name, "rb");
			//version number of argv[3]?
			read_from_ad(archive_name);               
        //                fclose(archive_tar);} //for readingi
			}
	else if(option=='m'){
			archive_tar = fopen(archive_name, "rb");
			if (archive_tar == NULL){printf("Error: %s\n", strerror(errno));}
			//printf("Here\n");
			print_ad_metadata(archive_tar);
			fclose(archive_tar);	
	}
	else if (option == 'p'){
			archive_tar = fopen(archive_name, "rb");
                        if (archive_tar == NULL){printf("Error: %s\n", strerror(errno));}
                        //printf("Here\n");
                        print_dir_list(archive_tar);
                        fclose(archive_tar);
			

	}		

	return 0;
} 

