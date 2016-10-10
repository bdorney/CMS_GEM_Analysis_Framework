#!/bin/bash
# Usage:
#	source renameRuns.sh <Data Directory> <New String>
#
# Note dashes (e.g. "-") between numbers are accepted
#
# Example:
#	source renameRuns.sh $DATA_QC5/GE11-VII-S-CERN-0002 TimeCorr_DPGGeo
#
#

# Store the original directory
DIR_ORIG=$PWD

# Store Input
DIR_SRC=$1
FIELD=$2

# Check if directory exists
if [ -d $DIR_SRC ]; then
	cd $DIR_SRC
else
	echo "Directory " $DIR_SRC " not found"
	echo "Exitting!!!"
	return
fi

# Rename Files
for run in *kEvt_dataTree.root
do	
	NAME=$(echo $run | sed "s/dataTree.root/$FIELD.root/g")
	NAME=$(echo $NAME | sed "s/.root/_dataTree.root/g")
	#echo $NAME

	mv $run $NAME
done

#echo "ls -lh $DIR_SRC"
#ls -lh $DIR_SRC

# Return to Original Directory
cd $DIR_ORIG
