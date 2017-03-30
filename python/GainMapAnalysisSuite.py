#!/bin/env python2.7

# -*- coding: utf-8 -*-

"""
    Created on Monday March 27
    
    @author: Brian L. Dorney
"""

#Imports
import sys, os
from ROOT import gROOT, TGraphErrors, TF1, TFile, TDirectory

#Container for storing gain parameters
#Gain is defined as:
#
#   G(x) = exp([0]*x+[1]) where x is Divider Current in uA
#
#The factor lambda is used such that:
#
#   G_ijk = lambda * ADCPkPos_ijk
#
class PARAMS_GAIN:
    def __init__(self, gain_p0=1, gain_p0_err=1, gain_p1=1, gain_p1_err=1, lam, lam_err):
        self.GAIN_CURVE_P0      = gain_p0
        self.GAIN_CURVE_P0_ERR  = gain_p0_err
        self.GAIN_CURVE_P1      = gain_p1
        self.GAIN_CURVE_P1_ERR  = gain_p1_err
        #self.GAIN_LAMBDA        = lam
        #self.GAIN_LAMBDA_ERR    = lam_err

        return

#Container
class PARAMS_DET:
    
    #hvPoints = [650,660,670,680,690,700]  #Divider current valuves
    
    def __init__(self, ieta=4, iphi=2, sectorsize, nbconnect=3, imon0=600, imonpts):
        self.DETPOS_IETA            = ieta          #iEta Position QC5_Gain_Cal performed in
        self.DETPOS_IPHI            = iphi          #iPhi "                                 "

        self.DETGEO_SECSIZE         = sectorsize    #Width in mm corresponding to DETPOS_IETA row
        self.DETGEO_NCONNECTORS     = nbconnect     #Number of readout conncetors in DETPOS_IETA row

        self.DET_IMON_QC5_RESP_UNI  = imon0         #Imon value QC5_Resp_Uni was performed at
        self.DET_IMON_POINTS        = imonpts       #Imon points gain map desired at
        #self.DET_ALPHA              = alpha         #Exponential factor to calculat G_ijk at some value in DET_IMON_POINTS

    return

class GainMapAnalysisSuite:

    def __init__(self, inputfilename="", params_gain, params_det, debug=False):

        self.INPUT_FILE = inputfilename

        self.GAIN_CURVE_P0      = params_gain.GAIN_CURVE_P0
        self.GAIN_CURVE_P0_ERR  = params_gain.GAIN_CURVE_P0_ERR
        self.GAIN_CURVE_P1      = params_gain.GAIN_CURVE_P1
        self.GAIN_CURVE_P1_ERR  = params_gain.GAIN_CURVE_P1_ERR
        self.GAIN_LAMBDA        = 1.
        self.GAIN_LAMBDA_ERR    = 0.

        self.DETPOS_IETA            = params_det.DETPOS_IETA
        self.DETPOS_IPHI            = params_det.DETPOS_IPHI
        
        self.DETGEO_SECSIZE         = params_det.DETPOS_SECSIZE
        self.DETGEO_NCONNECTORS     = params_det.DETGEO_NCONNECTORS
        
        self.DET_IMON_QC5_RESP_UNI  = params_det.DET_IMON_QC5_RESP_UNI
        self.DET_IMON_POINTS        = params_det.DET_IMON_POINTS
        self.DET_ALPHA              = 1.

        self.DEBUG                  = debug

        return

    def reset(self, debug=False):

        #Placeholder

        return

    def avgROSectorADCPkPos(self):
        file_Input = TFile(str(self.INPUT_FILE),"READ","",1)

        strPlotName = "g_iEta" + str(self.DETPOS_IETA) + "_clustADC_Fit_PkPos"

        gSector_clustADC_Fit_PkPos = file_Input.Get( "SectorEta" + str(self.DETPOS_IETA) + "/" + strPlotName )

        for i in range(0, gSector_clustADC_Fit_PkPos.GetN() ):
    
            fPx=0
            fPy=0
        
            gSector_clustADC_Fit_PkPos.GetPoint(i, fPx, fPy)
        
            print (i+ "\t" + fPx + "\t" + fPy)

        return

