#1.define project and target
#2.define make flags
#3.define include and lib dir
#4.check os to set include or lib dir
#5.group header and sourse
#6.add sub dir if necessary
#7.add executable or library
#8.target link libraries if necessary

#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#1.define project and target
SET(PROJECT_NAME test1)
SET(TARGET_NAME main)
PROJECT(${PROJECT_NAME})

#2.define make flags
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

MESSAGE(STATUS "CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE})
MESSAGE(STATUS "CMAKE_CXX_FLAGS=" ${CMAKE_CXX_FLAGS})
MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG=" ${CMAKE_CXX_FLAGS_DEBUG})
MESSAGE(STATUS "CMAKE_CXX_FLAGS_RELEASE=" ${CMAKE_CXX_FLAGS_RELEASE})

#3.define include and lib dir

#4.check os to set include or lib dir
IF (WIN32)
	MESSAGE(STATUS "******* OS: WIN32 *******")
ELSEIF (APPLE)
	MESSAGE(STATUS "******* OS: APPLE *******")
ELSEIF (UNIX)
	MESSAGE(STATUS "******* OS: UNIX *******")
ENDIF ()

MESSAGE(STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
MESSAGE(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Cygwin")
ELSE ()
ENDIF ()


#5.group header and sourse
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

#6.add sub dir if necessary

#7.add executable or library
ADD_EXECUTABLE(${TARGET_NAME} ${DIR_HEADERS} ${DIR_SRCS})

