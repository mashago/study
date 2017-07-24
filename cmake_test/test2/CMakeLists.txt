#define project name
PROJECT(my_proj)

#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#add a include dir
ADD_SUBDIRECTORY(src)

#AUX_SOURCE_DIRECTORY(<dir> <variable>)
#put all files name from dir into variable
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#add include file path
INCLUDE_DIRECTORIES(
./
./src
)

#add lib path
LINK_DIRECTORIES()

#$DIR_SRCS file create a execute file, name as main
ADD_EXECUTABLE(main ${DIR_SRCS})

#execute file "main" will link with a lib "Test"
TARGET_LINK_LIBRARIES(main Test)
