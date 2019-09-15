//
// Created by root on 19-3-5.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Hand_pfx.h"
#include <openssl/ssl.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string.h>
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "Read_file.h"
#include "String.h"
#include "pars_conf.h"
#include "logger.h"


#define false 0
#define true 1



char *Test_Encode(char *source,int len){
    CURL *curl = curl_easy_init();
    char *cres = curl_easy_escape(curl,source,len);
    curl_easy_cleanup(curl);
    return cres;
}

int mite_string_2_base64_block(char *input,int input_len,char **base64)
{
    int len = 0;
    char *out = NULL;
    out = (char *)malloc((input_len*4/3+4));
    len = EVP_EncodeBlock((unsigned char *)out,(unsigned char *)input,input_len);
    *base64 = out;
    return len;
}

//解析P12的证书 和私钥，并写入文件
int PraseP12Cert(char *p12Cert,char *p12pass,
                  char *pubCertFile,char *priCertFile,int format_pem)
{
    if (p12Cert == NULL)
    {
        return false;
    }
    EVP_PKEY *key=NULL;
    X509 *cert=NULL;
    STACK_OF(X509) *ca = NULL;
    BIO * bio=NULL,*bioCert=NULL,*bioKey=NULL;
    PKCS12 *p12=NULL;
    int i,j,ret;

    OpenSSL_add_ssl_algorithms();
    bio=BIO_new_file(p12Cert, "r");
    p12 = d2i_PKCS12_bio(bio, NULL);
    if (!PKCS12_parse(p12,p12pass, &key, &cert, &ca))
    {
        BIO_free(bio);
        return false;
    }
    PKCS12_free(p12);
    char file_path[128] = "./";
    char client_file_name[128] = { 0 };
    char client_key_name[128] = { 0 };
//    ret = get_key_string(CONFILE, "vpnconf", file_path);
//    if (ret < 0) {
//        printf("Read config file path ERROR\n");
//        return false;
//    }
//    else {
//        sprintf(client_file_name, "%s%s", file_path, "client.crt");
//        sprintf(client_key_name, "%s%s", file_path, "client.key");
//    }
    sprintf(client_file_name, "%s%s", file_path, "client.crt");
    sprintf(client_key_name, "%s%s", file_path, "client.key");
    bioCert = BIO_new_file(client_file_name, "w");              //建立文件保存证书数据
    if (!bioCert) {
        printf(" 解析文件失败，查看用户读写权限！\n");
        return false;
    }
    bioKey = BIO_new_file(client_key_name, "w");
    if (!bioKey) {
        printf(" 解析文件失败，查看用户读写权限！\n");
        return false;
    }
    //bioKey = BIO_new_file("/usr/local/sslvpn/conf/client_p12.key", "w");           //建立文件保存本地私钥
    int  FORMAT_PEM = 1;
    int  FORMAT_DER = 0;
    char *base64_crt = NULL;
    char *buff = NULL;
    char *buff_key = NULL;
    int  base64_len = 0;
    char *base64_codec = NULL;
    int  filelen = 0;
    int  i_cert = 1;
    if(format_pem == FORMAT_PEM)
    {                                                                           //获取证书文件包含BEGIN END
        i = PEM_write_bio_X509(bioCert, cert);
        j = PEM_write_bio_PrivateKey(bioKey, key, NULL, NULL, 0, 0, NULL);
    }
    if (format_pem == FORMAT_DER)                          //获取证书文件不包BEGIN END
    {
        i = i2d_X509_bio(bioCert, cert);
        j = i2d_PrivateKey_bio(bioKey, key);
    }
    BIO_free(bio);
    BIO_free(bioCert);
    BIO_free(bioKey);
    X509_free(cert);
    EVP_PKEY_free(key);
    EVP_cleanup();
    if(!i_cert)
    {
        return false;
    }
    FILE *fp =  NULL;
    fp = fopen(client_file_name, "rb");
    if(fp == NULL )
    {
        printf("open file fail !");
        fclose(fp);
        return false;
    }
    else // 对证书文件进行base64编码
    {
        fseek(fp,0,SEEK_END);                //get file size
        filelen = ftell(fp);
        rewind(fp);
        buff = (char *)malloc(filelen + 1);  //save file data
        memset(buff,0,filelen+1);
        fread(buff,filelen,1,fp);
        base64_len = mite_string_2_base64_block(buff,filelen,&base64_crt);
        base64_codec = Test_Encode(base64_crt,base64_len);
        fclose(fp);
    }
    //将文件保存为base64编码文件用于上传验证
    fp = fopen(client_file_name,"wb");
    if(fp == NULL)
    {
        printf("Create file failure check authority!");
        fclose(fp);
        return false;
    }
    else
    {
       // fwrite(base64_codec,1,strlen(base64_codec), fp);  //寫入編碼以後的數據
        fwrite(buff,1,strlen(buff), fp);
        fclose(fp);
        free(buff);
    }
    //对密钥文件进行base64编码
    fp = fopen(client_key_name, "rb");
    if (fp == NULL) {
        printf("open file fail !");
        fclose(fp);
        return false;
    }
    else {
        fseek(fp, 0, SEEK_END);                //get file size
        filelen = ftell(fp);
        rewind(fp);
        buff_key = (char *)malloc(filelen + 1);  //save file data
        memset(buff_key, 0, filelen + 1);
        fread(buff_key, filelen, 1, fp);
        base64_len = mite_string_2_base64_block(buff_key, filelen, &base64_crt);
        base64_codec = Test_Encode(base64_crt, base64_len);
        fclose(fp);
        //保存client.key文件
        fp = fopen(client_key_name, "wb");
        if (fp == NULL) {
            printf("Create file failure check authority!");
            fclose(fp);
            return false;
        }
        else {
            //fwrite(base64_codec, 1, strlen(base64_codec), fp);
            fwrite(buff_key, 1, strlen(buff_key), fp);
            fclose(fp);
            free(buff_key);
        }
    }
    free(base64_crt);
    free(base64_codec);
    return true;
}
int PEM_cert_get_user_info(char *filename,char* out_data,int iteam){
    X509 *cert =NULL;
    FILE *fp =  NULL;
    int iLen = 0;
    char csBuf[256] = {0};
    X509_NAME *pSubName = NULL;
    fp = fopen(filename,"r");
    if(!fp){
        fprintf(stderr, "unable to open: %s\n", filename);
        return -1;
    }
    cert = PEM_read_X509(fp,NULL,NULL,NULL);
    //get public_key test
//    BIO *bio = NULL;
//    X509 *x509 = NULL;
//    bio = BIO_new_file(filename,"r");
//    x509 = PEM_read_bio_X509(bio,NULL,NULL,NULL);
//    EC_KEY *key =NULL;
//    key = X509_get_pubkey(x509);
//    if (!key){
//        printf(" get public key error \n");
//        return -1;
//    }
//    printf( " version is %d \n",key->version);


    if(!cert){
        fprintf(stderr, "unable to parse\n");
        return -1;
    }
    pSubName = X509_get_subject_name(cert);
    if(!pSubName){
        printf("get subject name fail!\n");
        return -1;
    }
    iLen = X509_NAME_get_text_by_NID(pSubName,iteam, csBuf, 256);
    if(iLen> 0){
        strcpy(out_data,csBuf);
    }
    fclose(fp);
    return 0;
}

