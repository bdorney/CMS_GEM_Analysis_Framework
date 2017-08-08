#!/bin/zsh
# usage:
# ./copy2eos_ROOT.sh <Detector Serial Number> <Target Directory>
# example:
# source ../copy2eos_ROOT.sh GE11-III-CERN-0002 .

eos=/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select

DETECTOR=$1
DIRECTORY=/eos/cms/store/group/dpg_gem/comm_gem/QualityControl/$DETECTOR

if [[ ! -n $($eos find $DIRECTORY) ]]; then
        echo "Creating Directory: "$DIRECTORY
        $eos mkdir $DIRECTORY
else
        echo "Existing Directory: "$DIRECTORY
fi

#Get the files to send
FILES=$(ls $2/*dataTree.root)

#Tell the user which group of files will be copied
echo "The list of files to be copied is:"
for f in $FILES\;
do
        echo $f
done

#Copy the files
echo "Copying files"
for f in $FILES;
do
        echo "Processing " $f
        $eos cp "$f" $DIRECTORY/$f
done

#Confirm for the user that the files were copied
echo "Showing the contents of " $DIRECTORY
$eos ls $DIRECTORY
