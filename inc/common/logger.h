#ifndef LOGGER_H
#define LOGGER_H
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "Read_file.h"

#define USE_MYLOG
#define LOG_FILE "../config/test_process.log"           //log location
#define LOG_TAG "LOG"
extern unsigned int log_level ;
extern unsigned int log_size ;

#define NO_USE_SYSLOG
#ifndef NO_USE_SYSLOG
#include <sys/syscall.h>
#include <syslog.h>
//  syslog  level
typedef enum _sysType{
    LOG_SYS_INFO,
    LOG_SYS_ALERT,
    LOG_SYS_EMERG,
    LOG_SYS_CRIT,
    LOG_SYS_NOTICE,
    LOG_SYS_WARNING,
    LOG_SYS_ERR,
    LOG_SYS_DEBUG,
}Type_syslog;
static const char* TypeToString_syslog(const Type_syslog type)
{
    switch (type) {
        case LOG_SYS_INFO:
            return "LOG_INFO   ";
        case LOG_SYS_ALERT:
            return "LOG_ALERT  ";
        case LOG_SYS_EMERG:
            return "LOG_EMERG  ";
        case LOG_SYS_CRIT:
            return "LOG_CRIT   ";
        case LOG_SYS_NOTICE:
            return "LOG_NOTICE ";
        case LOG_SYS_WARNING:
            return "LOG_WARNING";
        case LOG_SYS_ERR:
            return "LOG_ERR    ";
        case LOG_SYS_DEBUG:
            return "LOG_DEBUG  ";
        default:
            break;
    }
    return "UNKNOWN";
}

#define PRINT_SYSLOG(type,format,...)\
{\
    do{\
    if(log_level < type){}\
    else{\
        char msg[256]={0};\
        sprintf(msg, " %s {%s:%d} " format,TypeToString_syslog(type),__FILE__,__LINE__,##__VA_ARGS__);\
        openlog(LOG_TAG, LOG_CONS | LOG_PID,0);\
        syslog(type, msg);\
        closelog();\
       }\
       }while(0);\
}
#endif


//  my log file  level
typedef enum _Type{
    LOG_TYPE_FATAL,
    LOG_TYPE_WARN,
    LOG_TYPE_INFO,
    LOG_TYPE_NOTICE,
    LOG_TYPE_ERROR,
    LOG_TYPE_DEBUG,
}Type;



static const char* TypeToString_self(const Type type ){
    switch( type ) {
        case LOG_TYPE_FATAL:
            return "FATAL";
        case LOG_TYPE_WARN:
            return "WARN ";
        case LOG_TYPE_INFO:
            return "INFO ";
        case LOG_TYPE_NOTICE:
            return "NOTICE ";
        case LOG_TYPE_ERROR:
            return "ERROR";
        case LOG_TYPE_DEBUG:
            return "DEBUG";
        default:
            break;
    }
    return "UNKNOWN";
}

#define PRINT_MYLOG(type,format,...)\
{\
    do{\
        if(log_level < type){;}\
        else{\
            FILE *fp = NULL;\
            char msg[256] = {0};\
            if(log_size*1024*1024 > Get_file_size(LOG_FILE)){fp = fopen(LOG_FILE,"a+");}\
            else{\
                fp = fopen(LOG_FILE,"w");\
            }\
            struct tm *ptr = NULL;\
            time_t ltime;\
            ptr = localtime(&ltime);\
            sprintf(msg," %02d-%02d-%02d-%02d-%02d  %s : %s {%s:%d}  "format,ptr->tm_mon,ptr->tm_mday,ptr->tm_hour,ptr->tm_min,ptr->tm_sec,LOG_TAG,TypeToString_self(type),__FILE__,__LINE__,##__VA_ARGS__);\
            fwrite(msg,1,strlen(msg),fp);\
            fclose(fp);\
        }\
       }while(0);\
}

/*传入数据未知原因会丢失，所以参数先接收备份，原因待查*/
#define PRINTF_MESSAGE(len,str,flag,message)\
{\
    printf(" Information %s %s %s %d  \n",message,__FUNCTION__,__FILE__,__LINE__);\
    if(flag == 1 && strlen(str) != 0){\
        int inlen = len;\
        int i = 0;\
        for(i =0 ;i < len;i++){\
            if( (i+1)%16 == 1 ) printf("%04x ",i);\
            printf("%02x%s",str[i],(i+1)%16?((i+1)%16==8?"-":" "):"\n");\
        }\
        printf("\n");\
    }\
 }
#define PRINTF_FORMAT(format,...)\
{\
    char meg[256] = {0};\
    sprintf(meg,"%s %s %d  \n" format,__FUNCTION__,__FILE__,__LINE__,##__VA_ARGS__);\
    printf(" \n %s \n",meg);\
}


#endif // LOGGER_H
