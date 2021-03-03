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
#define _POSIX_SOURCE 1

void eval(char *cmdline);
int parseline(const char *cmdline, char **arr);
void record(int num, char** arr, char *path);
void pwd();
int command(char **arr,int argu_number);
//int path_find(char**arr, int argu_number);
void expo(char **arr, int argu_number);
void record_export(int num,char** arr, char* path);
int cutpizza(const char *cmdline, char **arr);
void pizza(char **arr);
void hist(char **arr);
void change_director(char **arr,int argu_number);
void orginal_pwd(char *b);
void individual_export_read();
void read_export_by_line();
int unix_cmd(int argu_num, char **arr);
void path_replacement(int argu_num,char **arr);
int special_parseline(const char *cmdline, char **arr);

int path_run(char **all_path);
//ALL PATH , need add PWD
char base_path[LINE];
char dir_path[LINE];
char history_path[LINE];
char export_path[LINE];
char tmp[LINE];
char PWD_path[LINE]; // have to bzero at every chdir

void real_pwd(char* a){
        char b[LINE];
        getcwd(b,sizeof(b));
        strcpy(a,b);
}

int unix_cmd(int argu_num, char **arr){ //check for access in each path line and if there is replace the first argument with it and send back 1
        FILE *linereader = fopen(export_path,"r");
        char each_line[LINE];
        int i = 0; int bk = 0;
        char new_pt[LINE];
        while((bk==0) &&(fgets(each_line,sizeof each_line, linereader) != NULL)){ //read each line till NULL
                char * rain_check[ARGU];
                each_line[strlen(each_line)-1]='\0';
                int vamp= cutpizza(each_line,rain_check); //vamp gives number of argument
                if(vamp>1){
                        int num =0;
                        while(num!=(vamp-1)){
                                char ck_for_ac[strlen(rain_check[num+1])+strlen(arr[0])+1];
                                strcpy(ck_for_ac,rain_check[num+1]);strcat(ck_for_ac,"/"); strcat(ck_for_ac,arr[0]);
//                              printf("%li ~%s~\n",strlen(ck_for_ac),ck_for_ac);
                                if(access(ck_for_ac,X_OK)==0){strcpy(new_pt,ck_for_ac);bk++;break;}
                                i++;
                                num++;}
                }
}
        fclose(linereader);// need to put error checks everywhere later ?
        arr[argu_num] = NULL;
//------------------------------------------------------------------------------------- 
        int counter =0;
        while(counter!=argu_num){
                char word_holder[strlen(arr[counter])];
                strcpy(word_holder,arr[counter]);
                if((word_holder[0]=='$')&&(strlen(word_holder)>1)){ //replace the $PATH with whats behind.
                        char new_word[strlen(word_holder)]; int k = 0; while((k+1)!=strlen(word_holder)){new_word[k]=word_holder[k+1];k++;}
                        FILE *linereader = fopen(export_path,"r");
                        char each_line[LINE];           char* stuff_in_file[ARGU];
                        while(fgets(each_line,sizeof each_line, linereader) != NULL){
                        each_line[strlen(each_line)-1]='\0';
                        int vamp= cutpizza(each_line,stuff_in_file);
                        char first_word[strlen(stuff_in_file[0])]; strcpy(first_word,stuff_in_file[0]);int jun= 1;
                        if(!strcmp(new_word,first_word)){ char sub[LINE]; memset(sub,0,1);
                                        while(jun!=(vamp)){
                                                char right[LINE];
                                                strcpy(right,stuff_in_file[jun]);
                                                strcat(sub,right);
//                                              printf("jun is %i and vamp is %i insert is %s",jun,vamp,stuff_in_file[jun]);
                                                jun++; if(jun==vamp){break;}
                                                strcat(sub,":");
                                                } arr[counter] = sub;
                         }

                        }
                        fclose(linereader);
                }
                counter++;
                }
//---------------------------------------------------------------
//      printf("!%s!%li!\n",new_pt,strlen(new_pt));
        if(bk==1){arr[0] = new_pt;return 1;} //theres such a thing and it have been changed
        return 0;
} //gcc only works on OS server, IDK WHY

