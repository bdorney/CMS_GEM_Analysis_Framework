#!bin/bash

clear

setenv GEM_BASE $PWD

source /afs/cern.ch/sw/lcg/contrib/gcc/4.8/x86_64-slc6/setup.csh
#source /afs/cern.ch/sw/lcg/contrib/gcc/4.9.2/x86_64-slc6/setup.sh

#cd /afs/cern.ch/sw/lcg/app/releases/ROOT/6.00.02/x86_64-slc6-gcc48-opt/root
cd /afs/cern.ch/sw/lcg/app/releases/ROOT/6.06.06/x86_64-slc6-gcc48-opt/root
source bin/thisroot.csh

cd $GEM_BASE

which root
