cmake_minimum_required(VERSION 3.10.2)
project(linux_star C)


#add src file name to value DIRSRCS   实现文件目录需要逐个添加
AUX_SOURCE_DIRECTORY(./src   DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/json/src   DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/common/src   DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/websocket_linux/src  DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/gmssl/src  DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/linux_net/src  DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/datastruct/src  DIRSRCS)




#添加头文件目录
INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} ./src/common/inc  ./src/json/inc ./src/datastruct/inc  )
INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} ./src/gmssl/inc    ./src/websocket_linux/inc ./src/linux_net/inc )

INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} ./src/thirdPart/inc/curl )
INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} ./src/thirdPart/inc/openssl )

#添加库目录
LINK_DIRECTORIES(${catkin_LIB_DIRS} ./lib/thirdPartLib )

#添加系统库目录
FIND_LIBRARY(RUNTIME_LIB rt /usr/lib /usr/local/lib NO_DEFAULT_PATH)

set(CMAKE_C_STANDARD 99)

#
set(GNU_FLAGS "-m64 -static -fPIC -Wall -O2 -MMD -DLINUX_PRG")
set(CMAKE_C_FLAGS "${GNU_FLAGS}")

ADD_EXECUTABLE(linux_star ${DIRSRCS} ./src/main_test.c)
set(EXECUTABLE_OUTPUT_PATH ../bin/)

TARGET_LINK_LIBRARIES(
        linux_star ${catkin_LIBRARIES}  curl ssl crypto m dl rt pthread event
)
