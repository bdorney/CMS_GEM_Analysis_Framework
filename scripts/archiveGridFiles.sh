#!/bin/zsh

# Store Original Directory
DIR_ORIG=$PWD

# Make temporary directory
mkdir $GEM_BASE/tmp

# Move run config files
mv $GEM_BASE/config/configRun_RunNo*.cfg $GEM_BASE/tmp

# Move submission scripts
mv $GEM_BASE/scripts/submitFrameworkJob_RunNo*.sh $GEM_BASE/tmp

# Move std err, log, and out files
mv $GEM_BASE/stderr/*.txt $GEM_BASE/tmp
mv $GEM_BASE/stdlog/*.txt $GEM_BASE/tmp
mv $GEM_BASE/stdout/*.txt $GEM_BASE/tmp

# Move to temporary directory
cd $GEM_BASE/tmp

# Get the Date 
TODAY=`date +%Y-%m-%d-%H-%M-%S`

# Archive
tar -cf gridArchive_$TODAY.tar *
mv gridArchive_$TODAY.tar $GEM_BASE

# Return to original directory
cd $DIR_ORIG

# Clean up temporary directory
rm $GEM_BASE/tmp/*
rmdir $GEM_BASE/tmp