void carry_out(int argu_num, char**arr){ //exec here
        if(fork()==0){
                execvp(arr[0],arr);}
        else{wait();}
}
//for | need to store stuff on system first before pass through pipes
int command(char **arr,int argu_number){// need break down before continuing 
        int unix_c = unix_cmd(argu_number,arr); //need to combine with path run cause //if  == 1, means there is somesort of out cmd
//      if(unix_c==1){carry_out(argu_number,arr);}      

        int cmd_check =0;

//      printf("%s",(arr[0]));
        char ex_check[strlen(arr[0])];
        strcpy(ex_check,arr[0]);ex_check[strlen(arr[0])] ='\0';
        if(strcmp(arr[0],"cd")==0){change_director(arr,argu_number);cmd_check=1;}
        else if(unix_c == 1){carry_out(argu_number,arr);cmd_check = 1;}
        else if(strcmp(arr[0],"exit")==0){exit(0);} //add removal of path file 
        else if(strcmp(arr[0],"history")==0){ hist(arr);cmd_check=1;}
        else if(strcmp(arr[0],"export")==0){expo(arr,argu_number);cmd_check=1;}
        else if(strcmp(arr[0],"pwd")==0){pwd();cmd_check=1;}
//      else {int horse= path_find(arr,argu_number); cmd_check=horse;}//1 means its accessable ? others means it is printed already


        if (cmd_check==0){
                printf("%s","command not found\n");return 0;}

        else if(cmd_check==2){printf("%s","there is such a command\n");return 1;}//for anything to look at 


}


