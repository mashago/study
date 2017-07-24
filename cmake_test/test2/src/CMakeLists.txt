
#AUX_SOURCE_DIRECTORY(<dir> <variable>)
#put all files name from dir into variable
AUX_SOURCE_DIRECTORY(. DIR_LIB_SRCS)

#create a dynamic lib name "Test"
ADD_LIBRARY(Test STATIC ${DIR_LIB_SRCS})

