#!/bin/sh

CENTRALITY=bb3m6

EVENT_DIR=/home/mgalazyn/workspace/tpi_output/bb3m6


# parsing command line arguments
if [ "$1" != "" ]; then
	EVENT_DIR=$1
fi

if [ "$2" != "" ]; then
	CENTRALITY=$2
fi

# preparing space for new data
# rm -f data/$CENTRALITY/kk*.out
rm -f data/$CENTRALITY/pipi*.out
# rm -f data/$CENTRALITY/pp*.out
rm -f log/*.log

echo -e "\nRunning for centrality $CENTRALITY\n"

if [ ! -d data/$CENTRALITY ] ; then
	mkdir data/$CENTRALITY
fi

if [ ! -d log ] ; then
  mkdir log
fi


IFS_BAK=$IFS
IFS="
"

# # Kaons
#  find $EVENT_DIR -name "outfilekkcf*" -type f | sort | ./ktDecoder 1> data/$CENTRALITY/filelist.kk.in
#  FILES=`cat data/$CENTRALITY/filelist.kk.in`
#  for parameter in $FILES
#  do
#  	IFS=$IFS_BAK
#  	PARAMETERS_ARR=( $parameter );
#  	./fitsh $parameter data/$CENTRALITY/kk &>> log/fitsh.$CENTRALITY.kk.${PARAMETERS_ARR[1]}.log &
#  	./fit1d $parameter data/$CENTRALITY/kk &>> log/fit1d.$CENTRALITY.kk.${PARAMETERS_ARR[1]}.log &
#  	IFS="
#  "
#  done

# Pions
find $EVENT_DIR -name "outfilecf*" -type f | sort | ./ktDecoder 1> data/$CENTRALITY/filelist.pipi.in
FILES=`cat data/$CENTRALITY/filelist.pipi.in`
for parameter in $FILES
do
	IFS=$IFS_BAK
	PARAMETERS_ARR=( $parameter );
	./fitsh $parameter data/$CENTRALITY/pipi &>> log/fitsh.$CENTRALITY.pipi.${PARAMETERS_ARR[1]}.log &
	./fit1d $parameter data/$CENTRALITY/pipi &>> log/fit1d.$CENTRALITY.pipi.${PARAMETERS_ARR[1]}.log &
	IFS="
"
done

# # Protons
# find $EVENT_DIR -name "outfileppcf*" -type f | sort | ./ktDecoder 1> data/$CENTRALITY/filelist.pp.in
# FILES=`cat data/$CENTRALITY/filelist.pp.in`
# for parameter in $FILES
# do
# 	IFS=$IFS_BAK
# 	PARAMETERS_ARR=( $parameter );
# 	./fitsh $parameter data/$CENTRALITY/pp &>> log/fitsh.$CENTRALITY.pp.${PARAMETERS_ARR[1]}.log &
# 	./fit1d $parameter data/$CENTRALITY/pp &>> log/fit1d.$CENTRALITY.pp.${PARAMETERS_ARR[1]}.log &
# 	IFS="
# "
# done



IFS=$IFS_BAK
IFS_BAK=

echo "Waiting for all fitting processes..."
while [ `ps -U $USER -u $USER u | grep fit | wc -l` != 1 ]; do
	sleep 0.5
done
#echo "Plotting..."
#make plots
echo -e "\n\n[ DONE ]"
