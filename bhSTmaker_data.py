# This macro takes an input textfile with a list of samples and sends them to a c++ root macro
# to make an ST histogram
# John Hakala, 10/22/2015

# Usage example: 
# python bhSTmaker.py firstTen2015Dpmptv4.tx
# the option following bhSTmaker.py is the name of a file in the samplesLists directory.
# This text file is a list of the data ntuples' full paths on eos starting with /store.
# Each file should be on its own line. 

import sys
from macrotools import *

if len(sys.argv)!=2:
	print "Please supply one argument to bhSTmaker.py"
	print " --> Example:"
	print "     python bhSTmaker.py exampleList.tx"
	exit(1)

macroName = "SThist.cc"
inputFile = "samplesLists_data/%s" % str(sys.argv[1])
callMacro(macroName, inputFile)
