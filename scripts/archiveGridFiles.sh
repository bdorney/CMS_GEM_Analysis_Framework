#!/bin/zsh

# Store Original Directory
DIR_ORIG=$PWD

# Make temporary directory
mkdir $FRAMEWORK_BASE/tmp

# Move run config files
mv $FRAMEWORK_BASE/config/configRun_JobNo*.cfg $FRAMEWORK_BASE/tmp

# Move submission scripts
mv $FRAMEWORK_BASE/scripts/submitFrameworkJob_JobNo*.sh $FRAMEWORK_BASE/tmp

# Move std err, log, and out files
mv $FRAMEWORK_BASE/stderr/*.txt $FRAMEWORK_BASE/tmp
mv $FRAMEWORK_BASE/stdlog/*.txt $FRAMEWORK_BASE/tmp
mv $FRAMEWORK_BASE/stdout/*.txt $FRAMEWORK_BASE/tmp

# Move to temporary directory
cd $FRAMEWORK_BASE/tmp

# Get the Date 
TODAY=`date +%Y-%m-%d-%H-%M-%S`

# Archive
tar -cf gridArchive_$TODAY.tar *
mv gridArchive_$TODAY.tar $FRAMEWORK_BASE

# Return to original directory
cd $DIR_ORIG

# Clean up temporary directory
rm $FRAMEWORK_BASE/tmp/*
rmdir $FRAMEWORK_BASE/tmp
