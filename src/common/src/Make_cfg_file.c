//
// Created by root on 19-4-17.
//

#include "../inc/Make_cfg_file.h"
#include "../../json/inc/cJSON.h"
#include "../../json/inc/cJSON_Utils.h"
#include <stdio.h>
#include <string.h>


int Make_default_cfg(void){
    cJSON *config_root = cJSON_CreateObject(); //创建日志根节点

    cJSON *net_config = cJSON_CreateObject(); //创建网络相关配置 根节点

    cJSON_AddStringToObject(net_config, "reportIP","127.0.0.1" );
    cJSON_AddNumberToObject(net_config, "port", 8080);
    cJSON_AddNumberToObject(net_config, "local", 6523);
    cJSON_AddNumberToObject(net_config, "localSSL", 6522);
    cJSON_AddNumberToObject(net_config, "port", 6638);

    cJSON_AddItemToObjectCS(config_root,"net_item",net_config);

    cJSON *log_json_root = cJSON_CreateObject(); //创建json log 根节点
    cJSON_AddNumberToObject(log_json_root,"log_level",7);
    cJSON_AddNumberToObject(log_json_root,"log_size",10);   //default 10M
    cJSON_AddItemToObjectCS(config_root,"log_item",log_json_root);

    char *jsondata = cJSON_Print(config_root);


    FILE *fp = fopen(DEFAULT_CFG_FILE, "w");
    if (!fp){
        return -1;
    }
    fwrite(jsondata, strlen(jsondata), 1, fp);
    fclose(fp);
    cJSON_Delete(config_root);
    return 0;
}