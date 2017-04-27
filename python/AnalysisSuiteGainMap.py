#!/bin/env python2.7

# -*- coding: utf-8 -*-

"""
    Created on Monday March 27
    
    @author: Brian L. Dorney
"""

#Imports
import sys, os
import numpy as np
from Utilities import *
from ROOT import gROOT, Double, TCanvas, TDirectory, TF1, TFile, TGraph2D, TGraphErrors, TH1F, TH2F, TLegend

class AnalysisSuiteGainMap:
    
    def __init__(self, inputfilename="", outputfilename="AnaSuiteGainMapOutput.root", outputfileoption="RECREATE", params_gain=PARAMS_GAIN(), params_det=PARAMS_DET(), params_discharge=PARAMS_PD(), debug=False):
        
        self.ADCPKPOS_SECTOR_AVG    = 0. #Average of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        self.ADCPKPOS_SECTOR_STDDEV = 0. #Std. Dev. of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        
        self.ANA_UNI_GRANULARITY    = 32
        
        self.AVGCLUSTSIZE_SECTOR_AVG    = 0. #Average of Average Cluster Size distributions in defined (ieta,iphi) sector
        self.AVGCLUSTSIZE_SECTOR_STDDEV = 0. #Std. Dev. of Average Cluster Size distributions in defined (ieta,iphi) sector
        
        self.DEBUG                  = debug
        
        self.DETECTOR               = params_det
                
        self.DET_IMON_QC5_RESP_UNI  = params_det.DET_IMON_QC5_RESP_UNI
        self.DET_IMON_POINTS        = []
        
        self.FILE_IN = []
        if len(inputfilename) > 0:
            self.FILE_IN	= TFile(str(inputfilename),"READ","",1)

        if len(outputfilename) == 0:
            outputfilename	= inputfilename.split('/')
            outputfilename	= "AnaSuiteGainMapOutput_" + outputfilename[len(outputFileName)-1]

        self.FILE_OUT	= TFile(str(outputfilename),outputfileoption,"",1)
        
        self.GAIN_CALCULATOR    = params_gain
        self.GAIN_LAMBDA        = 1.
        self.GAIN_LAMBDA_ERR    = 0.

        self.GAIN_AVG_POINTS	= [] #Average Gain over the entire detector
        self.GAIN_STDDEV_POINTS	= [] #Std. Dev of Gain over the entire detector
        self.GAIN_MAX_POINTS	= [] #Max Gain over the entire detector
        self.GAIN_MIN_POINTS	= [] #Min Gain over the entire detector

        self.G2D_MAP_ABS_RESP_UNI = TGraph2D()          #Absolute Response Uniformity Map
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG = TGraph2D()   #Absolute Avg Cluster Size Map
        self.G2D_MAP_AVG_CLUST_SIZE_NORM = TGraph2D()   #Normalized "                   "
        self.G2D_MAP_GAIN_ORIG = TGraph2D()             #Effective Gain Map
        
        self.PD_CALCULATOR      = params_discharge

        self.PD_AVG_POINTS      = [] #Avg P_D over entire detector
        self.PD_STDDEV_POINTS	= [] #Std. Dev of P_D over entire detector
        self.PD_MAX_POINTS      = [] #Max P_D over the entire detector
        self.PD_MIN_POINTS      = [] #Min P_D over the entire detector

        return
    
    def reset(self, debug=False):        
        #Close TFiles
        self.closeTFiles(debug)
        
        #Reset Variables
        self.DEBUG = debug
        
        self.ADCPKPOS_SECTOR_AVG    = 0.
        self.ADCPKPOS_SECTOR_STDDEV = 0.
        
        self.ANA_UNI_GRANULARITY    = 32
        
        self.AVGCLUSTSIZE_SECTOR_AVG    = 0.
        self.AVGCLUSTSIZE_SECTOR_STDDEV = 0.

        self.DET_IMON_QC5_RESP_UNI  = 0.
    
        self.GAIN_LAMBDA        = 1.
        self.GAIN_LAMBDA_ERR    = 0.
        
        #Reset classes
        self.DETECTOR.reset()
        #self.GAIN_CALCULATOR
        #self.PD_CALCULATOR
        
        #Clear Lists
        del self.DET_IMON_POINTS[:]
        
        del self.GAIN_AVG_POINTS[:]
        del self.GAIN_STDDEV_POINTS[:]
        del self.GAIN_MAX_POINTS[:]
        del self.GAIN_MIN_POINTS[:]
        
        del self.PD_AVG_POINTS[:]
        del self.PD_STDDEV_POINTS[:]
        del self.PD_MAX_POINTS[:]
        del self.PD_MIN_POINTS[:]
        
        #Clear TObjects?
        #self.G2D_MAP_ABS_RESP_UNI
        #self.G2D_MAP_AVG_CLUST_SIZE_ORIG
        #self.G2D_MAP_AVG_CLUST_SIZE_NORM
        #self.G2D_MAP_GAIN_ORIG
        
        return
    
    #Determines the Average & Std. Dev. ADC PkPos in the (DETPOS_IETA, DETPOS_IPHI) sector
    def avgROSectorADCPkPos(self):
        #Load the plot
        strPlotName = "g_iEta" + str(self.DETECTOR.DETPOS_IETA) + "_clustADC_Fit_PkPos"
        gSector_clustADC_Fit_PkPos = self.FILE_IN.Get( "SectorEta" + str(self.DETECTOR.DETPOS_IETA) + "/" + strPlotName )
        
        #Calculate the iphi sector boundaries
        list_sectBoundary = self.DETECTOR.calcROSectorBoundariesByEta(self.DETECTOR.DETPOS_IETA)
        
        #Print to user - Section Boundaries
        if self.DEBUG == True:
            for i in range(0,len(list_sectBoundary)):
                print list_sectBoundary[i]
        
        #Loop over points in the plot
        list_clustADC_Fit_PkPos = []
        for i in range(0, gSector_clustADC_Fit_PkPos.GetN() ):
            #Get the i^th point in this plot
            fPx=Double(0.0)
            fPy=Double(0.0)
            gSector_clustADC_Fit_PkPos.GetPoint(i, fPx, fPy)
            
            #Check if this point is within the defined (ieta,iphi) sector, if so store it for later use
            if list_sectBoundary[self.DETECTOR.DETPOS_IPHI-1] <= fPx and fPx <= list_sectBoundary[self.DETECTOR.DETPOS_IPHI]:
                #Print to user - selected data points
                if self.DEBUG == True:
                    print (str(i) + "\t" + str(fPx) + "\t" + str(fPy) )
                
                #store data point
                list_clustADC_Fit_PkPos.append(fPy)
        
        #Store this list as a numpy array and then remove all outliers
        array_clustADC_Fit_PkPos = np.array(list_clustADC_Fit_PkPos)
        array_clustADC_Fit_PkPos = rejectOutliers(array_clustADC_Fit_PkPos)

        if self.DEBUG:
            print "np.mean(list_clustADC_Fit_PkPos) = " + str(np.mean(list_clustADC_Fit_PkPos))
            print "np.mean(array_clustADC_Fit_PkPos) = " + str(np.mean(array_clustADC_Fit_PkPos)) + "\t No Outliers"

        #Calculate Average
        self.ADCPKPOS_SECTOR_AVG    = np.mean(array_clustADC_Fit_PkPos) #Average of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        self.ADCPKPOS_SECTOR_STDDEV = np.std(array_clustADC_Fit_PkPos) #Std Dev of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        
        print "Avg PkPos = " + str(self.ADCPKPOS_SECTOR_AVG) + "+/-" + str(self.ADCPKPOS_SECTOR_STDDEV)
        
        return
    
    #Determine the average of the average cluster sizes within a single readout sector
    def avgROSectorAvgClustSize(self):
        #Load the plot
        strPlotName = "h_iEta" + str(self.DETECTOR.DETPOS_IETA) + "_clustSize_v_clustPos"
        hSector_clustSize_v_clustPos = self.FILE_IN.Get( "SectorEta" + str(self.DETECTOR.DETPOS_IETA) + "/" + strPlotName )
        
        #Calculate the iphi sector boundaries
        #list_sectBoundary = self.calcROSectorBoundaries(self.LIST_DET_GEO_PARAMS[self.DETECTOR.DETPOS_IETA-1])
        list_sectBoundary = self.DETECTOR.calcROSectorBoundariesByEta(self.DETECTOR.DETPOS_IETA)
        
        #Print to user - Section Boundaries
        if self.DEBUG == True:
            for i in range(0,len(list_sectBoundary)):
                print list_sectBoundary[i]

        #Loop over points in the plot
        list_avgClustSize = []
        for i in range(1, hSector_clustSize_v_clustPos.GetNbinsX() +1):
            fBinCenter = hSector_clustSize_v_clustPos.GetXaxis().GetBinCenter(i)
        
            #Check if this point is within the defined (ieta,iphi) sector, if so store it for later use
            if list_sectBoundary[self.DETECTOR.DETPOS_IPHI-1] <= fBinCenter and fBinCenter <= list_sectBoundary[self.DETECTOR.DETPOS_IPHI]:
                
                #Project out cluster size distribution for *this* slice
                strPlotName = "h_iEta" + str(self.DETECTOR.DETPOS_IETA) + "Slice" + str(i) + "_clustSize"
                h_clustSize = hSector_clustSize_v_clustPos.ProjectionY(strPlotName, i, i, "")
                
                fAvgClustSize = h_clustSize.GetMean()
        
                #store data point
                list_avgClustSize.append(fAvgClustSize)
                
                #Print to user - selected data points
                if self.DEBUG == True:
                    print (str(i) + "\t" + str(fBinCenter) + "\t" + str(fAvgClustSize) )
        
        #Store this list as a numpy array and then remove all outliers
        array_avgClustSize = np.array(list_avgClustSize)
        array_avgClustSize = rejectOutliers(array_avgClustSize)

        if self.DEBUG:
            print "np.mean(list_avgClustSize) = " + str(np.mean(list_avgClustSize))
            print "np.mean(array_avgClustSize) = " + str(np.mean(array_avgClustSize)) + "\t No Outliers"

        #Calculate Average
        self.AVGCLUSTSIZE_SECTOR_AVG    = np.mean(array_avgClustSize) #Average of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        self.AVGCLUSTSIZE_SECTOR_STDDEV = np.std(array_avgClustSize) #Std. Dev. of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        
        print "Avg of Avg Clust Size = " + str(self.AVGCLUSTSIZE_SECTOR_AVG) + "+/-" + str(self.AVGCLUSTSIZE_SECTOR_STDDEV)
        
        return
    
    #alpha(x) = exp([0]*(x-x0) ) where x is hvPt and x0 is self.DET_IMON_QC5_RESP_UNI
    def calcAlpha(self, hvPt):
        return np.exp(self.GAIN_CALCULATOR.GAIN_CURVE_P0 * (hvPt - self.DETECTOR.DET_IMON_QC5_RESP_UNI) )
    
    #Determines the linear correlation factor lambda which relates Gain to ADC counts
    def calcROSectorLambda(self):
        gain = self.GAIN_CALCULATOR.calcGain(self.DET_IMON_QC5_RESP_UNI)
        gain_err = self.GAIN_CALCULATOR.calcGainErr(self.DET_IMON_QC5_RESP_UNI)
        
        self.GAIN_LAMBDA = gain / self.ADCPKPOS_SECTOR_AVG
        self.GAIN_LAMBDA_ERR = ( 1. / self.ADCPKPOS_SECTOR_AVG ) * np.sqrt( np.square(gain_err) + np.square(self.ADCPKPOS_SECTOR_STDDEV * gain / self.ADCPKPOS_SECTOR_AVG) - 2. * gain_err * self.ADCPKPOS_SECTOR_STDDEV * gain / self.ADCPKPOS_SECTOR_AVG)
        
        print "lambda = " + str(self.GAIN_LAMBDA) + "+/-" + str(self.GAIN_LAMBDA_ERR)
        
        return

    #Determines the gain map from the absolute response uniformity map
    def calcGainMap(self, strDetName):
        if self.DEBUG:
            print "Attempted to Load:"
            print "Summary/" + strPlotName
        
        #Load the absolute response uniformity map
        strPlotName = "g2D_" + strDetName + "_ResponseFitPkPos_AllEta"
        self.G2D_MAP_ABS_RESP_UNI = self.FILE_IN.Get( "Summary/" + strPlotName )
        
        #Setup the gain map
        self.G2D_MAP_GAIN_ORIG.Set( self.G2D_MAP_ABS_RESP_UNI.GetN() )
        self.G2D_MAP_GAIN_ORIG.SetName( "g2D_" + strDetName + "_EffGain_AllEta_" + str(int(self.DET_IMON_QC5_RESP_UNI)) )
        
        #Get the arrays that make the response uniformity map
        array_fPx = self.G2D_MAP_ABS_RESP_UNI.GetX()
        array_fPy = self.G2D_MAP_ABS_RESP_UNI.GetY()
        array_fPz = self.G2D_MAP_ABS_RESP_UNI.GetZ()
        
        #Loop Over all Points of self.G2D_MAP_ABS_RESP_UNI
        array_Gain_Vals = np.zeros(self.G2D_MAP_ABS_RESP_UNI.GetN())
        array_PD_Vals = np.zeros(self.G2D_MAP_ABS_RESP_UNI.GetN())
        for i in range(0, self.G2D_MAP_ABS_RESP_UNI.GetN() ):
            #Set the i^th point in self.G2D_MAP_GAIN_ORIG
            array_Gain_Vals[i] = array_fPz[i] * self.GAIN_LAMBDA
            array_PD_Vals[i] = self.PD_CALCULATOR.calcPD(array_fPz[i] * self.GAIN_LAMBDA) 
            self.G2D_MAP_GAIN_ORIG.SetPoint(i, array_fPx[i], array_fPy[i], array_fPz[i] * self.GAIN_LAMBDA)
    
        #Store Average, Std. Dev., Max, & Min Gain
        array_Gain_Vals = rejectOutliers(array_Gain_Vals)
        self.DET_IMON_POINTS.append(self.DET_IMON_QC5_RESP_UNI)
        self.GAIN_AVG_POINTS.append(np.mean(array_Gain_Vals) )
        self.GAIN_STDDEV_POINTS.append(np.std(array_Gain_Vals) )
        self.GAIN_MAX_POINTS.append(np.max(array_Gain_Vals) )
        self.GAIN_MIN_POINTS.append(np.min(array_Gain_Vals) )
            
        #Store Average, Std. Dev., Max & Min P_D
        array_PD_Vals = rejectOutliers(array_PD_Vals)
        self.PD_AVG_POINTS.append(np.mean(array_PD_Vals) )
        self.PD_STDDEV_POINTS.append(np.std(array_PD_Vals) )
        self.PD_MAX_POINTS.append(np.max(array_PD_Vals) )
        self.PD_MIN_POINTS.append(np.min(array_PD_Vals) )

        #Draw the effective gain map
        canv_Gain_Map_Orig = TCanvas("canv_" + strDetName + "_EffGain_AllEta_" + str(int(self.DET_IMON_QC5_RESP_UNI)),"Gain Map - Original " + str(self.DET_IMON_QC5_RESP_UNI),600,600)
        canv_Gain_Map_Orig.cd()
        canv_Gain_Map_Orig.cd().SetLogz(1)
        self.G2D_MAP_GAIN_ORIG.Draw("TRI2Z")
        
        #Write the effective gain map to the output file
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
        array_Gain_Vals = np.zeros(self.G2D_MAP_ABS_RESP_UNI.GetN())
        array_PD_Vals = np.zeros(self.G2D_MAP_ABS_RESP_UNI.GetN())
        for i in range(0, self.G2D_MAP_ABS_RESP_UNI.GetN() ):
            #Set the i^th point in self.G2D_MAP_GAIN_ORIG
            array_Gain_Vals[i] = array_fPz[i] * alpha
            array_PD_Vals[i] = self.PD_CALCULATOR.calcPD(array_fPz[i] * alpha)
            g2D_Map_Gain_hvPt.SetPoint(i, array_fPx[i], array_fPy[i], array_fPz[i] * alpha)
            g2D_Map_PD_hvPt.SetPoint(i, array_fPx[i], array_fPy[i], self.PD_CALCULATOR.calcPD(array_fPz[i] * alpha) )
    
        #Store Average, Std. Dev., Max, & Min Gain
        array_Gain_Vals = rejectOutliers(array_Gain_Vals)
        self.DET_IMON_POINTS.append(hvPt)
        self.GAIN_AVG_POINTS.append(np.mean(array_Gain_Vals) )
        self.GAIN_STDDEV_POINTS.append(np.std(array_Gain_Vals) )
        self.GAIN_MAX_POINTS.append(np.max(array_Gain_Vals) )
        self.GAIN_MIN_POINTS.append(np.min(array_Gain_Vals) )
            
        #Store Average, Std. Dev., Max & Min P_D
        array_PD_Vals = rejectOutliers(array_PD_Vals)
        self.PD_AVG_POINTS.append(np.mean(array_PD_Vals) )
        self.PD_STDDEV_POINTS.append(np.std(array_PD_Vals) )
        self.PD_MAX_POINTS.append(np.max(array_PD_Vals) )
        self.PD_MIN_POINTS.append(np.min(array_PD_Vals) )

        #Draw the effective gain map
        canv_Gain_Map_hvPt = TCanvas("canv_" + strDetName + "_EffGain_AllEta_" + str(int(hvPt)),"Gain Map - hvPt = " + str(hvPt),600,600)
        canv_Gain_Map_hvPt.cd()
        canv_Gain_Map_hvPt.cd().SetLogz(1)
        g2D_Map_Gain_hvPt.Draw("TRI2Z")

        #Draw the discharge probability map
        canv_PD_Map_hvPt = TCanvas("canv_" + strDetName + "_PD_AllEta_" + str(int(hvPt)),"Discharge Probability Map - hvPt = " + str(hvPt),600,600)
        canv_PD_Map_hvPt.cd()
        canv_PD_Map_hvPt.cd().SetLogz(1)
        g2D_Map_PD_hvPt.Draw("TRI2Z")
        
        #Write the effective gain map to the output file
        dir_hvPt = self.FILE_OUT.mkdir( "GainMap_HVPt" + str(int(hvPt)) )
        dir_hvPt.cd()        
        canv_Gain_Map_hvPt.Write()
        g2D_Map_Gain_hvPt.Write()
        canv_PD_Map_hvPt.Write()
        g2D_Map_PD_hvPt.Write()
            
        return g2D_Map_Gain_hvPt

    #Determines the average cluster size map for the entire detector
    def calcClusterSizeMap(self, strDetName):
        #Create the container which will store the clusterSize
        #array_shape = ( self.G2D_MAP_ABS_RESP_UNI.GetN(), 3)   #Not gauranteed to work since some points are thrown out during the fitting process in the C++ class AnalyzeResponseUniformityClusters
        iNEtaSectors = len(self.DETECTOR.LIST_DET_GEO_PARAMS)
        iNBinNum = self.ANA_UNI_GRANULARITY * iNEtaSectors * self.DETECTOR.LIST_DET_GEO_PARAMS[0].NBCONNECT
        array_shape = (iNBinNum, 3)
        array_clustSize = np.zeros(array_shape)
        
        #Create the average cluster size map
        strPlotName = "g2D_" + strDetName + "_AvgClustSize_AllEta_" + str(int(self.DET_IMON_QC5_RESP_UNI))
        #self.G2D_MAP_AVG_CLUST_SIZE_ORIG.Set( self.G2D_MAP_ABS_RESP_UNI.GetN() )    #Set number of pts
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG.Set( iNBinNum ) #Set number of pts, see comments above
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG.SetName( strPlotName )
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG.SetTitle("")

        #Create the average cluster size map
        strPlotName = "g2D_" + strDetName + "_AvgClustSizeNormalized_AllEta_" + str(int(self.DET_IMON_QC5_RESP_UNI))
        #self.G2D_MAP_AVG_CLUST_SIZE_NORM.Set( self.G2D_MAP_ABS_RESP_UNI.GetN() )    #Set number of pts
        self.G2D_MAP_AVG_CLUST_SIZE_NORM.Set( iNBinNum ) #Set number of pts, see comments above
        self.G2D_MAP_AVG_CLUST_SIZE_NORM.SetName( strPlotName )
        self.G2D_MAP_AVG_CLUST_SIZE_NORM.SetTitle("")

        for iEta in range(1, iNEtaSectors+1):
            #Get the Eta Sector
            etaSector = self.DETECTOR.LIST_DET_GEO_PARAMS[iEta-1]

            #Load the cluster size vs cluster position plot for this iEta value
            strPlotName = "SectorEta" + str(iEta) + "/h_iEta" + str(iEta) + "_clustSize_v_clustPos"
            
            if self.DEBUG:
                print "Attempted to Load:"
                print strPlotName

            h_clustSize_v_clustPos = self.FILE_IN.Get( strPlotName )

            #Loop over the x-bins of this plot
            for iSlice in range(1, h_clustSize_v_clustPos.GetNbinsX() + 1):
                #Project out cluster size distribution for *this* slice
                strPlotName = "h_iEta" + str(iEta) + "Slice" + str(iSlice) + "_clustSize"
                h_clustSize = h_clustSize_v_clustPos.ProjectionY(strPlotName, iSlice, iSlice, "")
    
                #Store average cluster size, y-position and x-position
                array_clustSize[ (iEta-1) * h_clustSize_v_clustPos.GetNbinsX() + iSlice-1 ] = (h_clustSize_v_clustPos.GetXaxis().GetBinCenter(iSlice), etaSector.SECTPOS, h_clustSize.GetMean() )
                
                #Set this point in the plot - Absolute
                self.G2D_MAP_AVG_CLUST_SIZE_ORIG.SetPoint( (iEta-1) * h_clustSize_v_clustPos.GetNbinsX() + iSlice-1, h_clustSize_v_clustPos.GetXaxis().GetBinCenter(iSlice), etaSector.SECTPOS, h_clustSize.GetMean() )
    
                #Set this point in the plot - Normalized
                self.G2D_MAP_AVG_CLUST_SIZE_NORM.SetPoint( (iEta-1) * h_clustSize_v_clustPos.GetNbinsX() + iSlice-1, h_clustSize_v_clustPos.GetXaxis().GetBinCenter(iSlice), etaSector.SECTPOS, h_clustSize.GetMean() / self.AVGCLUSTSIZE_SECTOR_AVG )
    
        #Print the cluster map to the user if requested
        if self.DEBUG:
            print "Average Cluster Size Map:"
            print array_clustSize

        #Draw the average cluster size map - Absolute
        canv_AvgClustSize_Map_Orig = TCanvas("canv_" + strDetName + "_AvgClustSize_AllEta_" + str(int(self.DET_IMON_QC5_RESP_UNI)),"Average Cluster Size Map - Original " + str(self.DET_IMON_QC5_RESP_UNI),600,600)
        canv_AvgClustSize_Map_Orig.cd()
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG.Draw("TRI2Z")
        
        #Draw the average cluster size map - Normalized
        canv_AvgClustSize_Map_Norm = TCanvas("canv_" + strDetName + "_AvgClustSizeNormalized_AllEta_" + str(int(self.DETECTOR.DET_IMON_QC5_RESP_UNI)),"Average Cluster Size Map - Normalized " + str(self.DETECTOR.DET_IMON_QC5_RESP_UNI),600,600)
        canv_AvgClustSize_Map_Norm.cd()
        self.G2D_MAP_AVG_CLUST_SIZE_NORM.Draw("TRI2Z")
        
        #Write the average cluster size map to the output file
        dir_hvOrig = self.FILE_OUT.GetDirectory( "GainMap_HVPt" + str(int(self.DETECTOR.DET_IMON_QC5_RESP_UNI)), False, "GetDirectory" )
        dir_hvOrig.cd()
        canv_AvgClustSize_Map_Orig.Write()
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG.Write()
        canv_AvgClustSize_Map_Norm.Write()
        self.G2D_MAP_AVG_CLUST_SIZE_NORM.Write()

        return

    #Closes TFiles
    def closeTFiles(self, debug=False):
        if self.FILE_IN.IsOpen():
            self.FILE_IN.Close()

        if self.FILE_OUT.IsOpen():
            self.FILE_OUT.Close()

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

    #Open Input File
    def openInputFile(self, inputfilename):
        self.FILE_IN	= TFile(str(inputfilename),"READ","",1)

        return

    #Set the detector
    def setDetector(self, params_det=PARAMS_DET()):
        self.DETECTOR = params_det

        return
