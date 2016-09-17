#!bin/zsh
# Usage:
#	source runMode_Comparison.sh <Data File Directory> <Output Data Filename> <Obs Name> <iEta,iPhi,iSlice> <Identifier>
#
# NOTE: iEta must be an integer greater than 0
#	iPhi must be an integer greater than 0, or for only iEta case -1
#	iSlice must be an interger greater than 0, or for only the (iEta,iPhi) or iEta cases -1
#
# Example:
#	source runMode_Comparison.sh data/clustSelStudy/GE11-VII-L-CERN-0002 GE11-VII-L-CERN-0002_ClustSize_Comparison.root clustADC 4,2,-1 ClustSize
#

#store the original directory
DIR_ORIG=$PWD

#store the run config file
FILE_RUN=$GEM_BASE/config/configComp.cfg
FILE_RUN_TEMP=$GEM_BASE/config/configComp_Template.cfg
cp $FILE_RUN_TEMP $FILE_RUN

#setup input variables
DIR_DATA=$1
FILE_OUT=$2
OBS_NAME=$3
OBS_POS=$4
IDENT=$5

#Move to the data directory
cd $DIR_DATA

#Replace observable name
sed -i -- "s@OBSERVABLE@$OBS_NAME@g" $FILE_RUN

#Replace identifier name
sed -i -- "s@IDENTIFIER@$IDENT@g" $FILE_RUN

#Replace filenames
sed -i -- "s@OUTPUTFILE@$FILE_OUT@g" $FILE_RUN

#Determine detector position
for idx in $(echo $OBS_POS | sed "s/,/ /g");
do
		
done

for idx in $(echo $OBS_POS | tr "," "\n")
do
	if [ $(grep -c "INDEXETA" $FILE_RUN) -eq 1 ]; then
		sed -i -- "s@INDEXETA@$idx@g" $FILE_RUN
	elif [ $(grep -c "INDEXPHI" $FILE_RUN) -eq 1 ]; then
		sed -i -- "s@INDEXPHI@$idx@g" $FILE_RUN
	elif [ $(grep -c "INDEXSLICE" $FILE_RUN) -eq 1 ]; then
		sed -i -- "s@INDEXSLICE@$idx@g" $FILE_RUN
	fi
done

#Determine which files should be analyzed
sed -i '$ a [BEGIN_RUN_LIST]' $FILE_RUN
for f in *.root
do
	INPUTFILENAME=$DIR_DATA/$f
	sed -i "$ a $INPUTFILENAME" $FILE_RUN
done
sed -i '$ a [END_RUN_LIST]' $FILE_RUN

#return to the original directory
cd $DIR_ORIG

#tell the user what to do
echo "I have created a run configuration file for you at:"
echo "$FILE_RUN"
echo ""
echo "To view this file execute:"
echo "gedit $FILE_RUN &"
echo ""
echo "To launch the analysis execute:"
echo "cd $GEM_BASE"
echo "./analyzeUniformity $FILE_RUN true"
