#1.define project and target
#2.define make flags
#3.define include and lib dir
#4.check os to set include or lib dir
#5.add definitions
#6.group header and sourse
#7.add sub dir if necessary
#8.add executable or library
#9.target link libraries if necessary
#10.set target output dir

#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#1.define project and target
SET(PROJECT_NAME lua_study)
SET(TARGET_STU stu)
SET(TARGET_LUA_INTERPRETER lua_interpreter)
PROJECT(${PROJECT_NAME})

#2.define make flags
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CONFIGURATION_TYPES "Debug")
MESSAGE(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")
IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
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


#5.add definitions
ADD_DEFINITIONS(-DLUA_COMPAT_ALL -D__LUA_5_2)

#6.group header and sourse
SET(FILES_STU "stu.c")
SET(FILES_LUA_INTERPRETER "lua_interpreter.c")

#7.add sub dir if necessary

#8.add executable or library
ADD_EXECUTABLE(${TARGET_STU} ${FILES_STU})
ADD_EXECUTABLE(${TARGET_LUA_INTERPRETER} ${FILES_LUA_INTERPRETER})

#9.target link libraries if necessary
TARGET_LINK_LIBRARIES(${TARGET_STU} ${LIBRARIES})
TARGET_LINK_LIBRARIES(${TARGET_LUA_INTERPRETER} ${LIBRARIES})

#SET_TARGET_PROPERTIES(${TARGET_STU} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)
SET_TARGET_PROPERTIES(${TARGET_STU} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ../)
SET_TARGET_PROPERTIES(${TARGET_LUA_INTERPRETER} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ../)
