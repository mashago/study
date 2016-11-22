#!/bin/bash

CMD1=`cat << DELIM
ls -la
DELIM`

echo "$CMD1" 

CMD2=`
ls -la
echo hhhh
`

echo "$CMD2" 


echo "---- for test ----"
LIST="aaa bbb ccc"

for file in $LIST
do
	echo file=$file
done

F1=0
for ((i=1; i<10; i=i+1))
do
	F1=$(($F1+$i))
done
echo "F1=$F1"

# num
echo "---- num test ----"
N1=1
N2=5
N1=$((1+2))
echo "N1_1=$N1"

N1=$(($N1+$N2))
echo "N1_2=$N1"

N3=$N1+$N2
echo "N3_1=$N3"
N3=$(($N3))
echo "N3_2=$N3"

# sed
echo "---- sed test ----"
T1="test.txt"
T2=`echo $T1 | sed 's/\.txt//g'`
echo T1=$T1 T2=$T2

echo "aaa bbb ccc" > sed_text.txt
echo "origin sed_text.txt"
cat sed_text.txt
sed -i 's/aaa/xxx/g' sed_text.txt
echo "after sed sed_text.txt"
cat sed_text.txt

# awk
echo "---- awk test ----"
echo `ls -l | awk '{print $1 "\t" $2}'`

# if
echo "---- if test ----"
P1=123
if [ "$P1" == "123" ]; then
	echo "P1==123"
else
	echo "P1!=123"
fi

P2=10
echo "P2=$P2"
if [ $P2 -eq 10 ]; then
	echo "P2 == 10"
else
	echo "P2 != 10"
fi

if [ $P2 -lt 10 ]; then
	echo "P2 < 10"
else
	echo "P2 >= 10"
fi

# case
echo "---- case test ----"
P1=456
case $P1 in
	"123")
		echo "P1==123"
		;;
	"456")
		echo "P1==456"
		;;
	*)
		echo "P1==other thing"
		;;
esac

# function
echo "---- function test ----"
function print_hello()
{
	echo "call print_hello function"
}
# call function
print_hello;

# loop
echo "---- loop test ----"
# declare variable is integer
declare -i L1=0
#L1=0

echo "L1=$L1"
while [ $L1 -lt 10 ]
do
	#L1=`echo $L1+1| bc`
	L1=$L1+1
done
echo "L1=$L1"

until [ $L1 -eq 0 ]
do
	L1=$L1-1
done
echo "L1=$L1"


