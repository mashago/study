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
SET(PROJECT_NAME sc1)
SET(TARGET_SERVER server_test)
SET(TARGET_CLIENT client_test)
PROJECT(${PROJECT_NAME})

#2.define make flags
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS} -std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
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

SET(LIBRARIES "event")

#4.check os to set include or lib dir
MESSAGE(STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
MESSAGE(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Cygwin")
	INCLUDE_DIRECTORIES(/usr/include)
	LINK_DIRECTORIES(/usr/lib)
ELSE ()
ENDIF ()

#5.group header and sourse
SET(FILES_SERVER "server_test.cpp")
SET(FILES_CLIENT "client_test.cpp")

#6.add sub dir if necessary

#7.add executable or library
ADD_EXECUTABLE(${TARGET_SERVER} ${FILES_SERVER})
ADD_EXECUTABLE(${TARGET_CLIENT} ${FILES_CLIENT})

#8.target link libraries if necessary
TARGET_LINK_LIBRARIES(${TARGET_SERVER} ${LIBRARIES})
TARGET_LINK_LIBRARIES(${TARGET_CLIENT} ${LIBRARIES})
