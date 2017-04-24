#!/afs/cern.ch/sw/lcg/external/Python/2.7.4/x86_64-slc6-gcc48-opt/bin/python2.7

# -*- coding: utf-8 -*-

#"""
#    Created on Thursday April 20 17:21:01 2017
#    
#    @author: Brian L. Dorney
#
#    Example call:
#    
#        python2.7 python/computeClustQLandaus.py --file=$PWD/data/TestBeam2013_FNAL_ClustCharge/ClustChargeData_MPV.txt --gp0=7.04964e-03 --gp0Err=1.23211e-05 --gp1=-1.48229e+01 --gp1Err=4.09164e-02 --name=GE11-III-FIT-0001 --obsVar=MPV
#    
#"""

import sys, os

if __name__ == "__main__":

    #Import Analysis Suit
    from AnalysisOptions import *
    from AnalysisSuiteClusterCharge import *
    from Utilities import PARAMS_GAIN

    #Specific Options
    parser.add_option("--obsVar", type="string", dest="strObsVar",
		      help="Ovservable that you are interested in computing. From set {MPV, MEAN, SIGMA}",
		      metavar="strObsVar")

    #Get input options
    (options, args) = parser.parse_args()

    #Check to make sure user specified an input file name
    if options.filename is None:
        #print "options.filename = " + str(options.filename)
        print "You must specify the physical filepath of a Framework output file"
        print "Use option -f or --file"
        exit(1)
        pass
    
    #Check to make sure the user specified a gain curve
    if options.gain_P0 is None or options.gain_P1 is None:
        print "For G(x) = exp(P0 * x + P1)"
        print "P0 = " + str(options.gain_P0)
        print "P0_Err = " + str(options.gain_P0_Err)
        print "P1 = " + str(options.gain_P1)
        print "P1_Err = " + str(options.gain_P1_Err)
        print "You must specify values for P0 & P1"
        exit(1)
        pass

    #Setup Gain Parameters
    params_gain = PARAMS_GAIN(gain_p0=options.gain_P0,
                              gain_p0_err=options.gain_P0_Err,
                              gain_p1=options.gain_P1,
                              gain_p1_err=options.gain_P1_Err)

    #Initialize analysis suite
    anaSuite = AnalysisSuiteClusterCharge(params_gain, True, False)

    #Make Landau Plots
    anaSuite.makePlot(options.filename, iSkip=2, strObsName=options.strObsVar)

    #Store Landau Plots
    anaSuite.storePlots("ClusterChargeLandauPlots.root", strFileOpt="UPDATE", strObsName=options.strObsVar)

    #Interpolate Data - Note Data is already loaded
    anaSuite.interpolateData(strInterpolateKind="cubic", strObsName=options.strObsVar)
    print anaSuite.getInterpolatedData(iClustSize=2.5,fHVOrGain=5e3, strObsName=options.strObsVar)

    print "Finished"
