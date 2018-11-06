#!bin/bash

clear

#Check Paths
if [[ -n "$FRAMEWORK_BASE" ]]; then
        echo FRAMEWORK_BASE $FRAMEWORK_BASE
else
    echo "FRAMEWORK_BASE not set, please set FRAMEWORK_BASE to the directory above the root of your repository"
    echo " (export FRAMEWORK_BASE=<your path>/CMS_GEM_Analysis_Framework) and then rerun this script"
    return
fi

# Add to paths
export PATH=$PATH:$FRAMEWORK_BASE
export PATH=$PATH:$FRAMEWORK_BASE/python
export PATH=$PATH:$FRAMEWORK_BASE/scripts

#Setup eos
alias eos='/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select'
export CMS_GEM_EOS_COMM=/eos/cms/store/group/dpg_gem/comm_gem/QualityControl

#Setup gcc/g++
source /afs/cern.ch/sw/lcg/contrib/gcc/4.8/x86_64-slc6/setup.sh

#Setup ROOT
cd /afs/cern.ch/sw/lcg/app/releases/ROOT/6.06.06/x86_64-slc6-gcc48-opt/root
source bin/thisroot.sh

cd $FRAMEWORK_BASE

#Setup Python
export PYTHONDIR=/afs/cern.ch/sw/lcg/external/Python/2.7.4/x86_64-slc6-gcc48-opt
export PYTHONPATH=$PYTHONPATH:$ROOTSYS/lib
export LD_LIBRARY_PATH=$ROOTSYS/lib:$PYTHONDIR/lib:$LD_LIBRARY_PATH:/opt/rh/python27/root/usr/lib64
alias python2.7="$PYTHONDIR/bin/python2.7"
alias python2.7-config="$PYTHONDIR/bin/python2.7-config"

#Install pip & other packages (should be done only once)
DIR_PIP=$FRAMEWORK_BASE/python/pip
if [[ !  -d $DIR_PIP ]]; then
	#Installing pip
        python2.7 python/get-pip.py --prefix=$DIR_PIP

	#Update environment
	export PYTHONPATH=$DIR_PIP/lib/python2.7/site-packages:$PYTHONPATH
	export PATH="$PATH:~/.local/bin"
	alias pip2.7="$DIR_PIP/bin/pip"

	#Installing packages
	#$DIR_PIP/bin/pip install --user numpy scipy matplotlib ipython jupyter pandas sympy nose root_numpy xlrd
	$DIR_PIP/bin/pip install --user numpy root_numpy xlrd
else
	#Update environment	
	export PYTHONPATH=$DIR_PIP/lib/python2.7/site-packages:$PYTHONPATH
	export PATH="$PATH:~/.local/bin"
	alias pip2.7="$DIR_PIP/bin/pip"
fi

which root
which python2.7
which pip2.7
