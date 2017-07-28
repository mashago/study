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
SET(PROJECT_NAME lua_study)
SET(TARGET_STU stu)
SET(TARGET_LUA_INTERPRETER lua_interpreter)
PROJECT(${PROJECT_NAME})

#2.define make flags
SET(CMAKE_BUILD_TYPE "Debug")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} ${CMAKE_CXX_FLAGS} -std=c99")
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

SET(LIBRARIES "-llua -ldl -lm")

#4.check os to set include or lib dir
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/windows/lua)
	LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib/windows)
	LIST(APPEND LIBRARIES "ws2_32")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "CYGWIN")
	INCLUDE_DIRECTORIES(/usr/include)
	LINK_DIRECTORIES(/usr/lib)
ELSE()
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

