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
SET(PROJECT_NAME timer_test)
SET(TARGET_NAME timer_test)
PROJECT(${PROJECT_NAME})

#2.define make flags
SET(CMAKE_BUILD_TYPE "Debug")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS} -std=c++11")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
ENDIF()
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

#3.define include and lib dir
#add include path
INCLUDE_DIRECTORIES(
./
/usr/local/include
)

#add lib path
LINK_DIRECTORIES(
/usr/local/lib
)

#4.check os to set include or lib dir
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../include/windows/libevent)
	LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../lib/windows)
	SET(LIBRARIES "ws2_32;libevent")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
	SET(LIBRARIES "event")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
	SET(LIBRARIES "event")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
	INCLUDE_DIRECTORIES(/usr/include)
	LINK_DIRECTORIES(/usr/lib)
	SET(LIBRARIES "event")
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

#7.add executable or library
ADD_EXECUTABLE(${TARGET_NAME} ${DIR_HEADERS} ${DIR_SRCS})

#8.target link libraries if necessary
TARGET_LINK_LIBRARIES(${TARGET_NAME} ${LIBRARIES})
