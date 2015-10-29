import subprocess
import math
import os
import shutil

def callMacro(macroName, inputFile, outputFile):
	macroCommand = "%s(\"%s\", \"%s\")" % (macroName, inputFile, outputFile)
	subprocess.call(["root", "-l", "-q", macroCommand])

def splitJobsForBsub(dataOrMC, inputFile, numberOfJobs):
	if dataOrMC == "data":
		samplesListsDir="samplesLists_data"
	elif dataOrMC == "mc":
		samplesListsDir="samplesLists_mc"
	else:
		print "Please choose either 'data' or 'mc'." 
		exit(1)
	inputFilePath = samplesListsDir+"/"+inputFile
	num_input_samples = sum(1 for line in open(inputFilePath))
	with open(inputFilePath) as inputSamplesList:
		#print "number of input samples is %i" % num_input_samples
		chunksize = num_input_samples / (numberOfJobs-1)
		print "chunksize is: %i" % chunksize
		fid = 1
		lineswritten = 0
		f = open(samplesListsDir+"/splitLists/"+'split_%i_%s'%(fid, inputFile), 'w')
		for i,line in enumerate(inputSamplesList):
			#print "%i : \n      %s" % (i, line)
			f.write(line)
			lineswritten+=1
			if lineswritten == chunksize and fid<numberOfJobs:
				f.close()
				fid += 1
				lineswritten = 0
				f = open(samplesListsDir+"/splitLists/"+'split_%i_%s'%(fid, inputFile), 'w')
		f.close()
		return fid

def bSubSplitJobs(pyScriptName, dataOrMC, inputFile, numberOfJobs):
	if dataOrMC == "data":
		samplesListsDir="samplesLists_data"
	if dataOrMC == "mc":
		samplesListsDir="samplesLists_mc"
		lumiNorm = raw_input("Please enter the integrated luminosity to normalize: ")
	#splitJobsForBsub(dataOrMC, inputFile, numberOfJobs)
	clearSplitLists(dataOrMC)
	clearBsubShellScripts()
	nJobs = splitJobsForBsub(dataOrMC, inputFile, numberOfJobs)
	print "Prepared %i jobs ready to be submitted to bsub." % nJobs
	for i in range (1, nJobs+1):
		splitListFile="split_%i_%s" % (i , inputFile)
		pyCommand = "python " + pyScriptName + " splitLists/" + splitListFile + " " + "output/" + pyScriptName + "-output_%i-" %i + inputFile + ".root"
		if dataOrMC == "mc":
			pyCommand = pyCommand + " " + lumiNorm
		makeBsubShellScript(pyCommand, samplesListsDir+"/splitLists/"+splitListFile, pyScriptName, i)

def makeBsubShellScript(pyCommand, splitListName, pyScriptName, index):
	f = open("bsubs/bsub-%s-%s.sh" % (pyScriptName, index), "w")
	f.write("#!/bin/bash\n")
	f.write("source /cvmfs/cms.cern.ch/cmsset_default.sh\n")
	f.write("cd " + os.getcwd()+"\n")
	f.write("source bhSetup.sh /afs/cern.ch/user/j/johakala/work/public/CMSSW_7_4_14/src\n")
	f.write(pyCommand)
	f.close()
	os.chmod("bsubs/bsub-%s-%s.sh" % (pyScriptName, index), 0777)

def clearSplitLists(dataOrMC):
	if dataOrMC == "data":
		samplesListsDir="samplesLists_data"
	if dataOrMC == "mc":
		samplesListsDir="samplesLists_mc"
	splitListsDir=samplesListsDir+"/splitLists/"
	for the_file in os.listdir(splitListsDir):
		file_path = os.path.join(splitListsDir, the_file)
		try:
			if os.path.isfile(file_path):
				os.unlink(file_path)
			#elif os.path.isdir(file_path): shutil.rmtree(file_path)
		except Exception, e:
			print e

def clearBsubShellScripts():
	bSubScriptsDir="bsubs/"
	for the_file in os.listdir(bSubScriptsDir):
		file_path = os.path.join(bSubScriptsDir, the_file)
		try:
			if os.path.isfile(file_path):
				os.unlink(file_path)
			#elif os.path.isdir(file_path): shutil.rmtree(file_path)
		except Exception, e:
			print e
