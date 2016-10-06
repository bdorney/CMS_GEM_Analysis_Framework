#!bin/zsh
#Usage:
#	source runMode_Grid.sh <Data File Directory> <Config File - Analysis> <Config File - Mapping> <Queue Names>
#
#	Possible queue names on lxplus LSF system are:
#		8nm	Eight natural minutes
#		1nh	One natural hour
#		8nh	Eight natural hours
#		1nd	One natural day

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
NAME_QUEUE=$4

#Move to the data directory
cd $DIR_DATA

#Determine which files should be analyzed
COUNTER=0
for f in *dataTree.root
do
    #increment counter
    let COUNTER=$(( ${COUNTER} + 1 ))

    #copy file
    FILE_RUN=$GEM_BASE/config/configRun_RunNo${COUNTER}.cfg
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
    if [[ !  -d $DIR_STDERR ]]; then
        echo "Creating stdout directory: $GEM_BASE/stderr"
        mkdir $DIR_STDERR
    fi

    #Check for the stdlog directory
    DIR_STDLOG=$GEM_BASE/stdlog
    if [[ ! -d $DIR_STDLOG ]]; then
        echo "Creating stdout directory: $GEM_BASE/stdlog"
        mkdir $DIR_STDLOG
    fi

    #Check for the stdout directory
    DIR_STDOUT=$GEM_BASE/stdout
    if [[ ! -d $DIR_STDOUT ]]; then
        echo "Creating stdout directory: $GEM_BASE/stdout"
        mkdir $DIR_STDOUT
    fi

    #make the script to send to the scheduler
    DIR_SCRIPTS=$GEM_BASE/scripts
    FILE_SCRIPT=$GEM_BASE/scripts/submitFrameworkJob_RunNo${COUNTER}.sh
    echo '#!/bin/zsh' >> $FILE_SCRIPT
    echo "cd $GEM_BASE" >> $FILE_SCRIPT
    echo "source scripts/setup_CMS_GEM.sh"
    echo "nohup ./frameworkMain $FILE_RUN true >> $DIR_STDLOG/frameworkLog_RunNo${COUNTER}.txt &" >> $FILE_SCRIPT

    #make the script executable
    chmod 777 $FILE_SCRIPT

    #Launch the Job
    sleep 1
    echo "launching job: " $FILE_SCRIPT
    echo "bsub -q $NAME_QUEUE -o $DIR_STDOUT/frameworkOut_RunNo${COUNTER}.txt -e $DIR_STDERR/frameworkErr_RunNo${COUNTER}.txt $FILE_SCRIPT"
    bsub -q $NAME_QUEUE -o $DIR_STDOUT/frameworkOut_RunNo${COUNTER}.txt -e $DIR_STDERR/frameworkErr_RunNo${COUNTER}.txt $FILE_SCRIPT
    sleep 1
done

#return to the original directory
cd $DIR_ORIG

#Tell the user some useful information
echo "Hi I have finished submitting your jobs"
echo ""
echo "To check the status of your jobs call:"
echo "	bjobs"
echo ""
echo "To kill a running job call:"
echo "	bkill JOBID"
echo ""
echo "Here JOBID is the number returned when calling bjobs"
echo ""
echo "To force kill a running job call:"
echo "	bkill -r JOBID"
echo ""
echo "For the full manual and options please see: https://batchconf.web.cern.ch/batchconf/doc/lsf/print/lsf_users_guide.pdf"
echo ""
echo "Once all jobs are finished and you are satisfied with the output you may call:"
echo "	source scripts/cleanGridFiles.sh"
echo ""
echo "To safely and conveniently remove the stdout, stderr, stdlog, config, and script files"
echo ""
echo "After all jobs have completed call:"
echo "	cd $DIR_DATA"
echo "	hadd somefilename.root *Ana.root"
echo ""
echo "this will create a new TFile that is the sum of all the individual TFile's"
echo ""
