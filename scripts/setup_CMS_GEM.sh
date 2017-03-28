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
export PYTHONDIR=/afs/cern.ch/sw/lcg/external/Python/2.7.3/x86_64-slc6-gcc48-opt
#export PYTHONDIR=/afs/cern.ch/sw/lcg/external/Python/2.7.4/x86_64-slc6-gcc48-opt
export PYTHONPATH=$PYTHONPATH:$ROOTSYS/lib
export LD_LIBRARY_PATH=$ROOTSYS/lib:$PYTHONDIR/lib:$LD_LIBRARY_PATH:/opt/rh/python27/root/usr/lib64
alias python2.7="$PYTHONDIR/bin/python2.7"
alias python2.7-config="$PYTHONDIR/bin/python2.7-config"

which root
which python2.7
