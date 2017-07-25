#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(PROJECT_NAME timer_test)
SET(TARGET_NAME timer_test)

#define project name
PROJECT(${PROJECT_NAME})

IF (WIN32)  
	MESSAGE(STATUS "OS: WIN32")  
ELSEIF (APPLE)  
	MESSAGE(STATUS "OS: APPLE")  
ELSEIF (UNIX)  
	MESSAGE(STATUS "OS: UNIX")  
ENDIF ()  

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

#add include path
INCLUDE_DIRECTORIES(
./
/usr/local/include
)

#add lib path
LINK_DIRECTORIES(
/usr/local/lib
)

#add lib
#LINK_LIBRARIES(event)

#$DIR_SRCS file create a execute file, name as main
ADD_EXECUTABLE(${TARGET_NAME} ${DIR_HEADERS} ${DIR_SRCS})

TARGET_LINK_LIBRARIES(${TARGET_NAME} event)
