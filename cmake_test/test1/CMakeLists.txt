#1.define project and target
#2.define compile flags
#3.define include and lib
#4.add definitions
#5.group header and sourse
#6.add sub dir if necessary
#7.add executable or library
#8.target link libraries if necessary
#9.set output path

#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#1.define project and target
SET(PROJECT_NAME test1)
SET(TARGET_NAME main)
PROJECT(${PROJECT_NAME})
SET(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

#2.define compile flags
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CONFIGURATION_TYPES "Debug")
IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS} -std=c++11")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
ENDIF()
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

MESSAGE(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
MESSAGE(STATUS "CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE})
MESSAGE(STATUS "CMAKE_CXX_FLAGS=" ${CMAKE_CXX_FLAGS})
MESSAGE(STATUS "CMAKE_CXX_FLAGS_DEBUG=" ${CMAKE_CXX_FLAGS_DEBUG})
MESSAGE(STATUS "CMAKE_CXX_FLAGS_RELEASE=" ${CMAKE_CXX_FLAGS_RELEASE})
MESSAGE(STATUS "CMAKE_CURRENT_SOURCE_DIR=" ${CMAKE_CURRENT_SOURCE_DIR})

#3.define include and lib
INCLUDE_DIRECTORIES(
./
/usr/local/include
)

LINK_DIRECTORIES(
/usr/local/lib
)

IF(WIN32)
ELSEIF(APPLE)
ELSEIF(UNIX)
ENDIF()

MESSAGE(STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
MESSAGE(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
ELSE ()
ENDIF()

IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
ENDIF()

#4.add definitions
#ADD_DEFINITIONS(-DDEBUG)

#5.group header and sourse
#search and put all match files name into variable
#FILE(GLOB|GLOB_RECURSE <variable> *.h *.hpp)
#GLOB|GLOB_RECURSE
FILE(GLOB FILES_HEAD *.h *.hpp)

#AUX_SOURCE_DIRECTORY(<dir> <variable>)
#put all files name from dir into variable
AUX_SOURCE_DIRECTORY(. FILES_SRC)

#put files into folder in vs
SOURCE_GROUP("Include" FILES ${FILES_HEAD})
SOURCE_GROUP("Source" FILES ${FILES_SRC})

#6.add sub dir if necessary
#ADD_SUBDIRECTORY(src)

#7.add executable or library
ADD_EXECUTABLE(${TARGET_NAME} ${FILES_HEAD} ${FILES_SRC})

#8.target link libraries if necessary
#TARGET_LINK_LIBRARIES(${TARGET_NAME} mylib)

#9.set output path
IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	#INSTALL(TARGETS ${TARGET_NAME} DESTINATION bin)
	SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_PATH}/bin")
	#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin) # not work ...
	#MESSAGE(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	# normally put cmake win32 project into ./sln/
	SET_TARGET_PROPERTIES(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_PATH}/bin")
ENDIF()

