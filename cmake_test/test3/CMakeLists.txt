#1.define project and target
#2.define make flags
#3.define include and lib dir
#4.check os to set include or lib dir
#5.add definitions
#6.group header and sourse
#7.add sub dir if necessary
#8.add executable or library
#9.target link libraries if necessary

#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#1.define project and target
SET(PROJECT_NAME test3)
SET(TARGET_NAME main)
PROJECT(${PROJECT_NAME})

#2.define make flags
SET(CMAKE_BUILD_TYPE "Debug")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS} -std=c++11")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
ENDIF()
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

#3.define include and lib dir
#add include file path
INCLUDE_DIRECTORIES(
./
./core
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
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Cygwin")
ELSE()
ENDIF()

#6.add definitions

#7.group header and sourse
#search and put all match files name into variable
#FILE(GLOB|GLOB_RECURSE <variable> *.h *.hpp)
#GLOB|GLOB_RECURSE
#FILE(GLOB DIR_HEADERS *.h *.hpp)

#AUX_SOURCE_DIRECTORY(<dir> <variable>)
#put all files name from dir into variable
#AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#put files into folder in vs
#SOURCE_GROUP("Include" FILES ${DIR_HEADERS})
#SOURCE_GROUP("Source" FILES ${DIR_SRCS})

#8.add sub dir if necessary
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(main)

#9.add executable or library
#ADD_EXECUTABLE(${TARGET_NAME} ${DIR_HEADERS} ${DIR_SRCS})

#8.target link libraries if necessary
#TARGET_LINK_LIBRARIES(${TARGET_NAME} mylib)

