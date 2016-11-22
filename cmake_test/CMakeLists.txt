#define project name
PROJECT(my_proj)

#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#add a include dir
ADD_SUBDIRECTORY(my_dir)

#get all files name and put into DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

# $DIR_SRCS file create a execute file, name as main
ADD_EXECUTABLE(main ${DIR_SRCS})

# execute file "main" will link a lib "Test"
TARGET_LINK_LIBRARIES(main Test)
