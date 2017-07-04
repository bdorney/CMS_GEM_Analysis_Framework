#!/bin/zsh
#Usage:
#	runMode_Grid_Reco.sh <Detector Name> <Config File - Reco> <Config File - Mapping> <Queue Names> <Output Dir>
#
#	Possible queue names on lxplus LSF system are:
#		8nm	Eight natural minutes
#		1nh	One natural hour
#		8nh	Eight natural hours
#		1nd	One natural day
#
#Example:
#   runMode_Grid_Reco.sh GE11-VII-L-CERN-0001 config/configReco.cfg config/Mapping_GE11-VII-L.cfg 1nh /path/to/my/data/directory

#Substring comparison function
#See: https://stackoverflow.com/questions/2829613/how-do-you-tell-if-a-string-contains-another-string-in-unix-shell-scripting
#Returns 0 if str contains substr
#Returns 1 otherwise
strcmp() {
    string="$1"
    substring="$2"
    if test "${string#*$substring}" != "$string"
    then
        return 0
    else
        return 1
    fi
}

#store the original directory
DIR_ORIG=$PWD

#store the run config file
FILE_RUN_TEMP=$GEM_BASE/config/configRun_Template_Grid_Reco.cfg

#setup input variables
NAME_DET=$1
FILE_RECO=$2
FILE_MAP=$3
NAME_QUEUE=$4
DIR_OUTPUT=$5

# Setup eos
CMS_GEM_EOS_COMM=/eos/cms/store/group/dpg_gem/comm_gem
EOS=/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select

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

#Determine which files should be analyzed
COUNTER=0
for file in $(ls $CMS_GEM_EOS_COMM/$1/*.raw )
do
    #increment counter
    let COUNTER=$(( ${COUNTER} + 1 ))

    #copy file
    FILE_RUN=$GEM_BASE/config/configRun_JobNo${COUNTER}.cfg
    cp $FILE_RUN_TEMP $FILE_RUN

    #Replace filenames - Reco
    for substr in $(echo $FILE_RECO | tr "/" "\n")
    do
        if strcmp $substr ".cfg"
        then
            sed -i -- "s@CONFIGFILE_RECO@$substr@g" $FILE_RUN
            break
        fi
    done

    #Replace filenames - Mapping
    for substr in $(echo $FILE_MAP | tr "/" "\n")
    do
        if strcmp $substr ".cfg"
        then
            sed -i -- "s@CONFIGFILE_MAPPING@$substr@g" $FILE_RUN
            break
        fi
    done

    #Add Run name
    for substr in $(echo $file | tr "/" "\n")
    do
        if strcmp $substr ".raw"
        then
            sed -i '$ a [BEGIN_RUN_LIST]' $FILE_RUN
	        sed -i "$ a $substr" $FILE_RUN
            sed -i '$ a [END_RUN_LIST]' $FILE_RUN
            break
        fi
    done

    #Strip filepath from $FILE_RUN
    FILE_RUN_LOCAL="null"
    for substr in $(echo $FILE_RUN | tr "/" "\n")
    do
        if strcmp $substr ".cfg"
        then
            FILE_RUN_LOCAL=$substr
            break
        fi
    done

    #make the script to send to the scheduler
    DIR_JOBNUM=/tmp/JobNo${COUNTER}
    DIR_SCRIPTS=$GEM_BASE/scripts
    FILE_SCRIPT=$GEM_BASE/scripts/submitFrameworkJob_JobNo${COUNTER}.sh
    echo '#!/bin/zsh' >> $FILE_SCRIPT
    echo "" >> $FILE_SCRIPT
    echo "#Setup environment" >> $FILE_SCRIPT
    echo "cd $GEM_BASE" >> $FILE_SCRIPT
    echo "source scripts/setup_CMS_GEM.sh" >> $FILE_SCRIPT
    echo "" >> $FILE_SCRIPT
    echo "#Make temporary directory" >> $FILE_SCRIPT
    echo "mkdir $DIR_JOBNUM" >> $FILE_SCRIPT
    echo "" >> $FILE_SCRIPT
    echo "#Copy Config files" >> $FILE_SCRIPT
    echo "cp $FILE_RECO $DIR_JOBNUM" >> $FILE_SCRIPT
    echo "cp $FILE_MAP $DIR_JOBNUM" >> $FILE_SCRIPT
    echo "cp $FILE_RUN $DIR_JOBNUM" >> $FILE_SCRIPT
    echo "" >> $FILE_SCRIPT
    echo "#Move to temporary directory and copy input file" >> $FILE_SCRIPT
    echo "cd $DIR_JOBNUM" >> $FILE_SCRIPT
    echo "$EOS cp $file ." >> $FILE_SCRIPT
    echo "" >> $FILE_SCRIPT
    echo "#Launch frameworkMain" >> $FILE_SCRIPT
    echo "frameworkMain $FILE_RUN_LOCAL true 2>&1 | tee $DIR_STDLOG/frameworkLog_JobNo${COUNTER}.txt" >> $FILE_SCRIPT
    echo "" >> $FILE_SCRIPT
    echo "#Move Output" >> $FILE_SCRIPT
    echo "if [[ !  -d $DIR_OUTPUT/$NAME_DET ]]; then" >> $FILE_SCRIPT
    echo "    mkdir $DIR_OUTPUT/$NAME_DET" >> $FILE_SCRIPT
    echo "fi" >> $FILE_SCRIPT
    echo "mv *dataTree.root $DIR_OUTPUT/$NAME_DET" >> $FILE_SCRIPT
    echo "" >> $FILE_SCRIPT
    echo "#Delete Input" >> $FILE_SCRIPT
    echo "/bin/rm $file" >> $FILE_SCRIPT

    #make the script executable
    chmod +x $FILE_SCRIPT

    #Launch the Job
    sleep 1
    echo "launching job: " $FILE_SCRIPT
    echo "bsub -q $NAME_QUEUE -o $DIR_STDOUT/frameworkOut_JobNo${COUNTER}.txt -e $DIR_STDERR/frameworkErr_JobNo${COUNTER}.txt $FILE_SCRIPT"
    bsub -q $NAME_QUEUE -o $DIR_STDOUT/frameworkOut_JobNo${COUNTER}.txt -e $DIR_STDERR/frameworkErr_JobNo${COUNTER}.txt $FILE_SCRIPT
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
echo "	ls $DIR_OUTPUT/$NAME_DET"
echo ""
echo "this will show the contents of the directory"
echo ""
