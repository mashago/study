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
SET(PROJECT_NAME lua_study)
SET(TARGET_STU stu)
SET(TARGET_LUA_INTERPRETER lua_interpreter)
PROJECT(${PROJECT_NAME})
SET(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

#2.define compile flags
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CONFIGURATION_TYPES "Debug")
#SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_CONFIGURATION_TYPES "Release")
IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
ENDIF()
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

#3.define include and lib
INCLUDE_DIRECTORIES(
./
/usr/local/include
)

LINK_DIRECTORIES(
/usr/local/lib
)

IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/windows/lua)
	LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib/windows)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
	INCLUDE_DIRECTORIES(/usr/include)
	LINK_DIRECTORIES(/usr/lib)
ELSE()
ENDIF()

IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	SET(LIBRARIES "lua;dl;m")
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	SET(LIBRARIES "ws2_32;liblua")
ENDIF()

#4.add definitions
ADD_DEFINITIONS(-DLUA_COMPAT_ALL -D__LUA_5_2)

#5.group header and sourse
SET(FILES_STU "stu.c")
SET(FILES_LUA_INTERPRETER "lua_interpreter.c")

#6.add sub dir if necessary

#7.add executable or library
ADD_EXECUTABLE(${TARGET_STU} ${FILES_STU})
ADD_EXECUTABLE(${TARGET_LUA_INTERPRETER} ${FILES_LUA_INTERPRETER})

#8.target link libraries if necessary
TARGET_LINK_LIBRARIES(${TARGET_STU} ${LIBRARIES})
TARGET_LINK_LIBRARIES(${TARGET_LUA_INTERPRETER} ${LIBRARIES})

#9.set output path
SET_TARGET_PROPERTIES(${TARGET_STU} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_PATH}")
SET_TARGET_PROPERTIES(${TARGET_LUA_INTERPRETER} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_PATH}")
SET_TARGET_PROPERTIES(${TARGET_STU} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_PATH}")
SET_TARGET_PROPERTIES(${TARGET_LUA_INTERPRETER} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_PATH}")

