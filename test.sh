#!/bin/bash          

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"

NN="8 9 10 11 12 13 14 15 16 17 18 19 20"
CH_TYPE="1"
CH_PAR="0.2 0.25 0.3 0.35 0.4 0.45"

N_SAMPLES=100
MAX_N_SAMPLES=-1

OUTDIR="outputs/"

echo "Press any key to exit... "

for N in $NN; do
	for ctype in $CH_TYPE; do
		for cpar in $CH_PAR; do
			
			./pc_construction $N $ctype $cpar $N_SAMPLES $MAX_N_SAMPLES $OUTDIR > /dev/null & 

			cat

			exit

		done
	done
done

read -p "Press any key to exit... " -n1 -s

killall pc_construction

