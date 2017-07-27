#search and put all match files name into variable
#FILE(GLOB|GLOB_RECURSE <variable> *.h *.hpp)
#GLOB|GLOB_RECURSE
FILE(GLOB DIR_LIB_HEADERS *.h *.hpp)

#AUX_SOURCE_DIRECTORY(<dir> <variable>)
#put all source files name from dir into variable
AUX_SOURCE_DIRECTORY(. DIR_LIB_SRCS)

#put files into folder in vs
SOURCE_GROUP("Include" FILES ${DIR_LIB_HEADERS})
SOURCE_GROUP("Source" FILES ${DIR_LIB_SRCS})

#create a lib name "mylib"
ADD_LIBRARY(mylib ${DIR_LIB_HEADERS} ${DIR_LIB_SRCS})

