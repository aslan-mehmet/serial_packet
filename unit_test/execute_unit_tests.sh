#!/bin/bash

SRC_FILES="../fifo.c ../serial_packet.c"

testSourceFiles="\
is_little_endian.c \
encode.c \
decode.c"

numOfCompilationFails=0
numOfTestFails=0
numOfTests=0

for i in $testSourceFiles
do
	(( numOfTests++ ))

	gcc -std=c99 $i $SRC_FILES
	
	if [ "$?" == "0" ]
	then
		echo "PASS: compilation of $i"
	else
		echo "FAIL: compilation of $i"
		(( numOfCompilationFails++ ))
		(( numOfTestFails++ ))
		continue
	fi
	
	./a.out
	
	if [ "$?" == "0" ]
	then
		echo "PASS: test result of $i"
	else
		echo "FAIL: test result of $i"
		(( numOfTestFails++ ))
	fi
		
done

echo "number of compilation fails $numOfCompilationFails/$numOfTests"
echo "number of test fails $numOfTestFails/$numOfTests"

if [ "$numOfCompilationFails" != 0 ]
then
	exit 1
fi

if [ "numOfTestFails" != 0 ]
then
	exit 2
fi
