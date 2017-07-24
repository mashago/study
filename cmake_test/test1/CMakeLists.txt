#define project name
PROJECT(test1)

#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#AUX_SOURCE_DIRECTORY(<dir> <variable>)
#put all files name from dir into variable
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#$DIR_SRCS file create a execute file, name as main
ADD_EXECUTABLE(main ${DIR_SRCS})

