#!/bin/env python2.7

# -*- coding: utf-8 -*-

"""
    Created on Monday March 27
    
    @author: Brian L. Dorney
"""

#Imports
import sys, os
import numpy as np
from ROOT import gROOT, TCanvas, TGraph2D, TGraphErrors, TF1, TFile, TDirectory, Double

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
    #def __init__(self, gain_p0=1, gain_p0_err=1, gain_p1=1, gain_p1_err=1, lam, lam_err):
    def __init__(self, gain_p0=1, gain_p0_err=1, gain_p1=1, gain_p1_err=1):
        self.GAIN_CURVE_P0      = gain_p0
        self.GAIN_CURVE_P0_ERR  = gain_p0_err
        self.GAIN_CURVE_P1      = gain_p1
        self.GAIN_CURVE_P1_ERR  = gain_p1_err
        #self.GAIN_LAMBDA        = lam
        #self.GAIN_LAMBDA_ERR    = lam_err

        return

#Container
class PARAMS_DET:
    
    hvPoints = [650,660,670,680,690,700]  #Divider current valuves
    
    def __init__(self, sectorsize=-1, ieta=4, iphi=2, nbconnect=3, imon0=600, imonpts=hvPoints):
        self.DETPOS_IETA            = ieta          #iEta Position QC5_Gain_Cal performed in
        self.DETPOS_IPHI            = iphi          #iPhi "                                 "

        self.DETGEO_SECSIZE         = sectorsize    #Width in mm corresponding to DETPOS_IETA row
        self.DETGEO_NCONNECTORS     = nbconnect     #Number of readout conncetors in DETPOS_IETA row

        self.DET_IMON_QC5_RESP_UNI  = imon0         #Imon value QC5_Resp_Uni was performed at
        self.DET_IMON_POINTS        = imonpts       #Imon points gain map desired at
        #self.DET_ALPHA              = alpha         #Exponential factor to calculat G_ijk at some value in DET_IMON_POINTS

        return

