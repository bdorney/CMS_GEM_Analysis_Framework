#!/bin/env python2.7

# -*- coding: utf-8 -*-

"""
    Created on Monday March 27
    
    @author: Brian L. Dorney
    """

#Imports
import sys, os
import numpy as np
from ROOT import gROOT, Double, TCanvas, TDirectory, TF1, TFile, TGraph2D, TGraphErrors, TH1F, TLegend

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
        
        return

class PARAMS_PD:
    def __init__(self, const=-2.12136e+01, slope=2.49075e-05):
        self.PD_CONST = const
        self.PD_SLOPE = slope

	return

#Container
class PARAMS_DET:
    
    hvPoints = [650,660,670,680,690,700]  #Divider current valuves
    
    def __init__(self, sectorsize=-1, ieta=4, iphi=2, nbconnect=3, imon0=600):
        self.DETPOS_IETA            = ieta          #iEta Position QC5_Gain_Cal performed in
        self.DETPOS_IPHI            = iphi          #iPhi "                                 "
        
        self.DETGEO_SECSIZE         = sectorsize    #Width in mm corresponding to DETPOS_IETA row
        self.DETGEO_NCONNECTORS     = nbconnect     #Number of readout conncetors in DETPOS_IETA row
        
        self.DET_IMON_QC5_RESP_UNI  = imon0         #Imon value QC5_Resp_Uni was performed at
        
        return

