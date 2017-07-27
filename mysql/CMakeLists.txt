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
SET(PROJECT_NAME mysql_study)
SET(TARGET_MYSQL_OPERATOR mysql_operator)
SET(TARGET_MYSQL_TEST mysql_test)
SET(TARGET_MYSQL_TEST2 mysql_test2)
SET(TARGET_TEST_LOCK test_lock)
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

SET(LIBRARIES "-lmysqlclient")

#4.check os to set include or lib dir
IF(CMAKE_SYSTEM_NAME MATCHES "Windows")
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/windows/mysql)
	LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib/windows)
	LIST(APPEND LIBRARIES "ws2_32")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
	INCLUDE_DIRECTORIES(/usr/local/include/mysql)
	LINK_DIRECTORIES(/usr/local/mysql/lib)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Cygwin")
	INCLUDE_DIRECTORIES(/usr/include)
	LINK_DIRECTORIES(/usr/lib)
ELSE()
ENDIF()

#5.group header and sourse
SET(FILES_MYSQL_OPERATOR mysql_operator.cpp mysql_operator.h)
SET(FILES_MYSQL_TEST "mysql_test.cpp")
SET(FILES_MYSQL_TEST2 "mysql_test2.cpp")
SET(FILES_TEST_LOCK "test_lock.cpp")

#6.add sub dir if necessary

#7.add executable or library
ADD_LIBRARY(${TARGET_MYSQL_OPERATOR} ${FILES_MYSQL_OPERATOR})
ADD_EXECUTABLE(${TARGET_MYSQL_TEST} ${FILES_MYSQL_TEST})
ADD_EXECUTABLE(${TARGET_MYSQL_TEST2} ${FILES_MYSQL_TEST2})
ADD_EXECUTABLE(${TARGET_TEST_LOCK} ${FILES_TEST_LOCK})

#8.target link libraries if necessary
TARGET_LINK_LIBRARIES(${TARGET_MYSQL_TEST} ${LIBRARIES})
TARGET_LINK_LIBRARIES(${TARGET_MYSQL_TEST2} ${LIBRARIES} ${TARGET_MYSQL_OPERATOR})
TARGET_LINK_LIBRARIES(${TARGET_TEST_LOCK} ${LIBRARIES} ${TARGET_MYSQL_OPERATOR})