class GainMapAnalysisSuite:

    def __init__(self, inputfilename="", params_gain=PARAMS_GAIN(), params_det=PARAMS_DET(), debug=False):

	self.ADCPKPOS_SECTOR_AVG    = 0. #Average of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
	self.ADCPKPOS_SECTOR_STDDEV = 0. #Std. Dev. of the fitted cluster ADC PkPos in defined (ieta,iphi) sector

        self.DEBUG                  = debug

        self.DETPOS_IETA            = params_det.DETPOS_IETA
        self.DETPOS_IPHI            = params_det.DETPOS_IPHI
        
        self.DETGEO_SECSIZE         = params_det.DETGEO_SECSIZE
        self.DETGEO_NCONNECTORS     = params_det.DETGEO_NCONNECTORS
        
        self.DET_IMON_QC5_RESP_UNI  = params_det.DET_IMON_QC5_RESP_UNI
        self.DET_IMON_POINTS        = []
        #self.DET_ALPHA              = 1.

        self.FILE_IN	= TFile(str(inputfilename),"READ","",1)
	self.FILE_OUT	= TFile("GainMapAnalysisSuiteOutput.root","RECREATE","",1)

        self.GAIN_CURVE_P0      = params_gain.GAIN_CURVE_P0
        self.GAIN_CURVE_P0_ERR  = params_gain.GAIN_CURVE_P0_ERR
        self.GAIN_CURVE_P1      = params_gain.GAIN_CURVE_P1
        self.GAIN_CURVE_P1_ERR  = params_gain.GAIN_CURVE_P1_ERR
        self.GAIN_LAMBDA        = 1.
        self.GAIN_LAMBDA_ERR    = 0.
	self.GAIN_AVG_POINTS	= [] #Average Gain over the entire detector
	self.GAIN_STDDEV_POINTS	= [] #Std. Dev of Gain over the entire detector

	self.G2D_MAP_ABS_RESP_UNI = TGraph2D()
	self.G2D_MAP_GAIN_ORIG = TGraph2D()

        return

    def reset(self, debug=False):

	#Close TFiles
	self.FILE_IN.Close()
	self.FILE_OUT.Close()

        return

    #Determines the Average & Std. Dev. ADC PkPos in the (DETPOS_IETA, DETPOS_IPHI) sector
    def avgROSectorADCPkPos(self):
	#Load the TFile & get the plot
        #file_Input = TFile(str(self.INPUT_FILE),"READ","",1)
        strPlotName = "g_iEta" + str(self.DETPOS_IETA) + "_clustADC_Fit_PkPos"
        gSector_clustADC_Fit_PkPos = self.FILE_IN.Get( "SectorEta" + str(self.DETPOS_IETA) + "/" + strPlotName )

	#Calculate the iphi sector boundaries
	list_sectBoundary = []
	for i in range(0, self.DETGEO_NCONNECTORS+1):
	    list_sectBoundary.append(-0.5 * self.DETGEO_SECSIZE + i * self.DETGEO_SECSIZE / self.DETGEO_NCONNECTORS)

	#Print to user - Section Boundaries
	if self.DEBUG == True:
	    for i in range(0,len(list_sectBoundary)):
	    	print list_sectBoundary[i]

	#Loop Over poins in the plot
	list_clustADC_Fit_PkPos = []
        for i in range(0, gSector_clustADC_Fit_PkPos.GetN() ):
    	    #Get the i^th point in this plot
            fPx=Double(0.0)
            fPy=Double(0.0)
            gSector_clustADC_Fit_PkPos.GetPoint(i, fPx, fPy)
        
	    #Check if this point is within the defined (ieta,iphi) sector, if so store it for later use
	    if list_sectBoundary[self.DETPOS_IPHI-1] <= fPx and fPx <= list_sectBoundary[self.DETPOS_IPHI]:
		#Print to user - selected data points
		if self.DEBUG == True:
		    print (str(i) + "\t" + str(fPx) + "\t" + str(fPy) )

		#store data point
		list_clustADC_Fit_PkPos.append(fPy)

	self.ADCPKPOS_SECTOR_AVG    = np.mean(list_clustADC_Fit_PkPos) #Average of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
	self.ADCPKPOS_SECTOR_STDDEV = np.std(list_clustADC_Fit_PkPos) #Average of the fitted cluster ADC PkPos in defined (ieta,iphi) sector

	print "Avg PkPos = " + str(self.ADCPKPOS_SECTOR_AVG) + "+/-" + str(str(self.ADCPKPOS_SECTOR_STDDEV))

	#Close the input file
	#file_Input.Close()

        return

    #alpha(x) = exp([0]*(x-x0) ) where x is hvPt and x0 is self.DET_IMON_QC5_RESP_UNI
    def calcAlpha(self, hvPt):
	return np.exp(self.GAIN_CURVE_P0 * (hvPt - self.DET_IMON_QC5_RESP_UNI) )

    #G(x) = exp([0]*x+[1]) where x is hvPt
    def calcGain(self, hvPt):
	return np.exp(self.GAIN_CURVE_P0 * hvPt + self.GAIN_CURVE_P1)

    #G(x) = exp([0]*x+[1]) where x is hvPt
    def calcGainErr(self, hvPt):
	return self.calcGain(hvPt)*np.sqrt(np.square(self.GAIN_CURVE_P0_ERR * hvPt)+np.square(self.GAIN_CURVE_P1_ERR))

    #Determines the linear correlation factor lambda which relates Gain to ADC counts
    def calcROSectorLambda(self):
	gain = self.calcGain(self.DET_IMON_QC5_RESP_UNI)
	gain_err = self.calcGainErr(self.DET_IMON_QC5_RESP_UNI)

	self.GAIN_LAMBDA = gain / self.ADCPKPOS_SECTOR_AVG
	self.GAIN_LAMBDA_ERR = ( 1. / self.ADCPKPOS_SECTOR_AVG ) * np.sqrt( np.square(gain_err) + np.square(self.ADCPKPOS_SECTOR_STDDEV * gain / self.ADCPKPOS_SECTOR_AVG) - 2. * gain_err * self.ADCPKPOS_SECTOR_STDDEV * gain / self.ADCPKPOS_SECTOR_AVG)

	print "lambda = " + str(self.GAIN_LAMBDA) + "+/-" + str(self.GAIN_LAMBDA_ERR)

	return

    #Determines the gain map from the absolute response uniformity map
    def calcGainMap(self, strDetName):
	#Open the file and get the absolute response uniformity map 
        #file_Input = TFile(str(self.INPUT_FILE),"READ","",1)
        strPlotName = "g2D_" + strDetName + "_ResponseFitPkPos_AllEta"
        self.G2D_MAP_ABS_RESP_UNI = self.FILE_IN.Get( "Summary/" + strPlotName )

	if self.DEBUG:
	    print "Attempted to Load:"
	    print "Summary/" + strPlotName

	self.G2D_MAP_GAIN_ORIG.Set( self.G2D_MAP_ABS_RESP_UNI.GetN() )
	self.G2D_MAP_GAIN_ORIG.SetName( "g2D_" + strDetName + "_EffGain_AllEta" )

	#Get the arrays that make the response uniformity map
	array_fPx = self.G2D_MAP_ABS_RESP_UNI.GetX()
	array_fPy = self.G2D_MAP_ABS_RESP_UNI.GetY()
	array_fPz = self.G2D_MAP_ABS_RESP_UNI.GetZ()

	#Loop Over all Points of self.G2D_MAP_ABS_RESP_UNI
	list_Gain_Vals = []
        for i in range(0, self.G2D_MAP_ABS_RESP_UNI.GetN() ):
	    #Set the i^th point in self.G2D_MAP_GAIN_ORIG
	    list_Gain_Vals.append( array_fPz[i] * self.GAIN_LAMBDA )
	    self.G2D_MAP_GAIN_ORIG.SetPoint(i, array_fPx[i], array_fPy[i], array_fPz[i] * self.GAIN_LAMBDA)

	#Store Average Gain and Std. Dev. Gain
	self.DET_IMON_POINTS.append(self.DET_IMON_QC5_RESP_UNI)
	self.GAIN_AVG_POINTS.append(np.mean(list_Gain_Vals) )
	self.GAIN_STDDEV_POINTS.append(np.std(list_Gain_Vals) )

	#Draw the effective gain map
	canv_Gain_Map_Orig = TCanvas("canv_" + strDetName + "_EffGain_AllEta","Gain Map - Original",600,600)
	canv_Gain_Map_Orig.cd()
	canv_Gain_Map_Orig.cd().SetLogz(1)
	self.G2D_MAP_GAIN_ORIG.Draw("TRI2Z")
	canv_Gain_Map_Orig.SetTheta(90);
	canv_Gain_Map_Orig.SetPhi(0.0);

	#Write the effective gain map to the output file
	#file_Output = TFile("GainMapAnalysisSuiteOutput.root",strFileOption,"",1)
	self.FILE_OUT.cd()
	canv_Gain_Map_Orig.Write()
	self.G2D_MAP_GAIN_ORIG.Write()

	#Close the TFiles
	#file_Output.Close()
	#file_Input.Close()

	return

    #Determines the gain map from the absolute response uniformity map for an arbitrary voltage
    def calcGainMapHV(self, strDetName, hvPt):
	#Create the new TGraph2D
	g2D_Map_Gain_hvPt = TGraph2D( self.G2D_MAP_GAIN_ORIG.GetN() )
	g2D_Map_Gain_hvPt.SetName( "g2D_" + strDetName + "_EffGain_AllEta_" + str(hvPt) )

	#Get the arrays that make the response uniformity map
	array_fPx = self.G2D_MAP_GAIN_ORIG.GetX()
	array_fPy = self.G2D_MAP_GAIN_ORIG.GetY()
	array_fPz = self.G2D_MAP_GAIN_ORIG.GetZ()
	
	#Calculate alpha
	alpha = self.calcAlpha(hvPt)

	#Loop Over all Points of self.G2D_MAP_ABS_RESP_UNI
	list_Gain_Vals = []
        for i in range(0, self.G2D_MAP_ABS_RESP_UNI.GetN() ):
	    #Set the i^th point in self.G2D_MAP_GAIN_ORIG
	    list_Gain_Vals.append( array_fPz[i] * alpha )
	    g2D_Map_Gain_hvPt.SetPoint(i, array_fPx[i], array_fPy[i], array_fPz[i] * alpha)

	#Store Average Gain and Std. Dev. Gain
	self.DET_IMON_POINTS.append(hvPt)
	self.GAIN_AVG_POINTS.append(np.mean(list_Gain_Vals) )
	self.GAIN_STDDEV_POINTS.append(np.std(list_Gain_Vals) )

	#Draw the effective gain map
	canv_Gain_Map_hvPt = TCanvas("canv_" + strDetName + "_EffGain_AllEta_" + str(hvPt),"Gain Map - hvPt = " + str(hvPt),600,600)
	canv_Gain_Map_hvPt.cd()
	canv_Gain_Map_hvPt.cd().SetLogz(1)
	g2D_Map_Gain_hvPt.Draw("TRI2Z")
	canv_Gain_Map_hvPt.SetTheta(90);
	canv_Gain_Map_hvPt.SetPhi(0.0);

	#Write the effective gain map to the output file
	#file_Output = TFile("GainMapAnalysisSuiteOutput.root",strFileOption,"",1)
	self.FILE_OUT.cd()
	canv_Gain_Map_hvPt.Write()
	g2D_Map_Gain_hvPt.Write()

	#Close the TFiles
	#file_Output.Close()

	return

    #Plot Average Gain Over Entire Detector Area
    def plotAvgGain(self, strDetName):
	#Create the Plot
	gDet_AvgEffGain = TGraphErrors( len(self.GAIN_AVG_POINTS) )
	gDet_AvgEffGain.SetName("g_" + strDetName + "_AvgEffGain")

	#Set the points
	for i in range(0, len(self.GAIN_AVG_POINTS) ):
	    gDet_AvgEffGain.SetPoint(i,self.DET_IMON_POINTS[i],self.GAIN_AVG_POINTS[i])
	    gDet_AvgEffGain.SetPointError(i,0,self.GAIN_STDDEV_POINTS[i])

	#Draw
	canv_AvgEffGain = TCanvas("canv_" + strDetName + "_AvgEffGain",strDetName + " Average Effective Gain",600,600)
	canv_AvgEffGain.cd()
	canv_AvgEffGain.cd().SetLogy()
	gDet_AvgEffGain.GetXaxis().SetTitle("HV")
	gDet_AvgEffGain.GetYaxis().SetTitle("#LT Effective Gain #GT")
	gDet_AvgEffGain.SetMarkerStyle(21)
	gDet_AvgEffGain.Draw("AP")	

	#Write
	canv_AvgEffGain.Write()
	gDet_AvgEffGain.Write()

	return

