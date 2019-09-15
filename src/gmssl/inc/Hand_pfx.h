//
// Created by root on 19-3-5.
//

#ifndef STATION_HAND_PFX_H
#define STATION_HAND_PFX_H

#include "../../thirdPart/inc/openssl/pem.h"
#include "../../thirdPart/inc/openssl/ssl.h"
#include "../../thirdPart/inc/openssl/pkcs12.h"
#include <curl/curl.h>

//typedef int bool;

#define CONFILE "./config"


//获取crt 证书的内容
int Get_public_key_crt(char *);

int PraseP12Cert(char *p12Cert,char *p12pass,
                  char *pubCertFile,char *priCertFile,int format_pem);
int mite_string_2_base64_block(char *input,int input_len,char **base64);
char *Test_Encode(char *source,int len);

int PEM_cert_get_user_info(char *filename,char* out_data,int iteam);

int DER_cert_get_user_info(char *filename,char *out_data,char* iteam);



#endif //STATION_HAND_PFX_H
