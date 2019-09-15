#!/bin/bash
PWDPATH=`pwd`
SRC_PATH=../src/thirdPart/
INC_PATH=../inc/thirdPart/

TYPE=.tar.gz

CURL_VERSION=curl-7.51.0

echo "###### build third part lib ######"

function build_curl()
{   
    SRC_FILE=$CURL_VERSION$TYPE
    tar -xzvf $SRC_PATH$SRC_FILE
    cd $SRC_PATH$CURL_VERSION
    ./confiure -prefix=$PWDPATH$INC_PATH
    make && make install
    cd $PWDPATH
    echo "Build curl OK"
}


echo "###### build curl start ######"
build_curl 
echo "###### build curl end ######"



