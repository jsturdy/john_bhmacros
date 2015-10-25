import subprocess
import math

def callMacro(macroName, inputFile):
	macroCommand = "%s(\"%s\")" % (macroName, inputFile)
	subprocess.call(["root", "-l", "-q", macroCommand])

def splitJobsForBsub(dataOrMC, inputFile, numberOfJobs):
	if dataOrMC == "data":
		samplesListsDir="samplesLists_data"
	if dataOrMC == "mc":
		samplesListsDir="samplesLists_mc"
	else:
		print "Please choose either 'data' or 'mc'." 
		exit(1)
	inputFilePath = samplesListsDir+"/"+inputFile
	num_input_samples = sum(1 for line in open(inputFilePath))
	with open(inputFilePath) as inputSamplesList:
		print "number of input samples is %i" % num_input_samples
		chunksize = math.ceil(num_input_samples / numberOfJobs)
		print "chunksize is: %i" % chunksize
		#chunksize = 2
		fid = 1
		lineswritten = 0
		f = open(samplesListsDir+"/splitLists/"+'%s_split_%i.tx'%(inputFile, fid), 'w')
		for i,line in enumerate(inputSamplesList):
			print "%i : \n      %s" % (i, line)
			f.write(line)
			lineswritten+=1
			if lineswritten == chunksize and fid<=numberOfJobs:
				f.close()
				fid += 1
				lineswritten = 0
				f = open(samplesListsDir+"/splitLists/"+'%s_split_%i.tx'%(inputFile, fid), 'w')
		f.close()
		return fid

def bSubSplitJobs(macroname, dataOrMC, inputFile, numberOfJobs):
	#splitJobsForBsub(dataOrMC, inputFile, numberOfJobs)
	nJobs = splitJobsForBsub(dataOrMC, inputFile, numberOfJobs)
	#print "Going to submit %i jobs on bsub." % nJobs
