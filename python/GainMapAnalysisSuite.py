#!/bin/env python2.7

# -*- coding: utf-8 -*-

"""
    Created on Monday March 27
    
    @author: Brian L. Dorney
    """

#Imports
import sys, os
import numpy as np
from ROOT import gROOT, Double, TCanvas, TDirectory, TF1, TFile, TGraph2D, TGraphErrors, TH1F, TH2F, TLegend

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

class PARAMS_GEO:
    def __init__(self):
        self.IETA       = -1
        self.SECTPOS    = 0.0
        self.SECTSIZE   = 0.0
        self.NBCONNECT  = 3

class PARAMS_PD:
    def __init__(self, const=-2.12136e+01, slope=2.49075e-05):
        self.PD_CONST = const
        self.PD_SLOPE = slope

	return

#Container
class PARAMS_DET:
    
    #hvPoints = [650,660,670,680,690,700]  #Divider current valuves
    
    #def __init__(self, sectorsize=-1, ieta=4, iphi=2, nbconnect=3, imon0=600):
    def __init__(self, ieta=4, iphi=2, imon0=600):
        self.DETPOS_IETA            = ieta          #iEta Position QC5_Gain_Cal performed in
        self.DETPOS_IPHI            = iphi          #iPhi "                                 "
        
        #self.DETGEO_SECSIZE         = sectorsize    #Width in mm corresponding to DETPOS_IETA row
        #self.DETGEO_NCONNECTORS     = nbconnect     #Number of readout conncetors in DETPOS_IETA row

        self.LIST_DET_GEO_PARAMS    = []

        self.DET_IMON_QC5_RESP_UNI  = imon0         #Imon value QC5_Resp_Uni was performed at
        
        return

    #Load the mapping information
    def loadMapping(self, inputfilename, debug=False):
        #Open the mapping file
        file_Mapping = open(inputfilename, 'r')

        #Loop over the file
        for line in file_Mapping:
            #Skip commented lines
            if line[0] == "#":
                continue
        
            #Segement line by removing white space and segmenting by commas
            line.replace(" ","")
            list_sectParams = line.split(",")
        
            if list_sectParams[0] == "DET":
                detgeo = PARAMS_GEO()
                detgeo.IETA       = int(    list_sectParams[4].replace("CMSSECTOR","") )
                detgeo.SECTPOS    = float(  list_sectParams[5] )
                detgeo.SECTSIZE   = float(  list_sectParams[6] )
                detgeo.NBCONNECT  = int(  list_sectParams[7] )
                
                self.LIST_DET_GEO_PARAMS.append(detgeo)
            else:
                continue
        
        #Print the geometry to the user
        if debug:
            print "Loaded Mapping:"
            for idx in range(0,len(self.LIST_DET_GEO_PARAMS)):
                strLine = str(idx) + "\t" + str(self.LIST_DET_GEO_PARAMS[idx].IETA) + "\t"
                strLine = strLine + str(self.LIST_DET_GEO_PARAMS[idx].IETA) + "\t"
                strLine = strLine + str(self.LIST_DET_GEO_PARAMS[idx].SECTPOS) + "\t"
                strLine = strLine + str(self.LIST_DET_GEO_PARAMS[idx].SECTSIZE) + "\t"
                strLine = strLine + str(self.LIST_DET_GEO_PARAMS[idx].NBCONNECT) + "\n"
                
                print strLine
        
        #sectIdx = next((x for x in range(0, len(self.LIST_DET_GEO_PARAMS) ) if self.LIST_DET_GEO_PARAMS[x].IETA == self.DETPOS_IETA ), -1 )

        #print self.LIST_DET_GEO_PARAMS[sectIdx].SECTSIZE
        #print sectIdx

        return

