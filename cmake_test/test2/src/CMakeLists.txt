#AUX_SOURCE_DIRECTORY(<dir> <variable>)
#put all files name from dir into variable
AUX_SOURCE_DIRECTORY(. DIR_LIB_SRCS)

#create a lib name "MyLib"
ADD_LIBRARY(MyLib ${DIR_LIB_SRCS})

