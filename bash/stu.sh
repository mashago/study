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

	local P2="abc"
	if [[ $P2 == "abc" || $P2 == "qwe" ]]; then
		echo "P2 == \"abc\""
	else
		echo "P2 != \"abc\""
	fi

	local file_name="no_such_file.txt"
	if [ ! -f $file_name ]; then
		echo "not file $file_name"
	fi

	if [ ! -e $file_name ]; then
		echo "file not exists $file_name"
	fi

	local empty_str=
	if [ -z $empty_str ]; then
		echo "str empty"
	fi

	return 0
}

function func1()
{
	cat > ttt.sh 2>&1 < stu.sh 
	cat /dev/null > ttt.sh
	ls not_exists_file.txt stu.sh 2> /dev/null

	return 0
}

function func2()
{
	local -i T1=0
	local sum=0
	while [ $T1 -lt 10 ]
	do
		T1=$T1+1
		sum=`expr $T1 + $sum`
	done
	echo "T1=$T1 sum=$sum"
	return 0
}

function func3()
{
	local LIST="aaa bbb ccc"

	for file in $LIST
	do
		echo file=$file
	done
	echo

	for file in *
	do
		if [ -f $file ]; then
			echo "$file is a file"
		elif [ -d $file ]; then
			echo "$file is a dir"
		else
			echo "$file is not a file and not a dir"
		fi
	done
	echo

	local F1=0
	for ((i=1; i<10; i=i+1))
	do
		F1=$(($F1+$i))
	done
	echo "F1=$F1"

	return 0
}

function func4()
{
	local -i P1=456
	case $P1 in
		123)
			echo "P1==123"
			;;
		456)
			echo "P1==456"
			;;
		*)
			echo "P1==other thing"
			;;
	esac

	local P2="abc"
	case $P2 in
		"qwe")
			echo "P2==qwe"
			;;
		"abc")
			echo "P2==abc"
			;;
		*)
			echo "P2==other thing"
			;;
	esac

	return 0
}

function func5()
{
	# sed
	local file_name="edit_test.txt"
	echo "aaa bbb ccc" > $file_name
	sed -i 's/aaa/xxx/g' $file_name
	cat $file_name
	rm $file_name
	echo

	# sed trim
	echo "  aaa bbb ccc  " > $file_name
	sed -i -e 's/^[[:space:]]\+//' -e 's/[[:space:]]\+$//' $file_name
	cat $file_name | awk '{print "["$0"]"}'
	rm $file_name

	echo

	# awk
	ls -l | awk '{print "["$9"]" "\t" $1}'
	echo
	awk -F":" '{print $1}' /etc/passwd 2>/dev/null
	echo
	awk -F"|" 'NR==FNR{uid2name[$1]=$2; name2income[$2]=0} NR>FNR{name2income[uid2name[$1]] += $2} END{for (name in name2income) {printf "%s\t%d\n", name, name2income[name]}}' role_info.txt role_income.txt
	return 0
}

function func6()
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

function func7()
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
FUNC_LIST[4]=func4
FUNC_LIST[5]=func5
FUNC_LIST[6]=func6
FUNC_LIST[7]=func7

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

exit
