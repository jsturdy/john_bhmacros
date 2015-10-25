#!/bin/bash
# This setup script takes one argument -- the path to your CMSSW's src directory. This is needed to set up a current version of ROOT on lxplus since it has an outdated version by default. 
# The other thing this script does is set some custom python tools up.
# Example: source bhSetup.sh /afs/cern.ch/user/j/johakala/work/public/CMSSW_7_4_14/src
if [ "$#" -ne 1 ]; then
    echo "Please supply the path to your CMSSW area's 'src' directory"
    echo "Usage Example:"
    echo "source bhSetup.sh /afs/cern.ch/user/j/johakala/work/public/CMSSW_7_4_14/src"
fi
MYDIR=$PWD
cd $1
cmsenv
cd $MYDIR
export PYTHONPATH=$PYTHONPATH:$PWD/mctools:$PWD/macrotools

