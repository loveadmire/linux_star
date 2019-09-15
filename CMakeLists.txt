cmake_minimum_required(VERSION 3.10.2)
project(linux_star C)


#add src file name to value DIRSRCS   实现文件目录需要逐个添加
AUX_SOURCE_DIRECTORY(./src   DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/json   DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/common   DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/websocket_linux  DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/gmssl  DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/linux_net  DIRSRCS)
AUX_SOURCE_DIRECTORY(./src/datastruct  DIRSRCS)




#添加头文件目录
INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} ./inc  ./inc/linux_net ./inc/json   )
INCLUDE_DIRECTORIES( ${catkin_INCLUDE_DIRS} ./inc  ./inc/datastruct ./inc/common ./inc/gmssl)


#添加库目录
LINK_DIRECTORIES(${catkin_LIB_DIRS} ./lib/thirdPartLib )

#添加系统库目录
FIND_LIBRARY(RUNTIME_LIB rt /usr/lib /usr/local/lib NO_DEFAULT_PATH)

set(CMAKE_C_STANDARD 99)

#
set(GNU_FLAGS "-m64 -static -fPIC -Wall -O2 -MMD -DLINUX_PRG")
set(CMAKE_C_FLAGS "${GNU_FLAGS}")

ADD_EXECUTABLE(linux_star ${DIRSRCS} src/gmssl/gmssl_mast_secruty.c)
set(EXECUTABLE_OUTPUT_PATH ../bin/)

TARGET_LINK_LIBRARIES(
        linux_star ${catkin_LIBRARIES}  curl ssl crypto m dl rt pthread event
)
