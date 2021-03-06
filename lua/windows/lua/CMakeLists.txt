
#1.define project and target
#2.define make flags
#3.group source
#4.create lib and exe

#define cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#1.define project and target
SET(PROJECT_NAME lua)
PROJECT(${PROJECT_NAME})
SET(LUA_LIB_NAME liblua)
SET(LUA_NAME lua)
SET(LUAC_NAME luac)

#2.define make flags
#SET(PROJECT_BUILD_TYPE "Debug")
SET(PROJECT_BUILD_TYPE "Release")
SET(CMAKE_BUILD_TYPE ${PROJECT_BUILD_TYPE})
SET(CMAKE_CONFIGURATION_TYPES ${PROJECT_BUILD_TYPE})

#3.group source
AUX_SOURCE_DIRECTORY(src LIB_SOURCE)
LIST(REMOVE_ITEM LIB_SOURCE "src/lua.c" "src/luac.c")
SET(LUA_SOURCE "src/lua.c")
SET(LUAC_SOURCE "${LIB_SOURCE};src/luac.c")

#4.create lib and exe
# build lib
#SET(LIB_BUILD_TYPE "STATIC")
SET(LIB_BUILD_TYPE "SHARED")
IF(LIB_BUILD_TYPE MATCHES "STATIC")
	ADD_LIBRARY(${LUA_LIB_NAME} STATIC ${LIB_SOURCE})
ELSEIF(LIB_BUILD_TYPE MATCHES "SHARED")
	ADD_DEFINITIONS("-DLUA_BUILD_AS_DLL")
	ADD_LIBRARY(${LUA_LIB_NAME} SHARED ${LIB_SOURCE})
ENDIF()

# build lua
ADD_EXECUTABLE(${LUA_NAME} ${LUA_SOURCE})
TARGET_LINK_LIBRARIES(${LUA_NAME} ${LUA_LIB_NAME})

# build luac
ADD_EXECUTABLE(${LUAC_NAME} ${LUAC_SOURCE})

# .lib is ARCHIVE, .dll is RUNTIME
# DEBUG
SET_TARGET_PROPERTIES(${LUA_LIB_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin")
SET_TARGET_PROPERTIES(${LUA_LIB_NAME} ${LUA_NAME} ${LUAC_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin")
# RELEASE
SET_TARGET_PROPERTIES(${LUA_LIB_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin")
SET_TARGET_PROPERTIES(${LUA_LIB_NAME} ${LUA_NAME} ${LUAC_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin")

