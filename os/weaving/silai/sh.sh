#!/bin/bash
i=$1
gcc -lpthread count.c -o a.out
while [ $i -gt 0 ]
do
#	echo $i
	let "i=i-1"
	./a.out
done