int DER_cert_get_user_info(char *filename,char *out_data,char* iteam){

    int ret = 0;
    char *cert_buff = NULL;
    X509 *cert = NULL;
    unsigned int file_szie = 0;
    ret = Read_file_size(filename,&file_szie ,NULL);
    if(ret < 0){
        printf("read der cert fail!\n");
        return -1;
    }
    cert_buff = (char *)malloc(file_szie*(sizeof(char)));
    ret = Read_file_size(cert_buff, filename, cert_buff);
    if(ret < 0){
        printf("read der cert data fail!\n");
        return -1;
    }
    ASN1_INTEGER *Serial =NULL;
    cert = X509_new();
    cert = d2i_X509(NULL,(const unsigned char **)&cert_buff,ret);
    //printf("certbuff address is %p  is %s \n",cert_buff);
    Serial = X509_get_serialNumber(cert);
    X509_NAME *name =NULL;
    name = X509_get_subject_name(cert);
    if(!name){
        printf("get user name error!\n");
        return -1;
    }
    char buff[256] = {0};
    ret = X509_NAME_get_text_by_NID(name,NID_countryName,buff,256);
    if(ret < 0){
        printf(" get user NID_contryName fail!\n");
        return -1;
    }

    int ver = -1;
    ver = X509_get_version(cert);
    switch(ver){
        case 0:printf("V1 \n");break;
        case 1:printf("v2 \n");break;
        case 2:printf("v3 \n");break;
        default:
            printf("unknow version\n");
            break;
    }

    BIGNUM *Bignum = NULL;
    Bignum = ASN1_INTEGER_to_BN(Serial, NULL);
    char *serial = NULL;
    serial = BN_bn2hex(Bignum);
    ret =A_2_a(serial);
    printf("serial is %s \n",serial);
    return 0;

}