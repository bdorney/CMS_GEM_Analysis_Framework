#!bin/bash

clear

#Define Base Directory
export GEM_BASE=$PWD

#Setup gcc/g++
source /afs/cern.ch/sw/lcg/contrib/gcc/4.8/x86_64-slc6/setup.sh
#source /afs/cern.ch/sw/lcg/contrib/gcc/4.9.2/x86_64-slc6/setup.sh

#Setup ROOT
#cd /afs/cern.ch/sw/lcg/app/releases/ROOT/6.00.02/x86_64-slc6-gcc48-opt/root
cd /afs/cern.ch/sw/lcg/app/releases/ROOT/6.06.06/x86_64-slc6-gcc48-opt/root
#cd /afs/cern.ch/sw/lcg/app/releases/ROOT/6.06.08/x86_64-slc6-gcc49-opt/root
source bin/thisroot.sh

cd $GEM_BASE

#Setup Python
#export PYTHONDIR=/afs/cern.ch/sw/lcg/external/Python/2.7.3/x86_64-slc6-gcc48-opt
export PYTHONDIR=/afs/cern.ch/sw/lcg/external/Python/2.7.4/x86_64-slc6-gcc48-opt
export PYTHONPATH=$PYTHONPATH:$ROOTSYS/lib
export LD_LIBRARY_PATH=$ROOTSYS/lib:$PYTHONDIR/lib:$LD_LIBRARY_PATH:/opt/rh/python27/root/usr/lib64
alias python2.7="$PYTHONDIR/bin/python2.7"
alias python2.7-config="$PYTHONDIR/bin/python2.7-config"

#Install pip & other packages (should be done only once)
DIR_PIP=$GEM_BASE/python/pip
if [[ !  -d $DIR_PIP ]]; then
	#Installing pip
        python2.7 python/get-pip.py --prefix=$DIR_PIP

	#Update environment
	export PYTHONPATH=$DIR_PIP/lib/python2.7/site-packages:$PYTHONPATH
	export PATH="$PATH:~/.local/bin"
	alias pip2.7="$DIR_PIP/bin/pip"
	
	#echo "-----"
	#echo "ls $DIR_PIP/bin/pip"
	#ls $DIR_PIP/bin/pip
	#echo "pip2.7 = " 
	#which pip2.7
	#echo "-----"

	#Installing packages
	pip2.7 install --user numpy scipy matplotlib ipython jupyter pandas sympy nose root_numpy
else
	#Update environment	
	export PYTHONPATH=$DIR_PIP/lib/python2.7/site-packages:$PYTHONPATH
	export PATH="$PATH:~/.local/bin"
	alias pip2.7="$DIR_PIP/bin/pip"
fi

#alias helper scripts
#alias makePlots="source $GEM_BASE/scripts/makeAllPlots.sh"

which root
which python2.7
which pip2.7
