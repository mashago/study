#!/bin/bash

function func0()
{
	echo "---- if test ----"
	local P1=123
	if [ $P1 -eq 123 ]; then
		echo "P1 -eq"
	elif [ $P1 -gt 123 ]; then
		echo "P1 -gt"
	else
		echo "P1 -ne"
	fi

	local P2="10"
	if [ $P2 == "10" ]; then
		echo "P2 == \"10\""
	else
		echo "P2 != \"10\""
	fi

	return 0
}

function func1()
{
	echo "---- for test ----"
	local LIST="aaa bbb ccc"

	for file in $LIST
	do
		echo file=$file
	done

	local F1=0
	for ((i=1; i<10; i=i+1))
	do
		F1=$(($F1+$i))
	done
	echo "F1=$F1"

	return 0
}

function func2()
{
	local T1="abc"
	local T2="T1"

	eval echo '1' '${'${T2}'}'
	echo '2' '${'${T2}'}'
	echo '3' '${'"${T2}"'}'
	echo '4' '${"T1"}'
	#echo 'a'${"T1"}'b'
	echo '5' 'a'${T1}'b'
	echo '6' '${'"T1"'}'
	echo '7' '${'T1'}'
	echo '8' '${'$T1'}'
	echo '9' '${'aaa"T1"bbbb'}'
	echo '10' '${'$T1'}'
	echo "11 1"aaaa"2"
	# echo "12 ${'"${T2}"'}"

	return 0
}

function func3()
{
	declare -A L1
	L1=([KEY1]="a1" [KEY2]="a2" [KEY3]="a3")
	L2="L1"
	KEY1=
	KEY2=
	KEY3=

	COUNT=`eval echo '${#'$L2'[*]}'`
	echo "COUNT=${COUNT}"
	for key in `eval echo '${!'$L2'[*]}'`
	do
		if [ -z `eval echo '${'$key'}'` ]; then
				#echo "key=$key"
				#echo `eval echo '${'$key'}'`
				#value=`eval echo '${'$L2'['${key}']}'`
				#echo "value=$value"

				#eval "$key"=`eval echo '${'$L2'['"${key}"']}'`
				eval $key=`eval echo '${'$L2'['${key}']}'`
		fi
	done

	echo "KEY1=${KEY1}"
	echo "KEY2=${KEY2}"
	echo "KEY3=${KEY3}"

	return 0
}

declare -A FUNC_LIST
FUNC_LIST[0]=func0
FUNC_LIST[1]=func1
FUNC_LIST[2]=func2
FUNC_LIST[3]=func3

function main()
{
	# run all test
	if [[ $# -ge 1 && $1 = "all" ]]; then
		echo "all"
		for i in ${!FUNC_LIST[*]}
		do
			echo "call test[$i]"
			${FUNC_LIST[$i]}
			if [ $? -ne 0 ]; then
				echo "test[$i] ERROR"
				return 0
			fi
			echo ""
		done
		return 0
	fi

	# run target or last test
	local index=`expr ${#FUNC_LIST[*]} - 1`
	if [ $# -ne 0 ]; then
		local i=`expr $1`
		if [[ ${i} -ge 0 && ${i} -le $index ]]; then
			index=${i}
		fi
	fi
	echo "index=${index}"

	${FUNC_LIST[${index}]}


}

main $@

