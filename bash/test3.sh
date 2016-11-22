#!/bin/bash

# read file by line test

FILE_NAME=file.txt

# 1. while, file as stdin
echo -e "---- while 1 ----"
while read LINE
do
	echo $LINE
done < $FILE_NAME
echo -e "\n"


# 2. while, use pipo as input
echo -e "---- while 2 ----"
cat ${FILE_NAME} | while read LINE
do
	echo $LINE
done
echo -e "\n"

# 3. for
echo -e "---- for ----"
for LINE in `cat ${FILE_NAME}`
do
	echo $LINE
done
echo -e "\n"
