#!bin/bash

rm pipe.txt
rm ./a.out

gcc gauss_elim_pipe.c -fopenmp
for n in 32 64 128 256 512 1024 2048 4096
do
	for p in 1 2 4 8 16 32
	do
		echo -n "$n $p " >> pipe.txt
		for c in {1..5..1}
		do
			echo -n "$(./a.out $n $p) " >> pipe.txt 
		done
		echo " " >> pipe.txt
	done
done
