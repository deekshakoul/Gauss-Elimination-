#!bin/bash/
rm own.txt
gcc ser.c -lm -fopenmp

	for N in 16 32 64 128 256 512 1024 2048 4096 8192
			do	#echo "N is $N"
				c=0
				sum=0 
				  while [ $c -lt 41 ]
				do 
				count=$(./a.out $p $N )
			
				c=`expr $c + 1`
				
				sum=`echo $sum + $count | bc`
				#echo "sum is $sum"
				
				done
			#echo "sum is $sum"
			avg=$(echo "$sum/$c" | bc -l)
			#echo $avg
			echo "$avg">> "own.txt"
		done
