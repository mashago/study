#1.define project and target
#2.define make flags
#3.define include and lib dir
#4.check os to set include or lib dir
#5.group header and sourse
#6.add sub dir if necessary
#7.add executable or library
#8.target link libraries if necessary
#9.set output path

#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#1.define project and target
SET(PROJECT_NAME test2)
SET(TARGET_NAME main)
PROJECT(${PROJECT_NAME})
SET(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

#2.define make flags
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CONFIGURATION_TYPES "Debug")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS} -std=c++11")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
ENDIF()
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

#3.define include and lib dir
#add include file path
INCLUDE_DIRECTORIES(
./
./src
)

#add lib path
LINK_DIRECTORIES(
)

#4.check os to set include or lib dir
MESSAGE(STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
MESSAGE(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
ELSE()
ENDIF()

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
ADD_SUBDIRECTORY(src)

#7.add executable or library
ADD_EXECUTABLE(${TARGET_NAME} ${DIR_HEADERS} ${DIR_SRCS})

#8.target link libraries if necessary
TARGET_LINK_LIBRARIES(${TARGET_NAME} mylib)

#9.set output path
IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_PATH}/bin")
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_PATH}/bin")
ENDIF()