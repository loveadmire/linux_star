//
// Created by root on 19-4-19.
//

#include "gmssl_debug.h"
#include "logger.h"
#include <string.h>
#include <stdio.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/ecdsa.h>
#include <openssl/objects.h>
#include <openssl/err.h>

int gmssl_mask_secrit(){
    EC_KEY *key1,*key2;
    const EC_POINT *pubkey1,*pubkey2;
    EC_GROUP *group1,*group2;
    unsigned int ret ,nid,size,i,sig_len;
    unsigned char *signature,digest[20];
    BIO *berr;
    EC_builtin_curve *curves;
    int curve_len  = 0;
    char sharekey1[128],sharekey2[128];
    int len1 ,len2 ;
    key1 = EC_KEY_new();
    if (NULL == key1 ){
        PRINT_MYLOG(LOG_TYPE_ERROR,"EC_KEY_new error \n");
        return -1;
    }
    key2 = EC_KEY_new();
    if (NULL == key2 ){
        PRINT_MYLOG(LOG_TYPE_ERROR,"EC_KEY_new error \n");
        return -1;
    }
    curve_len = EC_get_builtin_curves(NULL,0);
    curves = (EC_builtin_curve*)malloc(sizeof(EC_builtin_curve)*curve_len);
    EC_get_builtin_curves(curves,curve_len);
    nid = curves[25].nid;
    PRINT_MYLOG(LOG_TYPE_DEBUG,"curve_name is %d  \n",nid);
    group1 = EC_GROUP_new_by_curve_name(nid);
    if ( NULL   == group1){
        PRINT_MYLOG(LOG_TYPE_ERROR,"EC_GROUP_new_by_curve_name  1 error \n");
        return -1;
    }
    group2 = EC_GROUP_new_by_curve_name(nid);
    if ( NULL   == group2){
        PRINT_MYLOG(LOG_TYPE_ERROR,"EC_GROUP_new_by_curve_name  2 error \n");
        return -1;
    }
    ret = EC_KEY_set_group(key1,group1);
    if (ret != 1){
        PRINT_MYLOG(LOG_TYPE_ERROR,"EC_KEY_set_group 1 error \n");
        return -1;
    }
    ret = EC_KEY_set_group(key2,group2);
    if (ret != 1){
        PRINT_MYLOG(LOG_TYPE_ERROR,"EC_KEY_set_group 2 error \n");
        return -1;
    }
    ret = EC_KEY_generate_key(key1);
    if (ret != 1){
        PRINT_MYLOG(LOG_TYPE_ERROR,"EC_KEY_generate_key 1 error \n");
        return -1;
    }
    ret = EC_KEY_generate_key(key2);
    if (ret != 1){
        PRINT_MYLOG(LOG_TYPE_ERROR,"EC_KEY_generate_key 2 error \n");
        return -1;
    }
    ret = EC_KEY_check_key(key1);
    if (ret != 1){
        PRINT_MYLOG(LOG_TYPE_ERROR,"EC_KEY_check_key 1 error \n");
        return -1;
    }
    size = ECDSA_size(key1);
    PRINT_MYLOG(LOG_TYPE_INFO,"ECDSA_size %d \n",size);
    for( i = 0;i < 20;i++){
        memset(&digest[i],i+1,1);
    }
    signature = (unsigned char*)malloc(size);
    ERR_load_crypto_strings();
    berr = BIO_new(BIO_s_file());
    ret = ECDSA_sign(0,digest,20,signature,&sig_len,key1);
    if (ret != 1){
        ERR_print_errors(berr);
        PRINT_MYLOG(LOG_TYPE_INFO,"ECDSA_sign  error  %d \n",ret);
    }
    ret = ECDSA_verify(0,digest,20,signature,sig_len,key1);
    if (ret != 1){
        ERR_print_errors(berr);
        PRINT_MYLOG(LOG_TYPE_INFO,"ECDSA_verify  error  %d \n",ret);
    }
    pubkey2 = EC_KEY_get0_public_key(key2);
    len1 = ECDH_compute_key(sharekey1,128,pubkey2,key1,NULL);
    pubkey1 = EC_KEY_get0_public_key(key1);
    len2 = ECDH_compute_key(sharekey1,128,pubkey1,key2,NULL);
    if (len1 != len2){
        PRINT_MYLOG(LOG_TYPE_INFO,"ECDH_compute_key  error  len1 = %d len2 = %d \n",len1,len2);
    }
    else{
        ret = memcpy(sharekey1,sharekey2,len1);
        if (ret == 0){
            PRINT_MYLOG(LOG_TYPE_INFO,"gentra mask key ok \n");
        }
        else{
            PRINT_MYLOG(LOG_TYPE_INFO,"gentra mask key fail \n");
        }
    }
    BIO_free(berr);
    EC_KEY_free(key1);
    EC_KEY_free(key2);
    free(signature);
    free(curves);
    return 0;



}