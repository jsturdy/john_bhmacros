# This macro takes an integrated luminosity as well as  input textfile
# The input textfile is a list of MC samples along with their cross 
# sections and number of events. It then uses mctools to calculate the
# appropriate weight for each sample, and them sends them to a c++ root
# macro to make an ST histogram
# John Hakala, 10/23/2015

# Usage example: 
# python bhSTmaker_mc.py qcdSamples_AhmadOct23.tx 1020
# the option following bhSTmaker_mc.py is the name of a file in the samplesLists_mc directory.
# This text file is a list of the MC ntuples' with the following format:
# <das name of source sample> ,  <full path to ntuple file on eos, starting with /store> ,  <cross section from DAS> ,  <number of events in source sample>
# Each file should be on its own line. 
# The second option is the integrated luminosity in /pb to which the samples are normalized

import sys
from macrotools import *
from calcweights import *

if len(sys.argv)!=3:
	print "Please supply two arguments to bhSTmaker_mc.py"
	print " --> Example:"
	print "     python bhSTmaker_mc.py exampleList.tx 1337"
	exit(1)

sampleListFile = "samplesLists_mc/" + sys.argv[1]
mcMap = MCSamplesWeightsMap(sys.argv[2], sampleListFile)
mapArray = mcMap.getMap()
samplesWithWeightsFile = open(".tmp.tx", "w")
for mapRow in mapArray:
	samplesWithWeightsFile.write(str(mapRow[1]) + "," + str(mapRow[2]) + "\n")
samplesWithWeightsFile.close()

macroName = "SThist_mc.cc"
tmpFile = ".tmp.tx"
callMacro(macroName, tmpFile)
