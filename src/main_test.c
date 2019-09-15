#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "cJSON.h"
#include "cJSON_Utils.h"
#include "logger.h"
#include "Make_cfg_file.h"
#include "Read_file.h"
#include "websocket_common.h"

#include "gmssl_debug.h"
#include "epoll_func.h"
#include "Sort_func.h"
#include "queue.h"
#include "pars_conf.h"
unsigned int log_level = 0;
unsigned int log_size = 0 ;

int main(int argc,char *argv[]){

    int ret = 0;
    ret = Make_default_cfg();
    if (ret  < 0){
        PRINT_MYLOG(LOG_TYPE_ERROR,"make default config file  error !\n");
        return ret;
    }
    log_level = Get_log_item(DEFAULT_CFG_FILE,"log_level");
    printf("log_level is %d \n",log_level);
    log_size = Get_log_item(DEFAULT_CFG_FILE,"log_size");
    printf("log_size is %d \n",log_size);

    if(argc == 1 ) {
        printf("Please input \" --help \" to get information\n");
        return 0;
    }
    if(argc >= 2) {
        if (strcmp(argv[1], "--help") == 0) {
            printf("epoll server      : program epoll_server port     (./program epoll_server 8080)\n");
            printf("gmssl_client_demo : program gmssl_client port ip  (./program gmssl_client 443 127.0.0.1)\n");
            printf("gmssl_server_demo : program gmssl_server port ip  (./program gmssl_server 443 127.0.0.1)\n");
            printf("gmssl_mast_key    : program gmssl_server port ip  (./program gmssl_mast_key)\n");
            return 0;
        }
        if (strcmp(argv[1], "epoll_server") == 0) {
            main_epoll((unsigned short) atoi(argv[2]));
        }
        if (strcmp(argv[1], "gmssl_client") == 0)
            gmssl_client_demo((unsigned short) atoi(argv[2]), argv[3]);
        if (strcmp(argv[1], "gmssl_server") == 0)
            gmssl_server_demo((unsigned short) atoi(argv[2]), argv[3]);
        if (strcmp(argv[1], "gmssl_mast_key") == 0) {
            ret = gmssl_mask_secrit();
            return ret;
        }
        if(strcmp(argv[1],"sort_fun") == 0){
            int a[] = {10,30,20,40,100,80,60,34};
            int a_len = sizeof(a)/sizeof(a[0]);
            quick_sort_int(a,0,a_len -1);
            int i = 0;
            for(i = 0;i<a_len;i++){
                printf(" %d ",a[i]);
            }
        }
        if(strcmp(argv[1],"web_server") == 0) {
            web_socket_server();
        }
        if(strcmp(argv[1],"log_test") == 0) {
            log_level = Get_log_item(DEFAULT_CFG_FILE,"log_level");
            printf("log_level is %d \n",log_level);
            log_size =  Get_log_item(DEFAULT_CFG_FILE,"log_size");
            printf("log_size is %d \n",log_size);
            char *str = "log test hello";
            PRINT_MYLOG(LOG_TYPE_WARN," %s 1  %d \n",str,32);
            PRINT_MYLOG(LOG_TYPE_WARN,"%s 1 %d \n",str,32);
        }
        if(strcmp(argv[1],"datastruct") == 0) {
            queue_test_01();


            return 0;
        }

    }





    return 0;
}