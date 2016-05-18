#!bin/zsh
#Usage:
#	source runMode_Series.sh <Data File Directory> <Config File - Analysis> <Config File - Mapping>

#store the original directory
DIR_ORIG=$PWD

#store the run config file
#FILE_RUN=$GEM_BASE/config/configRun.cfg
FILE_RUN_TEMP=$GEM_BASE/config/configRun_Template_Grid.cfg

#setup input variables
#NAME_DET=$1
DIR_DATA=$1
FILE_ANA=$2
FILE_MAP=$3
#FILE_OUT=$5
#NAME_QUEUE=$4

#Move to the data directory
cd $DIR_DATA

#Determine which files should be analyzed
COUNTER=0
for f in *dataTree.root
do
    #increment counter
    let COUNTER=$(( ${COUNTER} + 1 ))

    #copy file
    FILE_RUN=$GEM_BASE/config/configRun_${COUNTER}.cfg
    cp $FILE_RUN_TEMP $FILE_RUN

    #Replace Detector Name
    #sed -i -- "s@DETECTORNAME@$NAME_DET@g" $FILE_RUN

    #Replace filenames
    sed -i -- "s@CONFIGFILE_ANALYSIS@$FILE_ANA@g" $FILE_RUN
    sed -i -- "s@CONFIGFILE_MAPPING@$FILE_MAP@g" $FILE_RUN
    #sed -i -- "s@OUTPUTFILE@$FILE_OUT@g" $FILE_RUN

    #Add Run name
	INPUTFILENAME=$DIR_DATA/$f
    sed -i '$ a [BEGIN_RUN_LIST]' $FILE_RUN
	sed -i "$ a $INPUTFILENAME" $FILE_RUN
    sed -i '$ a [END_RUN_LIST]' $FILE_RUN

    #Check for the stderr directory
    DIR_STDERR=$GEM_BASE/stderr
    if [[ ! -n $( -d $DIR_STDERR ) ]]; then
        echo "Creating stdout directory: $GEM_BASE/stderr"
        mkdir $DIR_STDERR
    fi

    #Check for the stdlog directory
    DIR_STDLOG=$GEM_BASE/stdlog
    if [[ ! -n $( -d $DIR_STDLOG ) ]]; then
        echo "Creating stdout directory: $GEM_BASE/stdlog"
        mkdir $DIR_STDLOG
    fi

    #Check for the stdout directory
    DIR_STDOUT=$GEM_BASE/stdout
    if [[ ! -n $( -d $DIR_STDOUT ) ]]; then
        echo "Creating stdout directory: $GEM_BASE/stdout"
        mkdir $DIR_STDOUT
    fi

    #make the script to send to the scheduler
    DIR_SCRIPTS=$GEM_BASE/scripts
    FILE_SCRIPT=$GEM_BASE/scripts/submitFrameworkJob_${COUNTER}.sh
    echo '#!/bin/zsh' >> $FILE_SCRIPT
    echo "cd $GEM_BASE" >> $FILE_SCRIPT
    echo "nohup ./analyzeUniformity $FILE_RUN true >> $DIR_STDLOG/frameworkLog_${counter}.txt &" >> $FILE_SCRIPT

    #make the script executable
    chmod 777 $FILE_SCRIPT

    #Launch the Job
    sleep 2
    echo "launching job: " $FILE_SCRIPT
    echo "bsub -o $DIR_STDOUT/frameworkOut_${COUNTER}.txt -e $DIR_STDERR/frameworkErr_${COUNTER} $FILE_SCRIPT"
    sleep 2
done

#return to the original directory
cd $DIR_ORIG

