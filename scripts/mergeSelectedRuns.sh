#!/bin/bash
# Usage:
#	source mergeSelectedRuns.sh <Targe Filename> <Target Filename Replace String> <Data Directory> <Comman Separated List of Runs>
#
# Note dashes (e.g. "-") between numbers are accepted
#
# Example:
#	source mergeSelectedRuns.sh GE11-VII-S-CERN-0002_Summary_Physics_RandTrig_XRay40kV99uA_580uA_YYkEvt_Ana.root YY $DATA_QC5/GE11-VII-S-CERN-0002 134,135,137-153,154-158
#
#

# Store the original directory
DIR_ORIG=$PWD

# Store Input
FILE_OUT=$1
FILE_OUT_REPLACE_STR=$2
DIR_SRC=$3
LIST_RUNS=$4

if [ -d $DIR_SRC ]; then
	cd $DIR_SRC
else
	echo "Directory " $DIR_SRC " not found"
	echo "Exitting!!!"
fi

# Determine which files should be merged
LIST_FILES=""
for run in $(echo $LIST_RUNS | sed "s/,/ /g")
do
	# Check run ranges of the form X-Y and store [X,Y]
	RANGE_FIRST=$((-1))
	RANGE_LAST=$((-1))
	for substr in $(echo $run | tr "-" "\n")
	do
		if [ ${RANGE_FIRST} -lt 0 ]; then
			RANGE_FIRST=$substr
		elif [ $RANGE_LAST -lt 0 ]; then
			RANGE_LAST=$substr
		else

		fi
	done

	#Add file matching found run number to LIST_FILES
	if [ ${RANGE_LAST} -gt 0 ]; then
		for i in {$RANGE_FIRST..$RANGE_LAST}
		do
			if [ $i -gt 999 ]; then
				LIST_FILES="$LIST_FILES $(ls *Run$i*Ana.root)"
			elif [ $i -gt 99 ]; then
				LIST_FILES="$LIST_FILES $(ls *Run0$i*Ana.root)"
			elif [ $i -gt 9 ]; then
				LIST_FILES="$LIST_FILES $(ls *Run00$i*Ana.root)"
			else
				LIST_FILES="$LIST_FILES $(ls *Run000$i*Ana.root)"
			fi
		done
	else
		if [ $RANGE_FIRST -gt 999 ]; then
			LIST_FILES="$LIST_FILES $(ls *Run$RANGE_FIRST*Ana.root)"
		elif [ $RANGE_FIRST -gt 99 ]; then
			LIST_FILES="$LIST_FILES $(ls *Run0$RANGE_FIRST*Ana.root)"
		elif [ $RANGE_FIRST -gt 9 ]; then
			LIST_FILES="$LIST_FILES $(ls *Run00$RANGE_FIRST*Ana.root)"
		else
			LIST_FILES="$LIST_FILES $(ls *Run000$RANGE_FIRST*Ana.root)"
		fi

	fi
done

# Determine the number of Triggers Spanning LIST_FILES
N_EVT=0
FIELD_EVT="kEvt"
for f in $(echo $LIST_FILES | tr " " "\n")
do
	echo $f

        for substr in $(echo $f | tr "_" "\n")
	do
                if echo $substr | grep -q $FIELD_EVT
		then
			N_EVT=$(( N_EVT + $(echo $substr | sed 's/[^0-9]*//g') ))
			break
		fi
	done
done



# Put proper event number in output file
FILE_OUT=$(echo $FILE_OUT | sed "s@$FILE_OUT_REPLACE_STR@$N_EVT@g")

echo $FILE_OUT

# Merge Files
echo "Setup ready"
echo "Type:"
echo ""
echo ""

echo "hadd -k $FILE_OUT $LIST_FILES"
#hadd -k $FILE_OUT $LIST_FILES

echo ""
echo ""

# Return to Original Directory
#cd $DIR_ORIG
