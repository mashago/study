#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#define project name
PROJECT(test2)

#search and put all match files name into variable
#FILE(GLOB|GLOB_RECURSE <variable> *.h *.hpp)
#GLOB|GLOB_RECURSE
FILE(GLOB DIR_HEADERS *.h *.hpp)

#AUX_SOURCE_DIRECTORY(<dir> <variable>)
#put all files name from dir into variable
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#put files into folder in vs
SOURCE_GROUP("Include" FILES ${DIR_HEADERS})
SOURCE_GROUP("Source" FILES ${DIR_SRCS})

#add a sub dir to build
ADD_SUBDIRECTORY(src)

#add include file path
INCLUDE_DIRECTORIES(
./
./src
)

#add lib path
LINK_DIRECTORIES(
)

#create a execute file, name as main
ADD_EXECUTABLE(main ${DIR_HEADERS} ${DIR_SRCS})

#execute file "main" will link with a lib "mylib"
TARGET_LINK_LIBRARIES(main mylib)
