#!/bin/bash
# Usage:
#	source scpSelectedRawFiles.sh <Data Directory> <Comman Separated List of Runs> <Remote Location>
#
# Note dashes (e.g. "-") between numbers are accepted
#
# Example:
#	source scpSelectedRawFiles.sh $DATA_QC5/GE11-VII-S-CERN-0002 134,135,137-153,154-158 myusername@lxplus.cern.ch:/path
#
#

# Store the original directory
DIR_ORIG=$PWD

# Store Input
DIR_SRC=$1
LIST_RUNS=$2
DIR_REMOTE=$3

if [ -d $DIR_SRC ]; then
	cd $DIR_SRC
else
	echo "Directory " $DIR_SRC " not found"
	echo "Exitting!!!"
	return
fi

# Determine which files should be delete
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
				LIST_FILES="$LIST_FILES $(ls *Run$i*kEvt.raw)"
			elif [ $i -gt 99 ]; then
				#LIST_FILES="$LIST_FILES $(ls *Run0$i*kEvt.raw)"
				LIST_FILES="$LIST_FILES $(ls *Run$i*kEvt.raw)"
			elif [ $i -gt 9 ]; then
				#LIST_FILES="$LIST_FILES $(ls *Run00$i*kEvt.raw)"
				LIST_FILES="$LIST_FILES $(ls *Run0$i*kEvt.raw)"
			else
				#LIST_FILES="$LIST_FILES $(ls *Run000$i*kEvt.raw)"
				LIST_FILES="$LIST_FILES $(ls *Run00$i*kEvt.raw)"
			fi
		done
	else
		if [ $RANGE_FIRST -gt 999 ]; then
			LIST_FILES="$LIST_FILES $(ls *Run$RANGE_FIRST*kEvt.raw)"
		elif [ $RANGE_FIRST -gt 99 ]; then
			#LIST_FILES="$LIST_FILES $(ls *Run0$RANGE_FIRST*kEvt.raw)"
			LIST_FILES="$LIST_FILES $(ls *Run$RANGE_FIRST*kEvt.raw)"
		elif [ $RANGE_FIRST -gt 9 ]; then
			#LIST_FILES="$LIST_FILES $(ls *Run00$RANGE_FIRST*kEvt.raw)"
			LIST_FILES="$LIST_FILES $(ls *Run0$RANGE_FIRST*kEvt.raw)"
		else
			#LIST_FILES="$LIST_FILES $(ls *Run000$RANGE_FIRST*kEvt.raw)"
			LIST_FILES="$LIST_FILES $(ls *Run00$RANGE_FIRST*kEvt.raw)"
		fi

	fi
done

# Show every member of LIST_FILES
for f in $(echo $LIST_FILES | tr " " "\n")
do
	echo $f
done

# Merge Files
echo "Setup ready"
echo "Type:"
echo ""
echo ""
echo "scp $LIST_FILES $DIR_REMOTE"
echo ""
echo ""

# Return to Original Directory
#cd $DIR_ORIG
