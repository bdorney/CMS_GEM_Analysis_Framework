#!/afs/cern.ch/sw/lcg/external/Python/2.7.4/x86_64-slc6-gcc48-opt/bin/python2.7

# -*- coding: utf-8 -*-

#"""
#    Created on Tuesday March 28 10:54:01 2017
#    
#    @author: Brian L. Dorney
#
#    Example call:
#    
#        computeGainMap.py --file=$PWD/data/sliceTestAna/QC5_Resp_Uni/GE11-VII-L-CERN-0002_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_580uA_TimeCorr_DPGGeo_AnaWithFits.root --gp0=3.49545e-02 --gp0Err=1.98035e-04 --gp1=-1.40236e+01 --gp1Err=1.28383e-01 --name=GE11-VII-L-CERN-0002 --hvPoint=580 --hvlist=600,625,650,660,670,680,690,700,710,720,730 --fileMap=$PWD/config/Mapping_GE11-VII-L.cfg
#    
#"""

import sys, os

if __name__ == "__main__":

    #Import Analysis Suit
    from AnalysisSuiteGainMap import *
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

    #Check to make sure the user specified detector parameters
    if options.det_name is None:
        print "You must specify the detector name"
        print "Use option -n or --name"
        exit(1)
        pass

    #Check to make sure the user specified the HV value of the QC5_Resp_Uni measurement
    if options.hv_orig is None:
        print "You must specify the HV value the QC5_Resp_Uni measurmeent was performed at"
        print "Use option -hv or --hvPoint"
        exit(1)
        pass

    #Setup Gain Parameters
    params_gain = PARAMS_GAIN(gain_p0=options.gain_P0,
                              gain_p0_err=options.gain_P0_Err,
                              gain_p1=options.gain_P1,
                              gain_p1_err=options.gain_P1_Err)

    #Setup Detector Parameters
    params_det = PARAMS_DET(ieta=options.det_ieta,
                            iphi=options.det_iphi,
                            imon0=options.hv_orig)
    params_det.loadMapping(options.filename_Map, debug=options.debug)

    #Initialize analysis suite
    import ROOT as r
    outF = r.TFile("GainMap_%s.root"%(options.det_name),"RECREATE","",1)
    anaSuite = AnalysisSuiteGainMap(file_out=outF,
                    inputfilename=options.filename, 
				    params_gain=params_gain, 
				    params_det=params_det, 
				    debug=options.debug)

    anaSuite.ANA_UNI_GRANULARITY = 32

    #Calculate Gain & P_D maps
    anaSuite.avgROSectorADCPkPos()
    anaSuite.avgROSectorAvgClustSize()
    anaSuite.calcROSectorLambda()
    anaSuite.calcGainMap(options.det_name)

    for hvPt in options.hv_list.split(','):
        anaSuite.calcGainMapHV(options.det_name, float(hvPt) )

    anaSuite.plotGainSummary(options.det_name)
    anaSuite.plotPDSummary(options.det_name)

    #Calculate Cluster Size Map - Requires a rearrange since old test beam data was at (5,2)
    params_det.DETPOS_IETA = 5
    params_det.DETPOS_IPHI = 2

    anaSuite.setDetector(params_det)
    anaSuite.calcClusterSizeMap(options.det_name)

    #Reset Everything
    anaSuite.reset()

    print "Finished"
