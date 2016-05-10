#!bin/zsh
#Usage:
#	source runMode_Series.sh <Data File Directory> <Config File - Analysis> <Config File - Mapping> <Output Data Filename>

#store the original directory
DIR_ORIG=$PWD

#store the run config file
FILE_RUN=$GEM_BASE/config/configRun.cfg
FILE_RUN_TEMP=$GEM_BASE/config/configRun_Template_Series.cfg
cp $FILE_RUN_TEMP $FILE_RUN

#setup input variables
DIR_DATA=$1
FILE_ANA=$2
FILE_MAP=$3
FILE_OUT=$4

#Move to the data directory
cd $DIR_DATA

#Determine which files should be analyzed
sed -i '$ a [BEGIN_RUN_LIST]' $FILE_RUN
for f in *dataTree.root
do	
	INPUTFILENAME=$DIR_DATA/$f
	sed -i "$ a $INPUTFILENAME" $FILE_RUN
done
sed -i '$ a [END_RUN_LIST]' $FILE_RUN

#Replace filenames
sed -i -- "s@CONFIGFILE_ANALYSIS@$FILE_ANA@g" $FILE_RUN
sed -i -- "s@CONFIGFILE_MAPPING@$FILE_MAP@g" $FILE_RUN
sed -i -- "s@OUTPUTFILE@$FILE_OUT@g" $FILE_RUN

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
echo ".$GEM_BASE/analyzeUniformity $FILE_RUN true"
