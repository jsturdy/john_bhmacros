# Classes for working with monte carlo samples.
# Useful for dealing with lots of samples for which different weights need to be applied.
# Author: John Hakala, 10/14/2015

class MCSample:
    # Holds the name of a sample, the file where the sample is stored,
    # the cross section for a sample, and the number of events in the sample
    def __init__(self, sampleName, sampleFile, xSect, nEvents):
        self.sampleName = sampleName
        self.sampleFile = sampleFile
        self.xSect = xSect
        self.nEvents = nEvents

    def getSampleName(self):
        print "Sample name: %s" % self.sampleName

    def getSampleFile(self):
        print "   --> filename: %s" % self.sampleFile

    def getXSect(self):
        print "   --> cross sction: %f" % self.xSect

    def getNEvents(self):
        print "   --> number of events: %d" % self.nEvents

class MCSampleWithWeight:
    # Holds an MCSample and calculates its weight by being supplied an integrated luminosity
    def __init__(self, mcSampleObj, weight):
        self.mcSampleObj = mcSampleObj
        self.weight = weight

    def showWeight(self):
        print "Sample with filename %s has weight %f" % (self.mcSampleObj.sampleFile, self.weight)

class MCSamplesWeightsMap:
    # Holds a map of different samples with different weights and provides methods to
    # print out info about the map and to return the map in an easily navigable array
    def __init__(self, lumi, filename):
        self.lumi = lumi
        self.fileName = filename
        self.samples = []
        self.samplesWithWeights = []
        with open(self.fileName) as qcdsamplestext:
            for line in qcdsamplestext.readlines():
                sampleInfo = line.split(",")
                self.samples.append(MCSample( str(   sampleInfo[0].strip() ),
                                         str(   sampleInfo[1].strip() ),
                                         float( sampleInfo[2].strip() ),
                                         int(   sampleInfo[3].strip() )
                                       )
                              )
        for sample in self.samples:
            nExpected = float(self.lumi * sample.xSect)
            weight = nExpected/float(sample.nEvents)
            self.samplesWithWeights.append(MCSampleWithWeight(sample, weight))

    def showMap(self):
        for sampleWithWeight in self.samplesWithWeights:
            sampleWithWeight.showWeight()

    def getMap(self):
        mcMap = [];
        for sampleWithWeight in self.samplesWithWeights:
            mcMapRow = [sampleWithWeight.mcSampleObj.sampleName,
                        sampleWithWeight.mcSampleObj.sampleFile,
                        sampleWithWeight.weight]
            mcMap.append(mcMapRow)
        return mcMap
