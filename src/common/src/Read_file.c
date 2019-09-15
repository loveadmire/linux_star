#include "../inc/Read_file.h"
#include <stdio.h>
#include <sys/stat.h>

int Read_file_size(char *file_path, int *file_size,char *buff_out){
    if(!file_path) {
        return -1; //file_path is error
    }
    FILE *fp = NULL;
    int ret  = 0;
    fp = fopen(file_path,"r");
    if(!fp){
        return -2; //open file fail
    }
    fseek(fp,0,SEEK_END);
    *file_size =  ftell(fp);
    if(!buff_out) {   //!* 从右到左
        fclose(fp);
        return 0;
    }
    else{
        rewind(fp);
        ret  = fread(buff_out,1,*file_size,fp);
        if (ret <= 0){
            fclose(fp);
            return -3;  //read file fail
        }
        return 0;
    }
}

int Get_file_size(char *filepath){
    struct stat statbuf;
    stat(filepath , &statbuf);
    return statbuf.st_size;
}