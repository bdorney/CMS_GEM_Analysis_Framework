from AnalysisSuiteEfficiencyPredictor import *
myAnaSuite = AnalysisSuiteEfficiencyPredictor(inputfilename="config/configEffPredictor.cfg",debug=True)

myAnaSuite.predictEff(600,5000)
