#1.define target
#2.group source
#3.create lib

#1.define target
SET(TARGET_NAME mylib)

#2.group source
FILE(GLOB FILES_HEAD *.h *.hpp)
AUX_SOURCE_DIRECTORY(. FILES_SOURCE)
SOURCE_GROUP("Include" FILES ${FILES_HEAD})
SOURCE_GROUP("Source" FILES ${FILES_SOURCE})

#3.create lib
ADD_LIBRARY(${TARGET_NAME} ${FILES_HEAD} ${FILES_SOURCE})

