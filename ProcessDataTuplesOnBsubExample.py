from calcweights import *
from macrotools import *

mcMap = MCSamplesWeightsMap(209, "samplesLists_mc/qcdSamples_AhmadOct23.tx")
#mcMap.showMap()

mapArray = mcMap.getMap()
samplesWithWeights = open(".tmp.tx", "w")
for mapRow in mapArray:
    #print str(mapRow[1]) + ", " + str(mapRow[2])
    samplesWithWeights.write(str(mapRow[1]) + ", " + str(mapRow[2]) + "\n")
samplesWithWeights.close()

bSubSplitJobs("bhSTmaker_data.py", "data", "data2015D_oct23.tx", 50)


