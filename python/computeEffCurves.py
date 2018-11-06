#!/afs/cern.ch/sw/lcg/external/Python/2.7.4/x86_64-slc6-gcc48-opt/bin/python2.7

# -*- coding: utf-8 -*-

#"""
#    Created on Thursday May 11 21:30:00 2017
#    
#    @author: Brian L. Dorney
#
#    Example call:
#    
#        computeEffCurves.py --file=$PWD/config/configEffPredictor.cfg --deubg=True
#    
#"""

import sys, os

if __name__ == "__main__":
    #Import Analysis Suit
    from AnalysisSuiteEfficiencyPredictor import *
    from AnalysisOptions import *

    #Get input options
    (options, args) = parser.parse_args()

    #Check to make sure user specified an input file name
    if options.filename is None:
        #print "options.filename = " + str(options.filename)
        print "You must specify the physical filepath of a Framework output file"
        print "Use option -f or --file"
        exit(1)
        pass

    anaSuite = AnalysisSuiteEfficiencyPredictor(inputfilename=options.filename, debug=options.debug)
    anaSuite.predictEffCurves()

    print "Finished"
