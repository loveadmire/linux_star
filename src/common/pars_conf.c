//
// Created by root on 19-4-18.
//

#include "pars_conf.h"
#include "cJSON_Utils.h"
#include "cJSON.h"
#include "Make_cfg_file.h"
#include "Read_file.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define OK                 0
#define OPENERR           -1
#define CONFIGNOEEXUIT    -2
#define READERR           -3
#define KEYNOTEXIT        -4

int Get_log_item(char *filepath,char *item){
    int file_size = 0;
    int ret = 0;
    if (!filepath) {
        ret = Read_file_size(DEFAULT_CFG_FILE, &file_size, NULL);
    }
    else{
        ret = Read_file_size(filepath, &file_size, NULL);
    }

    if (ret < 0){
        return -1;
    }
    char *config_data = NULL;
    config_data = (char *)malloc(sizeof(char)* file_size);
    memset(config_data,0,file_size);

    Read_file_size(DEFAULT_CFG_FILE,&file_size,config_data);

    cJSON *root = cJSON_Parse(config_data);
    if(!root){
        return -1;
    }
    if (cJSON_GetObjectItem(root,"log_item")){
        if (cJSON_GetObjectItem(cJSON_GetObjectItem(root,"log_item"),item)) {
            ret = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "log_item"), item)->valueint;
        }
    }
    cJSON_Delete(root);
    free(config_data);
    config_data = NULL;

    return ret;
}

int get_key_string(char *file, char *key, char *key_val) {
    int file_size = 0;
    int ret = 0;
    char file_buff[1024] = { 0 };
    FILE *fp = NULL;
    if (file) {
        fp = fopen(file, "r");
        if (!fp) {
            return OPENERR;// 打开配置文件打开失败
        }
    }
    else {
        return CONFIGNOEEXUIT;// 配置文件不存在
    }
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);
    ret = fread(file_buff, 1, file_size, fp);
    if (ret <= 0) {
        fclose(fp);
        return READERR; //读取文件失败
    }
    fclose(fp);
    cJSON *json = cJSON_Parse(file_buff);
    if (cJSON_GetObjectItem(json, key)) {
        strcpy(key_val, cJSON_GetObjectItem(json, key)->valuestring);
        //key_val = key;
        //printf("key is %s key_val is %s ", key, key_val);
        cJSON_Delete(json);
        return OK;
    }
    else {
        cJSON_Delete(json);
        return KEYNOTEXIT;//获取键值失败
    }
}