class GainMapAnalysisSuite:
    
    def __init__(self, inputfilename="", params_gain=PARAMS_GAIN(), params_det=PARAMS_DET(), params_discharge=PARAMS_PD(), debug=False):
        
        self.ADCPKPOS_SECTOR_AVG    = 0. #Average of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        self.ADCPKPOS_SECTOR_STDDEV = 0. #Std. Dev. of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        
        self.DEBUG                  = debug
        
        self.DETPOS_IETA            = params_det.DETPOS_IETA
        self.DETPOS_IPHI            = params_det.DETPOS_IPHI
        
        self.DETGEO_SECSIZE         = params_det.DETGEO_SECSIZE
        self.DETGEO_NCONNECTORS     = params_det.DETGEO_NCONNECTORS
        
        self.DET_IMON_QC5_RESP_UNI  = params_det.DET_IMON_QC5_RESP_UNI
        self.DET_IMON_POINTS        = []
        
        self.FILE_IN	= TFile(str(inputfilename),"READ","",1)

        outputFileName	= inputfilename.split('/')
        outputFileName	= "GainMapAnalysisSuiteOutput_" + outputFileName[len(outputFileName)-1]
        self.FILE_OUT	= TFile(str(outputFileName),"RECREATE","",1)
        
        self.GAIN_CURVE_P0      = params_gain.GAIN_CURVE_P0
        self.GAIN_CURVE_P0_ERR  = params_gain.GAIN_CURVE_P0_ERR
        self.GAIN_CURVE_P1      = params_gain.GAIN_CURVE_P1
        self.GAIN_CURVE_P1_ERR  = params_gain.GAIN_CURVE_P1_ERR
        self.GAIN_LAMBDA        = 1.
        self.GAIN_LAMBDA_ERR    = 0.

        self.GAIN_AVG_POINTS	= [] #Average Gain over the entire detector
        self.GAIN_STDDEV_POINTS	= [] #Std. Dev of Gain over the entire detector
        self.GAIN_MAX_POINTS	= [] #Max Gain over the entire detector
        self.GAIN_MIN_POINTS	= [] #Min Gain over the entire detector 

        self.G2D_MAP_ABS_RESP_UNI = TGraph2D()
        self.G2D_MAP_GAIN_ORIG = TGraph2D()
        
        self.PD_CONST	= params_discharge.PD_CONST
        self.PD_SLOPE	= params_discharge.PD_SLOPE

        self.PD_AVG_POINTS	= [] #Avg P_D over entire detector
        self.PD_STDDEV_POINTS	= [] #Std. Dev of P_D over entire detector
        self.PD_MAX_POINTS	= [] #Max P_D over the entire detector
        self.PD_MIN_POINTS	= [] #Min P_D over the entire detector

        return
    
    def reset(self, debug=False):
        
        #Close TFiles
        self.FILE_IN.Close()
        self.FILE_OUT.Close()
        
        return
    
    #Determines the Average & Std. Dev. ADC PkPos in the (DETPOS_IETA, DETPOS_IPHI) sector
    def avgROSectorADCPkPos(self):
        #Load the plot
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
	#array_clustADC_Fit_PkPos = np.array()
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
                #array_clustADC_Fit_PkPos.append(fPy)
        
	array_clustADC_Fit_PkPos = np.array(list_clustADC_Fit_PkPos)
	array_clustADC_Fit_PkPos = self.rejectOutliers(array_clustADC_Fit_PkPos)

	if self.DEBUG:
	    print "np.mean(list_clustADC_Fit_PkPos) = " + str(np.mean(list_clustADC_Fit_PkPos))
	    print "np.mean(array_clustADC_Fit_PkPos) = " + str(np.mean(array_clustADC_Fit_PkPos)) + "\t No Outliers"

        #self.ADCPKPOS_SECTOR_AVG    = np.mean(list_clustADC_Fit_PkPos) #Average of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        #self.ADCPKPOS_SECTOR_STDDEV = np.std(list_clustADC_Fit_PkPos) #Std Dev of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        self.ADCPKPOS_SECTOR_AVG    = np.mean(array_clustADC_Fit_PkPos) #Average of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        self.ADCPKPOS_SECTOR_STDDEV = np.std(array_clustADC_Fit_PkPos) #Std Dev of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        
        print "Avg PkPos = " + str(self.ADCPKPOS_SECTOR_AVG) + "+/-" + str(str(self.ADCPKPOS_SECTOR_STDDEV))
        
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
    
    #PD(x) = exp(slope*x+Const)
    def calcPD(self, gain):
	return np.exp(self.PD_SLOPE*gain+self.PD_CONST)

    #Determines the linear correlation factor lambda which relates Gain to ADC counts
    def calcROSectorLambda(self):
        gain = self.calcGain(self.DET_IMON_QC5_RESP_UNI)
        gain_err = self.calcGainErr(self.DET_IMON_QC5_RESP_UNI)
        
        self.GAIN_LAMBDA = gain / self.ADCPKPOS_SECTOR_AVG
        self.GAIN_LAMBDA_ERR = ( 1. / self.ADCPKPOS_SECTOR_AVG ) * np.sqrt( np.square(gain_err) + np.square(self.ADCPKPOS_SECTOR_STDDEV * gain / self.ADCPKPOS_SECTOR_AVG) - 2. * gain_err * self.ADCPKPOS_SECTOR_STDDEV * gain / self.ADCPKPOS_SECTOR_AVG)
        
        print "lambda = " + str(self.GAIN_LAMBDA) + "+/-" + str(self.GAIN_LAMBDA_ERR)
        
        return
    
    #Use Median absolute deviation (MAD) to reject outliers)
    #See: http://stackoverflow.com/questions/22354094/pythonic-way-of-detecting-outliers-in-one-dimensional-observation-data
    def rejectOutliers(self, arrayData, thresh=3.5):
	if len(arrayData.shape) == 1:
	    tempData = arrayData[:,None]

	median = np.median(tempData, axis=0)

	diff = np.sum((tempData - median)**2, axis=-1)
	diff = np.sqrt(diff)

	med_abs_deviation = np.median(diff)

	modified_z_score = 0.6745 * diff / med_abs_deviation

	arrayMask = (modified_z_score < thresh) #true if points are not outliers, false if points are outliers
	arrayData = np.multiply(arrayData, arrayMask) #Now outliers are set to zero

	return arrayData[(arrayData > 0)] #Return only the non-outlier versions

    #Determines the gain map from the absolute response uniformity map
    def calcGainMap(self, strDetName):
        #Load the absolute response uniformity map
        strPlotName = "g2D_" + strDetName + "_ResponseFitPkPos_AllEta"
        self.G2D_MAP_ABS_RESP_UNI = self.FILE_IN.Get( "Summary/" + strPlotName )
        
        if self.DEBUG:
            print "Attempted to Load:"
            print "Summary/" + strPlotName
        
        self.G2D_MAP_GAIN_ORIG.Set( self.G2D_MAP_ABS_RESP_UNI.GetN() )
        self.G2D_MAP_GAIN_ORIG.SetName( "g2D_" + strDetName + "_EffGain_AllEta_" + str(int(self.DET_IMON_QC5_RESP_UNI)) )
        
        #Get the arrays that make the response uniformity map
        array_fPx = self.G2D_MAP_ABS_RESP_UNI.GetX()
        array_fPy = self.G2D_MAP_ABS_RESP_UNI.GetY()
        array_fPz = self.G2D_MAP_ABS_RESP_UNI.GetZ()
        
        #Loop Over all Points of self.G2D_MAP_ABS_RESP_UNI
        #list_Gain_Vals = []
        #list_PD_Vals = []
        array_Gain_Vals = np.zeros(self.G2D_MAP_ABS_RESP_UNI.GetN())
        array_PD_Vals = np.zeros(self.G2D_MAP_ABS_RESP_UNI.GetN())
        for i in range(0, self.G2D_MAP_ABS_RESP_UNI.GetN() ):
            #Set the i^th point in self.G2D_MAP_GAIN_ORIG
            #list_Gain_Vals.append( array_fPz[i] * self.GAIN_LAMBDA )
            #list_PD_Vals.append( self.calcPD(array_fPz[i] * self.GAIN_LAMBDA) )
            array_Gain_Vals[i] = array_fPz[i] * self.GAIN_LAMBDA
            array_PD_Vals[i] = self.calcPD(array_fPz[i] * self.GAIN_LAMBDA) 
            self.G2D_MAP_GAIN_ORIG.SetPoint(i, array_fPx[i], array_fPy[i], array_fPz[i] * self.GAIN_LAMBDA)
    
	#array_Gain_Vals = np.array(list_Gain_Vals)

	#print "len(array_Gain_Vals) = " + str(len(array_Gain_Vals) )
	#print "len(self.rejectOutliers(array_Gain_Vals)) = " + str(len(self.rejectOutliers(array_Gain_Vals)))

	#print "np.min(array_Gain_Vals) = " + str(np.min(array_Gain_Vals) )
	#print "np.min(self.rejectOutliers(array_Gain_Vals)) = " + str(np.min(self.rejectOutliers(array_Gain_Vals)))

	#print "np.mean(array_Gain_Vals) = " + str(np.mean(array_Gain_Vals) )
	#print "np.mean(self.rejectOutliers(array_Gain_Vals)) = " + str(np.mean(self.rejectOutliers(array_Gain_Vals)))

	#print "np.max(array_Gain_Vals) = " + str(np.max(array_Gain_Vals) )
	#print "np.max(self.rejectOutliers(array_Gain_Vals)) = " + str(np.max(self.rejectOutliers(array_Gain_Vals)))

        #Store Average, Std. Dev., Max, & Min Gain
	array_Gain_Vals = self.rejectOutliers(array_Gain_Vals)
        self.DET_IMON_POINTS.append(self.DET_IMON_QC5_RESP_UNI)
        #self.GAIN_AVG_POINTS.append(np.mean(list_Gain_Vals) )
        #self.GAIN_STDDEV_POINTS.append(np.std(list_Gain_Vals) )
        #self.GAIN_MAX_POINTS.append(np.max(list_Gain_Vals) )
        #self.GAIN_MIN_POINTS.append(np.min(list_Gain_Vals) )
        self.GAIN_AVG_POINTS.append(np.mean(array_Gain_Vals) )
        self.GAIN_STDDEV_POINTS.append(np.std(array_Gain_Vals) )
        self.GAIN_MAX_POINTS.append(np.max(array_Gain_Vals) )
        self.GAIN_MIN_POINTS.append(np.min(array_Gain_Vals) )
            
        #Store Average, Std. Dev., Max & Min P_D
	array_PD_Vals = self.rejectOutliers(array_PD_Vals)
        #self.PD_AVG_POINTS.append(np.mean(list_PD_Vals) )
        #self.PD_STDDEV_POINTS.append(np.std(list_PD_Vals) )
        #self.PD_MAX_POINTS.append(np.max(list_PD_Vals) )
        #self.PD_MIN_POINTS.append(np.min(list_PD_Vals) )
        self.PD_AVG_POINTS.append(np.mean(array_PD_Vals) )
        self.PD_STDDEV_POINTS.append(np.std(array_PD_Vals) )
        self.PD_MAX_POINTS.append(np.max(array_PD_Vals) )
        self.PD_MIN_POINTS.append(np.min(array_PD_Vals) )

        #Draw the effective gain map
        canv_Gain_Map_Orig = TCanvas("canv_" + strDetName + "_EffGain_AllEta_" + str(int(self.DET_IMON_QC5_RESP_UNI)),"Gain Map - Original " + str(self.DET_IMON_QC5_RESP_UNI),600,600)
        canv_Gain_Map_Orig.cd()
        canv_Gain_Map_Orig.cd().SetLogz(1)
        self.G2D_MAP_GAIN_ORIG.Draw("TRI2Z")
        #canv_Gain_Map_Orig.SetTheta(90);
        #canv_Gain_Map_Orig.SetPhi(0.0);
        
        #Write the effective gain map to the output file
        #self.FILE_OUT.cd()
        dir_hvOrig = self.FILE_OUT.mkdir( "GainMap_HVPt" + str(int(self.DET_IMON_QC5_RESP_UNI)) )
        dir_hvOrig.cd()                
        canv_Gain_Map_Orig.Write()
        self.G2D_MAP_GAIN_ORIG.Write()

        return

    #Determines the gain map from the absolute response uniformity map for an arbitrary voltage
    def calcGainMapHV(self, strDetName, hvPt):
        #Create the new TGraph2D - Gain
        g2D_Map_Gain_hvPt = TGraph2D( self.G2D_MAP_GAIN_ORIG.GetN() )
        g2D_Map_Gain_hvPt.SetName( "g2D_" + strDetName + "_EffGain_AllEta_" + str(int(hvPt)) )

        #Create the new TGraph2D - Discharge Probability
        g2D_Map_PD_hvPt = TGraph2D( self.G2D_MAP_GAIN_ORIG.GetN() )
        g2D_Map_PD_hvPt.SetName( "g2D_" + strDetName + "_PD_AllEta_" + str(int(hvPt)) )

        #Get the arrays that make the response uniformity map
        array_fPx = self.G2D_MAP_GAIN_ORIG.GetX()
        array_fPy = self.G2D_MAP_GAIN_ORIG.GetY()
        array_fPz = self.G2D_MAP_GAIN_ORIG.GetZ()
        
        #Calculate alpha
        alpha = self.calcAlpha(hvPt)
        
        #Loop Over all Points of self.G2D_MAP_ABS_RESP_UNI
        #list_Gain_Vals = []
        #list_PD_Vals = []
        array_Gain_Vals = np.zeros(self.G2D_MAP_ABS_RESP_UNI.GetN())
        array_PD_Vals = np.zeros(self.G2D_MAP_ABS_RESP_UNI.GetN())
        for i in range(0, self.G2D_MAP_ABS_RESP_UNI.GetN() ):
            #Set the i^th point in self.G2D_MAP_GAIN_ORIG
            #list_Gain_Vals.append( array_fPz[i] * alpha )
            #list_PD_Vals.append( self.calcPD(array_fPz[i] * alpha) )
            array_Gain_Vals[i] = array_fPz[i] * alpha 
            array_PD_Vals[i] = self.calcPD(array_fPz[i] * alpha)
            g2D_Map_Gain_hvPt.SetPoint(i, array_fPx[i], array_fPy[i], array_fPz[i] * alpha)
            g2D_Map_PD_hvPt.SetPoint(i, array_fPx[i], array_fPy[i], self.calcPD(array_fPz[i] * alpha) )
    
        #Store Average, Std. Dev., Max, & Min Gain
	array_Gain_Vals = self.rejectOutliers(array_Gain_Vals)
        self.DET_IMON_POINTS.append(hvPt)
        #self.GAIN_AVG_POINTS.append(np.mean(list_Gain_Vals) )
        #self.GAIN_STDDEV_POINTS.append(np.std(list_Gain_Vals) )
        #self.GAIN_MAX_POINTS.append(np.max(list_Gain_Vals) )
        #self.GAIN_MIN_POINTS.append(np.min(list_Gain_Vals) )
        self.GAIN_AVG_POINTS.append(np.mean(array_Gain_Vals) )
        self.GAIN_STDDEV_POINTS.append(np.std(array_Gain_Vals) )
        self.GAIN_MAX_POINTS.append(np.max(array_Gain_Vals) )
        self.GAIN_MIN_POINTS.append(np.min(array_Gain_Vals) )
            
        #Store Average, Std. Dev., Max & Min P_D
	array_PD_Vals = self.rejectOutliers(array_PD_Vals)
        #self.PD_AVG_POINTS.append(np.mean(list_PD_Vals) )
        #self.PD_STDDEV_POINTS.append(np.std(list_PD_Vals) )
        #self.PD_MAX_POINTS.append(np.max(list_PD_Vals) )
        #self.PD_MIN_POINTS.append(np.min(list_PD_Vals) )
        self.PD_AVG_POINTS.append(np.mean(array_PD_Vals) )
        self.PD_STDDEV_POINTS.append(np.std(array_PD_Vals) )
        self.PD_MAX_POINTS.append(np.max(array_PD_Vals) )
        self.PD_MIN_POINTS.append(np.min(array_PD_Vals) )

        #Draw the effective gain map
        canv_Gain_Map_hvPt = TCanvas("canv_" + strDetName + "_EffGain_AllEta_" + str(int(hvPt)),"Gain Map - hvPt = " + str(hvPt),600,600)
        canv_Gain_Map_hvPt.cd()
        canv_Gain_Map_hvPt.cd().SetLogz(1)
        g2D_Map_Gain_hvPt.Draw("TRI2Z")
        #canv_Gain_Map_hvPt.SetTheta(90);
        #canv_Gain_Map_hvPt.SetPhi(0.0);

        #Draw the discharge probability map
        canv_PD_Map_hvPt = TCanvas("canv_" + strDetName + "_PD_AllEta_" + str(int(hvPt)),"Discharge Probability Map - hvPt = " + str(hvPt),600,600)
        canv_PD_Map_hvPt.cd()
        canv_PD_Map_hvPt.cd().SetLogz(1)
        g2D_Map_PD_hvPt.Draw("TRI2Z")
        #canv_PD_Map_hvPt.SetTheta(90);
        #canv_PD_Map_hvPt.SetPhi(0.0);
        
        #Write the effective gain map to the output file
        dir_hvPt = self.FILE_OUT.mkdir( "GainMap_HVPt" + str(int(hvPt)) )
        dir_hvPt.cd()        
        canv_Gain_Map_hvPt.Write()
        g2D_Map_Gain_hvPt.Write()
        canv_PD_Map_hvPt.Write()
        g2D_Map_PD_hvPt.Write()
            
        return

    #Plot Average Gain Over Entire Detector Area
    def plotGainSummary(self, strDetName):
        #Create the Plot - Average
        gDet_AvgEffGain = TGraphErrors( len(self.GAIN_AVG_POINTS) )
        gDet_AvgEffGain.SetName("g_" + strDetName + "_EffGainAvg")
        
        #Create the Plot - Max Gain
        gDet_MaxEffGain = TGraphErrors( len(self.GAIN_MAX_POINTS) )
        gDet_MaxEffGain.SetName("g_" + strDetName + "_EffGainMax")	

        #Create the Plot - Min Gain
        gDet_MinEffGain = TGraphErrors( len(self.GAIN_MIN_POINTS) )
        gDet_MinEffGain.SetName("g_" + strDetName + "_EffGainMin")	

        #Set the points
        for i in range(0, len(self.GAIN_AVG_POINTS) ):
            #Average
            gDet_AvgEffGain.SetPoint(i,self.DET_IMON_POINTS[i],self.GAIN_AVG_POINTS[i])
            gDet_AvgEffGain.SetPointError(i,0,self.GAIN_STDDEV_POINTS[i])

            #Max
            gDet_MaxEffGain.SetPoint(i,self.DET_IMON_POINTS[i],self.GAIN_MAX_POINTS[i])

            #Min
            gDet_MinEffGain.SetPoint(i,self.DET_IMON_POINTS[i],self.GAIN_MIN_POINTS[i])
        
        #Draw
        canv_AvgEffGain = TCanvas("canv_" + strDetName + "_EffGainAvg",strDetName + " Average Effective Gain",600,600)
        canv_AvgEffGain.cd()
        canv_AvgEffGain.cd().SetLogy()
        gDet_AvgEffGain.GetXaxis().SetTitle("HV")
        gDet_AvgEffGain.GetYaxis().SetTitle("#LT Effective Gain #GT")
        gDet_AvgEffGain.GetYaxis().SetRangeUser(1e2,1e6)
        gDet_AvgEffGain.SetMarkerStyle(21)
        gDet_AvgEffGain.Draw("AP")
        gDet_MaxEffGain.Draw("sameL")
        gDet_MinEffGain.Draw("sameL")

        #Write
        dir_Summary = self.FILE_OUT.mkdir("Summary")
        dir_Summary.cd()
        canv_AvgEffGain.Write()
        gDet_AvgEffGain.Write()
        gDet_MaxEffGain.Write()
        gDet_MinEffGain.Write()
        
    	return

    #Plot Average Gain Over Entire Detector Area
    def plotPDSummary(self, strDetName):
        #Create the Plot - Average
        gDet_AvgPD = TGraphErrors( len(self.PD_AVG_POINTS) )
        gDet_AvgPD.SetName("g_" + strDetName + "_PDAvg")
        
        #Create the Plot - Max Gain
        gDet_MaxPD = TGraphErrors( len(self.PD_MAX_POINTS) )
        gDet_MaxPD.SetName("g_" + strDetName + "_PDMax")	

        #Create the Plot - Min Gain
        gDet_MinPD = TGraphErrors( len(self.PD_MIN_POINTS) )
        gDet_MinPD.SetName("g_" + strDetName + "_PDMin")

        #Set the points
        for i in range(0, len(self.PD_AVG_POINTS) ):
            #Average
            gDet_AvgPD.SetPoint(i,self.GAIN_AVG_POINTS[i],self.PD_AVG_POINTS[i])
            gDet_AvgPD.SetPointError(i,self.GAIN_STDDEV_POINTS[i],self.PD_STDDEV_POINTS[i])

            #Max
            gDet_MaxPD.SetPoint(i,self.GAIN_AVG_POINTS[i],self.PD_MAX_POINTS[i])

            #Min
            gDet_MinPD.SetPoint(i,self.GAIN_AVG_POINTS[i],self.PD_MIN_POINTS[i])
        
        #Draw
        canv_AvgPD = TCanvas("canv_" + strDetName + "_PDAvg",strDetName + " Discharge Probability",600,600)
        canv_AvgPD.cd()
        canv_AvgPD.cd().SetLogx()
        canv_AvgPD.cd().SetLogy()
        gDet_AvgPD.GetXaxis().SetTitle("#LT Effective Gain #GT")
        gDet_AvgPD.GetYaxis().SetTitle("Discharge Probability P_{D}")
        gDet_AvgPD.GetYaxis().SetRangeUser(1e-11,1e-6)
        gDet_AvgPD.SetMarkerStyle(21)
        gDet_AvgPD.Draw("AP")
        gDet_MaxPD.Draw("sameL")
        gDet_MinPD.Draw("sameL")

        #Write
        dir_Summary = self.FILE_OUT.GetDirectory("Summary")
        dir_Summary.cd()
        canv_AvgPD.Write()
        gDet_AvgPD.Write()
        gDet_MaxPD.Write()
        gDet_MinPD.Write()
        
    	return
