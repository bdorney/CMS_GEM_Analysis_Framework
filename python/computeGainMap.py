#!/bin/env python2.7

# -*- coding: utf-8 -*-

"""
    Created on Tuesday March 28 10:54:01 2017
    
    @author: Brian L. Dorney
"""

import sys, os

if __name__ == "__main__":

    #Import Analysis Suit
    from GainMapAnalysisSuite import *

    params_gain = PARAMS_GAIN(gain_p0=3.49545e-02,
                              gain_p0_err=1.98035e-04,
                              gain_p1=-1.40236e+01,
                              gain_p1_err=1.28383e-01)

    hvPoints = [650,660,670,680,690,700]  #Divider current valuves

    params_det = PARAMS_DET(ieta=4,
                            iphi=2,
                            sectorsize=319.48,
                            nbconnect=3,
                            imon=600,
                            imonpts=hvPoints)

    #filePath = "/afs/cern.ch/user/d/dorney/scratch0/CMS_GEM/CMS_GEM_Analysis_Framework/data/sliceTestAna/GE11-VII-L-CERN-0002_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_580uA_TimeCorr_DPGGeo_AnaWithFits.root"
    filePath = "/Users/dorney/Desktop/MyResearch/GitRepos/CMS_GEM_Analysis_Framework/data/sliceTestAna/GE11-VII-L-CERN-0002_Summary_Physics_Optimized_RandTrig_XRay40kV100uA_580uA_TimeCorr_DPGGeo_AnaWithFits.root"

    anaSuite = GainMapAnalysisSuite(filePath, params_gain, params_det, debug=True)
