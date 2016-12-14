#!bin/bash/
rm own.txt
gcc linearP.c -lm -fopenmp
#c=0
#sum=0
count=0

for bs in 4 64 128
	do			for p in 2 4 8 16 32   
					do
						
						echo "p is : $p">>"own.txt"
						for N in 16 32 64 128 256 512 1024 4096 8192	
							do	#echo "N is $N"
								c=0
								sum=0 
								while [ $c -lt 11 ]
								do 
								count=$(./a.out $p $N $bs)
							
								c=`expr $c + 1`
								
								sum=`echo $sum + $count | bc`
								#echo "sum is $sum"
								
								done
							#echo "sum is $sum"
							avg=$(echo "$sum/$c" | bc -l)
							#echo $avg
							echo "$avg">> "own.txt"
						done
				done  
	done
