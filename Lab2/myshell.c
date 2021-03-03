/* wqg203, Berwin Gan */
#include "function.h"



//shell main
int main(int argc, char **argv){
        char cmdline[LINE];
        real_pwd(base_path);
        real_pwd(dir_path);
        real_pwd(history_path);
        strcat(history_path,"/history.txt"); //all paths are stored in header fil
        real_pwd(export_path);
        strcat(export_path,"/export.txt");
        real_pwd(tmp);
        strcat(tmp,"/tmp.txt");
        real_pwd(PWD_path);
 
        //set up for export
        FILE* if_there;
        if(if_there=fopen(export_path,"r")){
                fclose(if_there);
        //      printf("%s","the file exist");
        }
        else{
        int trg =open(export_path,O_WRONLY|O_CREAT,0644);
        write(trg,"PATH=\n",6);
        write(trg,"PWD=",4);
        write(trg,PWD_path,strlen(PWD_path));
        write(trg,"\n",1);
        close(trg);//nice #starthere
        } 
        while(1){ 
                printf("%s",prompt);
                fgets(cmdline, LINE, stdin);
                //what is enter character

                
		eval(cmdline);
        }
//remove export ?       
        exit(0); 
}