class GainMapAnalysisSuite:
    
    def __init__(self, inputfilename="", params_gain=PARAMS_GAIN(), params_det=PARAMS_DET(), params_discharge=PARAMS_PD(), debug=False):
        
        self.ADCPKPOS_SECTOR_AVG    = 0. #Average of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        self.ADCPKPOS_SECTOR_STDDEV = 0. #Std. Dev. of the fitted cluster ADC PkPos in defined (ieta,iphi) sector
        
        self.ANA_UNI_GRANULARITY    = 32
        
        self.AVGCLUSTSIZE_SECTOR_AVG    = 0. #Average of Average Cluster Size distributions in defined (ieta,iphi) sector
        self.AVGCLUSTSIZE_SECTOR_STDDEV = 0. #Std. Dev. of Average Cluster Size distributions in defined (ieta,iphi) sector
        
        self.DEBUG                  = debug
        
        self.DETPOS_IETA            = params_det.DETPOS_IETA
        self.DETPOS_IPHI            = params_det.DETPOS_IPHI
        
        self.DETGEO_NETASECTORS     = len(params_det.LIST_DET_GEO_PARAMS)
        
        self.LIST_DET_GEO_PARAMS    = params_det.LIST_DET_GEO_PARAMS
        
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

        self.G2D_MAP_ABS_RESP_UNI = TGraph2D()          #Absolute Response Uniformity Map
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG = TGraph2D()   #Absolute Avg Cluster Size Map
        self.G2D_MAP_AVG_CLUST_SIZE_NORM = TGraph2D()   #Normalized "                   "
        self.G2D_MAP_GAIN_ORIG = TGraph2D()             #Effective Gain Map
        
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
        #list_sectBoundary = []
        #for i in range(0, self.LIST_DET_GEO_PARAMS[self.DETPOS_IETA-1].NBCONNECT+1):
        #    list_sectBoundary.append(-0.5 * self.LIST_DET_GEO_PARAMS[self.DETPOS_IETA-1].SECTSIZE + i * self.LIST_DET_GEO_PARAMS[self.DETPOS_IETA-1].SECTSIZE / self.LIST_DET_GEO_PARAMS[self.DETPOS_IETA-1].NBCONNECT)
        list_sectBoundary = calcROSectorBoundaries(self.LIST_DET_GEO_PARAMS[self.DETPOS_IETA-1])
        
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
            if list_sectBoundary[self.DETPOS_IPHI-1] <= fPx and fPx <= list_sectBoundary[self.DETPOS_IPHI]:
                #Print to user - selected data points
                if self.DEBUG == True:
                    print (str(i) + "\t" + str(fPx) + "\t" + str(fPy) )
                
                #store data point
                list_clustADC_Fit_PkPos.append(fPy)
        
        #Store this list as a numpy array and then remove all outliers
        array_clustADC_Fit_PkPos = np.array(list_clustADC_Fit_PkPos)
        array_clustADC_Fit_PkPos = self.rejectOutliers(array_clustADC_Fit_PkPos)

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
        strPlotName = "h_iEta" + str(self.DETPOS_IETA) + "_clustSize_v_clustPos"
        hSector_clustSize_v_clustPos = self.FILE_IN.Get( "SectorEta" + str(self.DETPOS_IETA) + "/" + strPlotName )
        
        #Calculate the iphi sector boundaries
        list_sectBoundary = calcROSectorBoundaries(self.LIST_DET_GEO_PARAMS[self.DETPOS_IETA-1])

        #Print to user - Section Boundaries
        if self.DEBUG == True:
            for i in range(0,len(list_sectBoundary)):
                print list_sectBoundary[i]

        #Loop over points in the plot
        list_avgClustSize = []
        for i in range(1, hSector_clustSize_v_clustPos.GetNbinsX() +1):
            fBinCenter = hSector_clustSize_v_clustPos.GetXaxis().GetBinCenter(i)
        
            #Check if this point is within the defined (ieta,iphi) sector, if so store it for later use
            if list_sectBoundary[self.DETPOS_IPHI-1] <= fBinCenter and fBinCenter <= list_sectBoundary[self.DETPOS_IPHI]:
                
                #Project out cluster size distribution for *this* slice
                strPlotName = "h_iEta" + str(self.DETPOS_IETA) + "Slice" + str(i) + "_clustSize"
                h_clustSize = h_clustSize_v_clustPos.ProjectionY(strPlotName, i, i, "")
                
                fAvgClustSize = h_clustSize.GetMean()
        
                #store data point
                list_avgClustSize.append(fAvgClustSize)
                
                #Print to user - selected data points
                if self.DEBUG == True:
                    print (str(i) + "\t" + str(fBinCenter) + "\t" + str(fAvgClustSize) )
        
        #Store this list as a numpy array and then remove all outliers
        array_avgClustSize = np.array(list_avgClustSize)
        array_avgClustSize = self.rejectOutliers(array_avgClustSize)

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

    #Determines the Phi boundaries within an Eta Sector
    def calcROSectorBoundaries(self, params_geo=PARAMS_GEO()):
        #Calculate the iphi sector boundaries
        list_boundaries = []
        for i in range(0, params_geo.NBCONNECT+1):
            list_sectBoundary.append(-0.5 * params_geo.SECTSIZE + i * params_geo.SECTSIZE / params_geo.NBCONNECT)
    
        return list_boundaries
    
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
    #And also: http://www.itl.nist.gov/div898/handbook/eda/section3/eda35h.htm
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
            array_PD_Vals[i] = self.calcPD(array_fPz[i] * self.GAIN_LAMBDA) 
            self.G2D_MAP_GAIN_ORIG.SetPoint(i, array_fPx[i], array_fPy[i], array_fPz[i] * self.GAIN_LAMBDA)
    
        #Store Average, Std. Dev., Max, & Min Gain
        array_Gain_Vals = self.rejectOutliers(array_Gain_Vals)
        self.DET_IMON_POINTS.append(self.DET_IMON_QC5_RESP_UNI)
        self.GAIN_AVG_POINTS.append(np.mean(array_Gain_Vals) )
        self.GAIN_STDDEV_POINTS.append(np.std(array_Gain_Vals) )
        self.GAIN_MAX_POINTS.append(np.max(array_Gain_Vals) )
        self.GAIN_MIN_POINTS.append(np.min(array_Gain_Vals) )
            
        #Store Average, Std. Dev., Max & Min P_D
        array_PD_Vals = self.rejectOutliers(array_PD_Vals)
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
            array_PD_Vals[i] = self.calcPD(array_fPz[i] * alpha)
            g2D_Map_Gain_hvPt.SetPoint(i, array_fPx[i], array_fPy[i], array_fPz[i] * alpha)
            g2D_Map_PD_hvPt.SetPoint(i, array_fPx[i], array_fPy[i], self.calcPD(array_fPz[i] * alpha) )
    
        #Store Average, Std. Dev., Max, & Min Gain
        array_Gain_Vals = self.rejectOutliers(array_Gain_Vals)
        self.DET_IMON_POINTS.append(hvPt)
        self.GAIN_AVG_POINTS.append(np.mean(array_Gain_Vals) )
        self.GAIN_STDDEV_POINTS.append(np.std(array_Gain_Vals) )
        self.GAIN_MAX_POINTS.append(np.max(array_Gain_Vals) )
        self.GAIN_MIN_POINTS.append(np.min(array_Gain_Vals) )
            
        #Store Average, Std. Dev., Max & Min P_D
        array_PD_Vals = self.rejectOutliers(array_PD_Vals)
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

    #Determines the average cluster size map for the entire detector
    def calcClusterSizeMap(self, strDetName):
        #Create the container which will store the clusterSize
        #array_shape = ( self.G2D_MAP_ABS_RESP_UNI.GetN(), 3)   #Not gauranteed to work since some points are thrown out during the fitting process in the C++ class AnalyzeResponseUniformityClusters
        iNBinNum = self.ANA_UNI_GRANULARITY * self.DETGEO_NETASECTORS * self.LIST_DET_GEO_PARAMS[0].NBCONNECT
        array_shape = (iNBinNum, 3)
        array_clustSize = np.zeros(array_shape)
        
        #Create the average cluster size map
        strPlotName = "g2D_" + strDetName + "_AvgClustSize_AllEta"
        #self.G2D_MAP_AVG_CLUST_SIZE_ORIG.Set( self.G2D_MAP_ABS_RESP_UNI.GetN() )    #Set number of pts
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG.Set( iNBinNum ) #Set number of pts, see comments above
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG.SetName( strPlotName )
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG.SetTitle("")

        #Create the average cluster size map
        strPlotName = "g2D_" + strDetName + "_AvgClustSizeNormalized_AllEta"
        #self.G2D_MAP_AVG_CLUST_SIZE_NORM.Set( self.G2D_MAP_ABS_RESP_UNI.GetN() )    #Set number of pts
        self.G2D_MAP_AVG_CLUST_SIZE_NORM.Set( iNBinNum ) #Set number of pts, see comments above
        self.G2D_MAP_AVG_CLUST_SIZE_NORM.SetName( strPlotName )
        self.G2D_MAP_AVG_CLUST_SIZE_NORM.SetTitle("")

        for iEta in range(1, self.DETGEO_NETASECTORS+1):
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
                array_clustSize[ (iEta-1) * h_clustSize_v_clustPos.GetNbinsX() + iSlice-1 ] = (h_clustSize_v_clustPos.GetXaxis().GetBinCenter(iSlice), self.LIST_DET_GEO_PARAMS[iEta-1].SECTPOS, h_clustSize.GetMean() )
                
                #Set this point in the plot - Absolute
                self.G2D_MAP_AVG_CLUST_SIZE_ORIG.SetPoint( (iEta-1) * h_clustSize_v_clustPos.GetNbinsX() + iSlice-1, h_clustSize_v_clustPos.GetXaxis().GetBinCenter(iSlice), self.LIST_DET_GEO_PARAMS[iEta-1].SECTPOS, h_clustSize.GetMean() )
    
                #Set this point in the plot - Normalized
                self.G2D_MAP_AVG_CLUST_SIZE_NORM.SetPoint( (iEta-1) * h_clustSize_v_clustPos.GetNbinsX() + iSlice-1, h_clustSize_v_clustPos.GetXaxis().GetBinCenter(iSlice), self.LIST_DET_GEO_PARAMS[iEta-1].SECTPOS, h_clustSize.GetMean() / self.AVGCLUSTSIZE_SECTOR_AVG )
    
        #Print the cluster map to the user if requested
        if self.DEBUG:
            print "Average Cluster Size Map:"
            print array_clustSize

        #Draw the average cluster size map - Absolute
        canv_AvgClustSize_Map_Orig = TCanvas("canv_" + strDetName + "_AvgClustSize_AllEta_" + str(int(self.DET_IMON_QC5_RESP_UNI)),"Average Cluster Size Map - Original " + str(self.DET_IMON_QC5_RESP_UNI),600,600)
        canv_AvgClustSize_Map_Orig.cd()
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG.Draw("TRI2Z")
        
        #Draw the average cluster size map - Normalized
        canv_AvgClustSize_Map_Norm = TCanvas("canv_" + strDetName + "_AvgClustSizeNormalized_AllEta_" + str(int(self.DET_IMON_QC5_RESP_UNI)),"Average Cluster Size Map - Normalized " + str(self.DET_IMON_QC5_RESP_UNI),600,600)
        canv_AvgClustSize_Map_Norm.cd()
        self.G2D_MAP_AVG_CLUST_SIZE_NORM.Draw("TRI2Z")
        
        #Write the average cluster size map to the output file
        dir_hvOrig = self.FILE_OUT.GetDirectory( "GainMap_HVPt" + str(int(self.DET_IMON_QC5_RESP_UNI)), False, "GetDirectory" )
        dir_hvOrig.cd()
        canv_AvgClustSize_Map_Orig.Write()
        self.G2D_MAP_AVG_CLUST_SIZE_ORIG.Write()
        canv_AvgClustSize_Map_Norm.Write()
        self.G2D_MAP_AVG_CLUST_SIZE_NORM.Write()

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