void expo(char **arr, int argu_number){ //1 means print, 2 means store// assuming all var is good
        //export + ' ' = 7 char// export PATH=/usr/local/bin:/usr/bin --> PATH= /usr/local/bin /usr/bin


        if (argu_number > 1){//will have two parts 1)PATH 2)/bin:/asdasd 
                char* frr[ARGU];
                char* stuff_in_file[ARGU];
                int count = cutpizza(arr[1],frr); //shows number of stuff PATH=/bas:/bin (3); need to compare frr[0] to check if need to change anything                
//              printf("%li",strlen(frr[0]));   
//              printf("frr[0] is %s joke",frr[0]);
                char spoon[strlen(frr[0])];
                char *s_spoon;
                s_spoon=strcpy(spoon,frr[0]);
//              printf("%s",spoon);     

                char cake[strlen(arr[1])];
                char *c_cake;
                c_cake= strcpy(cake,arr[1]);
//              printf("cake is %s",cake);

                FILE *linereader = fopen(export_path,"r");
                char each_line[LINE];
                int reuse =0;
                int kk = open(tmp,O_WRONLY|O_CREAT,0644);
                while(fgets(each_line,sizeof each_line, linereader) != NULL){ //read each line till NULL
                        int vamp= cutpizza(each_line,stuff_in_file);
                        char knife[strlen(stuff_in_file[0])];
                        char *k_knife;
                        k_knife=strcpy(knife,stuff_in_file[0]);
                        int over_w = 0;
                        if(strcmp(spoon,knife)==0){over_w = 1;write(kk,cake,strlen(cake));reuse=1;write(kk,"\n",1);} //wirte over then break
                        if(over_w == 0){write(kk,each_line,strlen(each_line));}
                        }
                fclose(linereader);// need to put error checks everywhere later ?
                close(kk); // delete old and rename new
                int apple = remove(export_path);
                int orange = rename(tmp,export_path);
                //else the path is entirely brand new which will be then used by record to record       
                if(reuse==0){record_export(argu_number,arr,export_path);}}
        else if (argu_number == 1){
                read_export_by_line();} //dam the damm char dam dam 
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

void read_export_by_line(){ //put each line in arr[] type by using parseline 
//      static const char file[] = export_path;
        FILE *linereader = fopen(export_path,"r");
        char each_line[LINE];
        while(fgets(each_line,sizeof each_line, linereader) != NULL){
                printf("%s",each_line);
                }
        fclose(linereader);// need to put error checks everywhere later ?

}


void pizza(char **arr){ //solely for reading path var 
        char * c =(char *) calloc(1000,sizeof(char));
        int file = open(export_path, O_RDONLY);
        int hotdog = read(file,c,1000);
        c[hotdog] = '\0'; //formating so there aint a gap under the last print and the next >>
        //first char is \n need to get rid      
        int party = 0;
        int first = 0;//use for =
        int ko = 0;
//      printf("%s",c);
        while (c[party]!= '\0'){ //this is the most lmao code for print ever dam 
                if (c[party]=='\n'){printf("%c",'\n'); first=0;}
                else if(isspace(c[party]) && (first==0)){printf("%c",'=');first++;}
                else if(isspace(c[party]) &&! isspace(c[party+1])){printf("%c",':');}
                else{printf("%c",c[party]);}
//              if(c[party] =='\n'){printf("%s","its here");}
                party++;
        }       //print correctly and change the \n counter     
//      printf("%s","\n");
        close(file);
}
void hist(char **arr){ // this is fine  
        char * c =(char *) calloc(1000,sizeof(char));
        int file = open(history_path, O_RDONLY);
        int hotdog = read(file,c,1000);
        c[hotdog-1] = '\0'; //formating so there aint a gap under the last print and the next >>
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


void change_director(char **arr, int argu_number){
        int changed =0;
//        char dir2_path[LINE]; strcpy(dir2_path,dir_path); strcat(dir2_path,"/");
//      strcat(dir2_path,arr[1]);
      if(argu_number == 1){chdir("..");changed = 1;}
       else if(chdir(arr[1])<0){ //not work
                printf("No such path\n");}
        else{ changed = 1;}
        if(changed == 1){ // get the new path and initiate the entire checking thing
                char tmp_path[LINE]; char a[LINE] =" export PWD=";
                real_pwd(tmp_path); strcat(a,tmp_path); strcat(a," ");
		printf("~%s~",a);
		eval(a);}
        return;
}
void pwd(){
//      printf("%s","pwd checki\n");
//      char *c = (char*)calloc(1000,sizeof(char));
        char b[2000];
        getcwd(b,sizeof(b));
        printf("%s",b);
        printf("%c",'\n');
}



void record(int num,char** arr, char* path){ // to be used after parseline
        int a = 0;
        int file = open(path, O_RDWR|O_APPEND|O_CREAT, 0644);
        while(a!=num){
                write(file,arr[a],strlen(arr[a]));
                a++;
                write(file," ",1);
                }
        write(file,"\n",1);
        close(file);
}

void record_export(int num,char** arr, char* path){ // to be used after parseline
        int a = 0;
        int file = open(path, O_RDWR|O_APPEND|O_CREAT, 0644);
        while(a!=(num-1)){
                write(file,arr[a+1],strlen(arr[a+1]));
                a++;
                write(file," ",1);
                }
        write(file,"\n",1);
        close(file);
}
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

void eval(char *cmdline){
//      if (cmdline ==NULL){return;}  QUESTION: WHAT HAPPENS WHEN YOU PRESS ENTER WITHOUT ANYTHING ?
        char * arr[ARGU]; //printf s should yield first argument
        int argu_number = parseline(cmdline, arr); // should also yield the number of argument in (CHECK)
//      if(strcmp(arr[0], "exit")==0){exit(0);} put in cmd_check to record everything
//      printf("%d",argu_number);
//      path_replacement(argu_number,arr); // go through arr and check for $ and replace them with appropriate line
//      printf("@%s %s@",arr[0],arr[1]);
        record(argu_number,arr,history_path); //special type of data 
        int check_data = 0; int total_pipe = 0;
        while(check_data!=argu_number){
                char a[strlen(arr[check_data])]; strcpy(a,arr[check_data]);
                char b[1] = "|";
                if(strcmp(a,b)==0){total_pipe++;}
                check_data++;}

        if(total_pipe == 0){
        int cmd_check = command(arr,argu_number);}//return a 0 if no such cmd, else 1(there is a cmd and do nothing), first ls must return 0
        else{ //do parsing for pipes divide it up to strings without | (as | axa |v ) =a axa v
                char * arr_pipe[ARGU];
                int num_of_list = special_parseline(cmdline,arr_pipe);
                int k = 0; //each requires its own command function and then pipe is used to pass info back and forth
                while(k!=num_of_list){printf("@%s@\n",arr_pipe[k]);k++;}
                }

                }



int special_parseline(const char *cmdline, char **arr){ //arr is the %s holder, last should be NULL
        int num = 0;//of argument 
        char * space;
        char local_holder[LINE]; //local cpy  DELETE
        char * ptr = local_holder; //pointer to first 
        strcpy(ptr,cmdline); //ptr is the first of holder which means 
        //ptr++;
        //replacing \n with ' '
        while (isspace(*ptr)){ptr++;} //front party gone,gets to first non white char, need take care of back
        ptr[strlen(ptr)-1] = '|';
        //printf("%c",*ptr); // currently at first char after blank space (\n) is last
        //if(ptr[strlen(ptr)-1] == '\n'){printf("%s","back space check");}
        //printf("%s",ptr);
        space = strchr(ptr,'|');
        while(space){ // for pointer, while is only false at '\0'
                arr[num] = ptr;//ptr is on the left of the space
                num++;
                // to move over the space
                ptr = space;
                ptr++;
                while(isspace(*ptr)){ptr++;}
                *space = '\0';
                space = strchr(ptr,'|'); //reuse for ':' in the path export thing 
                }
        arr[num] = NULL;// for last     
        if (num == 0){return (0);}
        else{ return (num);}
} //?????ASAK NANI

void path_replacement(int argu_num,char **arr){
                int cake = 0;
                int which; //the head of path to look for 
                while(cake!=argu_num){ //
//                      printf("%s",arr[cake]);
                        cake++;}
/*              FILE *linereader = fopen(export_path,"r");
                char each_line[LINE];
                while(fgets(each_line,sizeof each_line, linereader) != NULL){
                        printf("%s",each_line);
                }
                fclose(linereader);// need to put error checks everywhere later ?
*/

/*              char* stuff_in_file[ARGU];      
                FILE *linereader = fopen(export_path,"r");
                char each_line[LINE];
                while(fgets(each_line,sizeof each_line, linereader) != NULL){ //read each line till NULL
                        int vamp= cutpizza(each_line,stuff_in_file);
                        }
                fclose(linereader);// need to put error checks everywhere later ?
*/

}


