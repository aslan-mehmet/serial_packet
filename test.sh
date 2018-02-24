#!/bin/bash

cp serial_packet.c serial_packet.h fifo.c fifo.h ./test/encode
cp serial_packet.c serial_packet.h fifo.c fifo.h ./test/decode

cd ./test/encode
gcc *.c
str=$(./a.out)

if [ $? == 0 ]
then
	echo encode test pass
else
	echo encode test fail
fi

cd ../decode
gcc *.c
./a.out ${str}

if [ $? == 0 ]
then
	echo decode test pass
else
	echo decode test fail
fi


cd ../encode_all_var_types
gcc *.c
str=$(./a.out)

if [ $? == 0 ]
then
	echo encode_all_var_types test pass
else
	echo encode_all_var_types test fail
fi

cd ../decode_all_var_types
gcc *.c
./a.out ${str}

if [ $? == 0 ]
then
	echo decode_all_var_types test pass
else
	echo decode_all_var_types test fail
fi


cd ../..
echo done
