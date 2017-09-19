#!/bin/zsh
#Usage:
#	source makeAllPlots.sh <Plot Config File Directory>

#store the original directory
DIR_ORIG=$PWD

#setup input variables
DIR_CONFIG=$1

#Move to the data directory
cd $DIR_CONFIG

#Make Plots
for f in *.cfg
do
	echo "=============================================="
	echo "Making plot from file:" $f
	$FRAMEWORK_BASE/genericPlotter $f false
done

echo "=============================================="
echo "All Plots Have Been Made!"

echo "Your plots will be found in:"
echo ""
echo "\t"$DIR_CONFIG
echo ""

#return to the original directory
cd $DIR_ORIG
