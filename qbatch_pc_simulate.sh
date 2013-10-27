#!/bin/bash          

HOMEDIR="/home/ndf/pc"

NN="8 9 10 11 12 13 14 15 16 17 18 19 20"
CH_TYPE="1"
CH_PAR="0.2 0.25 0.3 0.35 0.4 0.45"
KK="0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45 0.5"

N_SAMPLES=100
MAX_N_SAMPLES=-1

OUTDIR="outputs2/"

for N in $NN; do
	for ctype in $CH_TYPE; do
		for cpar in $CH_PAR; do
			for Kp in $KK; do

				K=`echo "$Kp" "$N" | awk '{printf("%d\n",$1 * (2 ** $2))}'`
				qsub $HOMEDIR/pc_simulate_launcher.sh $N $ctype $cpar $K $N_SAMPLES $MAX_N_SAMPLES "$HOMEDIR/$OUTDIR"

			done			
		done
	done
done


