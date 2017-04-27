
# -*- coding: utf-8 -*-

"""
    Created on Thursday April 20
    
    @author: Brian L. Dorney
"""

import sys, os
import numpy as np
from scipy import interpolate
from Utilities import *
from ROOT import gROOT, TCanvas, TDirectory, TFile, TGraph2D

#The amount of charge collected on the readout of a GEM detector
#when irradiated by MIPs follows a Landau distribution
#This class is designed to use experimentally measured data to predict
#The Landau behaviors (MPV, Mean, and Sigma) for a given (gain, clustsize) pair
class AnalysisSuiteClusterCharge:
    def __init__(self, params_gain=PARAMS_GAIN(), calcGain=False, debug=False ):
        self.DEBUG          = debug
        self.TRANSFORM2GAIN = calcGain

        self.ARRAY_HVORGAIN     = np.zeros(0) #Array of HV Points or Gain Cluster Charge Measurements were taken as
        self.ARRAY_CLUSTSIZE	= np.zeros(0) #Array of Cluster Charge Measurements were taken as
        self.ARRAY_CLUSTQ_MPV	= np.zeros(0) #Array of MIP Cluster Charge Landua MPV, shape will be (col x row) = ( len(self.ARRAY_CLUSTSIZE) x len(self.ARRAY_HVPTS) )
        self.ARRAY_CLUSTQ_MEAN	= np.zeros(0) #Array of MIP Cluster Charge Landau Mean, shape as above
        self.ARRAY_CLUSTQ_SIGMA	= np.zeros(0) #Array of MIP Cluster Charge Landua Sigma, shape as above

        self.INTERPOLATE_MPV    = [] #Interpolator for Cluster Charge MPV null for now, will be a interpolate.interp2d() later
        self.INTERPOLATE_MEAN   = [] #As above but for Cluster Charge Mean
        self.INTERPOLATE_SIGMA  = [] #As anove but for Cluster Charge Sigma

        self.GAIN_CALCULATOR    = params_gain

        #self.G2D_CLUSTQ_MPV     = TGraph2D() #Cluster Charge - MPV;   (x,y,z) = (HV or Gain, Clust Size, MPV   in fC)
        #self.G2D_CLUSTQ_MEAN	= TGraph2D() #Cluster Charge - Mean;  (x,y,z) = (HV or Gain, Clust Size, Mean  in fC)
        #self.G2D_CLUSTQ_SIGMA	= TGraph2D() #Cluster Charge - Sigma; (x,y,z) = (HV or Gain, Clust Size, Sigma in fC)

        self.STROBSNAME_ALL     = "ALL"
        self.STROBSNAME_MPV     = "MPV"
        self.STROBSNAME_MEAN	= "MEAN"
        self.STROBSNAME_SIGMA	= "SIGMA"

        return

    #Check if a given coordinate (iClustSize, fHVOrGain) is inside the data to be interpolated
    #   Returns true if coordinate point is inside the range
    #   Returns false if else
    def checkIfDataPtInRange(self, iClustSize, fHVOrGain):
        #Check to make sure (iClustSize, fHVOrGain) is inside the expected range
        if iClustSize < self.ARRAY_CLUSTSIZE.min() or iClustSize > self.ARRAY_CLUSTSIZE.max():
            if self.DEBUG:
                print "Cluster size input: " + str(iClustSize)
                print "Is outside given range: [" + str(self.ARRAY_CLUSTSIZE.min()) + "," + str(self.ARRAY_CLUSTSIZE.max()) + "]"
                print "Calculation may not be reliable"

            return False
        elif fHVOrGain < self.ARRAY_HVORGAIN.min() or fHVOrGain > self.ARRAY_HVORGAIN.max():
            if self.DEBUG:
                print "HV or Gain input: " + str(iClustSize)
                print "Is outside given range: [" + str(self.ARRAY_HVORGAIN.min()) + "," + str(self.ARRAY_HVORGAIN.max()) + "]"
                print "Calculation may not be reliable"
                    
            return False
        else:
            return True

    #Given a coordinate (iClustSize, fHVOrGain), return the Cluster Charge Values corresponding to strObsName
    #	strObsName	->	Observable to plot, acceptable inputs are from set {MPV, MEAN, SIGMA}
    def getInterpolatedData(self, iClustSize, fHVOrGain, strObsName="MPV"):
        if self.checkIfDataPtInRange(iClustSize, fHVOrGain):
            if strObsName == self.STROBSNAME_MEAN:
                return self.getInterpolatedMean(iClustSize, fHVOrGain)
            elif strObsName == self.STROBSNAME_MPV:
                return self.getInterpolatedMPV(iClustSize, fHVOrGain)
            elif strObsName == self.STROBSNAME_SIGMA:
                return self.getInterpolatedSigma(iClustSize, fHVOrGain)
            else:
                print "Input Observable Name: " + strObsName
                print "Was not recognized, please cross-check and re-run"
                print "Exiting"
                return -1
        else:
            return -1

    #Given a coordinate (iClustSize, fHVOrGain), return the Cluster Charge Landau MPV
    def getInterpolatedMean(self, iClustSize, fHVOrGain):
        if self.checkIfDataPtInRange(iClustSize, fHVOrGain):
            return self.INTERPOLATE_MEAN(iClustSize, fHVOrGain)
        else:
            return -1

    #Given a coordinate (iClustSize, fHVOrGain), return the Cluster Charge Landau MPV
    def getInterpolatedMPV(self, iClustSize, fHVOrGain):
        if self.checkIfDataPtInRange(iClustSize, fHVOrGain):
            return self.INTERPOLATE_MPV(iClustSize, fHVOrGain)
        else:
            return -1

    #Given a coordinate (iClustSize, fHVOrGain), return the Cluster Charge Landau MPV
    def getInterpolatedSigma(self, iClustSize, fHVOrGain):
        if self.checkIfDataPtInRange(iClustSize, fHVOrGain):
            return self.INTERPOLATE_SIGMA(iClustSize, fHVOrGain)
        else:
            return -1

    #Here:
    #   stepSize_HVOrGain   -> Step size in HV or Gain coordinate to perform the interpolation with
    #   stepSize_ClustSize  -> As above but for Cluster Size
    #   strInterpolateKind  -> From the set  {‘linear’, ‘cubic’, ‘quintic’}
    #	strObsName	->	Observable to plot, acceptable inputs are from set {ALL, MPV, MEAN, SIGMA}
    #def interpolateData(self, stepSize_HVOrGain, stepSize_ClustSize=0.25, strInterpolateKind="cubic"):
    def interpolateData(self, strInterpolateKind="cubic", strObsName="ALL"):
        #Transform input observable name to uppercase
        strObsName = strObsName.upper()
        
        if strObsName == self.STROBSNAME_ALL:
            self.interpolateMean(strInterpolateKind)
            self.interpolateMPV(strInterpolateKind)
            self.interpolateSigma(strInterpolateKind)
        elif strObsName == self.STROBSNAME_MEAN:
            self.interpolateMean(strInterpolateKind)
        elif strObsName == self.STROBSNAME_MPV:
            self.interpolateMPV(strInterpolateKind)
        elif strObsName == self.STROBSNAME_SIGMA:
            self.interpolateSigma(strInterpolateKind)
        else:
            print "Input Observable Name: " + strObsName
            print "Was not recognized, please cross-check and re-run"
            print "Exiting"
            return

        return

    #Here:
    #   stepSize_HVOrGain   -> Step size in HV or Gain coordinate to perform the interpolation with
    #   stepSize_ClustSize  -> As above but for Cluster Size
    #   strInterpolateKind  -> From the set  {‘linear’, ‘cubic’, ‘quintic’}
    #def interpolateMean(self, stepSize_HVOrGain, stepSize_ClustSize=0.25, strInterpolateKind="cubic"):
    def interpolateMean(self, strInterpolateKind="cubic"):
        #Check to make sure the data is present
        if len(self.ARRAY_CLUSTQ_MEAN)==0:
            print "Cluster Charge Mean Data has not been loaded"
            print "Please load data and then try again"
            print "Exiting"
            
            return
        
        #Interpolate the data - Mean
        if self.DEBUG:
            print "Interpolating Mean"
                
        self.INTERPOLATE_MEAN = interpolate.interp2d(self.ARRAY_CLUSTSIZE,
                                                     self.ARRAY_HVORGAIN,
                                                     self.ARRAY_CLUSTQ_MEAN,
                                                     kind=strInterpolateKind)
                                                            
        return

    #Here:
    #   stepSize_HVOrGain   -> Step size in HV or Gain coordinate to perform the interpolation with
    #   stepSize_ClustSize  -> As above but for Cluster Size
    #   strInterpolateKind  -> From the set  {‘linear’, ‘cubic’, ‘quintic’}
    #def interpolateMPV(self, stepSize_HVOrGain, stepSize_ClustSize=0.25, strInterpolateKind="cubic"):
    def interpolateMPV(self, strInterpolateKind="cubic"):
        #Check to make sure the data is present
        if len(self.ARRAY_CLUSTQ_MPV)==0:
            print "Cluster Charge MPV Data has not been loaded"
            print "Please load data and then try again"
            print "Exiting"
                
            return
            
        #Interpolate the data - MPV
        if self.DEBUG:
            print "Interpolating MPV"
        
        self.INTERPOLATE_MPV = interpolate.interp2d(self.ARRAY_CLUSTSIZE,
                                                     self.ARRAY_HVORGAIN,
                                                     self.ARRAY_CLUSTQ_MPV,
                                                     kind=strInterpolateKind)
            
        return

    #Here:
    #   stepSize_HVOrGain   -> Step size in HV or Gain coordinate to perform the interpolation with
    #   stepSize_ClustSize  -> As above but for Cluster Size
    #   strInterpolateKind  -> From the set  {‘linear’, ‘cubic’, ‘quintic’}
    #def interpolateSigma(self, stepSize_HVOrGain, stepSize_ClustSize=0.25, strInterpolateKind="cubic"):
    def interpolateSigma(self, strInterpolateKind="cubic"):
        #Check to make sure the data is present
        if len(self.ARRAY_CLUSTQ_SIGMA)==0:
            print "Cluster Charge Sigma Data has not been loaded"
            print "Please load data and then try again"
            print "Exiting"
                    
            return
                
        #Interpolate the data - Sigma
        if self.DEBUG:
            print "Interpolating Sigma"
    
        self.INTERPOLATE_SIGMA = interpolate.interp2d(self.ARRAY_CLUSTSIZE,
                                                self.ARRAY_HVORGAIN,
                                                self.ARRAY_CLUSTQ_SIGMA,
                                                kind=strInterpolateKind)
        
        return

    #Load data from file
    #Note X & Y coordinate points are expected to be identical, only Z values should change
    def loadData(self,  inputfilename_MEAN="data/TestBeam2013_FNAL_ClustCharge/ClustChargeData_Mean.txt",
                        inputfilename_MPV="data/TestBeam2013_FNAL_ClustCharge/ClustChargeData_MPV.txt",
                        inputfilename_SIGMA="data/TestBeam2013_FNAL_ClustCharge/ClustChargeData_Sigma.txt",
                        iSkip=2):
        #Load the data from an input file
        ClustChargeData_MPV     = np.loadtxt(inputfilename_MPV,skiprows=iSkip)
        ClustChargeData_Mean	= np.loadtxt(inputfilename_MEAN,skiprows=iSkip)
        ClustChargeData_Sigma	= np.loadtxt(inputfilename_SIGMA,skiprows=iSkip)

        #Get the HVORGAIN & CLUSTSIZE values, note these are expected to be the same across the three files
        self.ARRAY_HVORGAIN = np.unique(ClustChargeData_MPV[:,0])
        self.ARRAY_CLUSTSIZE= np.unique(ClustChargeData_MPV[:,1])

        #Reparameterize in terms of Gain?
        if self.TRANSFORM2GAIN:
            self.ARRAY_HVORGAIN = self.GAIN_CALCULATOR.calcGain(self.ARRAY_HVORGAIN)

        #Get the Cluster Charge Parameters
        self.ARRAY_CLUSTQ_MPV   = np.reshape(ClustChargeData_MPV[:,2], (10,4), order='F')
        self.ARRAY_CLUSTQ_MEAN  = np.reshape(ClustChargeData_Mean[:,2], (10,4), order='F')
        self.ARRAY_CLUSTQ_SIGMA = np.reshape(ClustChargeData_Sigma[:,2], (10,4), order='F')

        #Print to user if requested
        if self.DEBUG:
            print "List of HV Or Gain Points:"
            print self.ARRAY_HVORGAIN

            print "List of Cluster Size Points:"
            print self.ARRAY_CLUSTSIZE

            print "Cluster Charge Landau MPV Values:"
            print self.ARRAY_CLUSTQ_MPV

            print "Cluster Charge Landau Mean Values:"
            print self.ARRAY_CLUSTQ_MEAN

            print "Cluster Charge Sigma Mean Values:"
            print self.ARRAY_CLUSTQ_SIGMA
            
        print "Data Loaded"
            
        return

    #Here:
    #	inputfilename	->	Text file that contains ordered triplets of the data in (x,y,z) format
    #	iSkip		->	Number of lines at start of inputfilename that should be skipped
    #	strObsName	->	Observable to plot, acceptable inputs are from set {MPV, MEAN, SIGMA}
    def makeAndStorePlot(self, inputfilename, iSkip=2, strObsName="MPV", outputfilename="AnaSuiteClustQOutput.root", strFileOpt="RECREATE"):
        #Load the data from an input text file
        clustChargeData = np.loadtxt(inputfilename, skiprows=iSkip)

        #Splice the clustChargeData into HVORGAIN & CLUSTSIZE
        self.ARRAY_HVORGAIN = clustChargeData[:,0]
        self.ARRAY_CLUSTSIZE= clustChargeData[:,1]

        #Print the shape if requested
        if self.DEBUG:
            print "Shape of clustChargeData = " + str(clustChargeData.shape)
            print "Shape of self.ARRAY_HVORGAIN = " + str(self.ARRAY_HVORGAIN.shape)
            print "Shape of self.ARRAY_CLUSTSIZE = " + str(self.ARRAY_CLUSTSIZE.shape)

        #Reparameterize in terms of Gain?
        strIndepVarName = "VDrift"
        if self.TRANSFORM2GAIN:
            strIndepVarName = "EffGain"
            #array_X = self.GAIN_CALCULATOR.calcGain(array_X)
            self.ARRAY_HVORGAIN = self.GAIN_CALCULATOR.calcGain(self.ARRAY_HVORGAIN)

        #Transform input observable name to uppercase
        strObsName = strObsName.upper()

        #Initialize the correct TGraph2D
	g2D_ClustQ_Obs = TGraph2D(len(clustChargeData))
	g2D_ClustQ_Obs.SetTitle("")
        if strObsName == self.STROBSNAME_MPV:
           #self.G2D_CLUSTQ_MPV.Set( len(clustChargeData) )
           #self.G2D_CLUSTQ_MPV.SetName("g2D_ClusterChargeMPV_StripSize_vs_" + strIndepVarName)
           #self.G2D_CLUSTQ_MPV.SetTitle("")
	   g2D_ClustQ_Obs.SetName("g2D_ClusterChargeMPV_StripSize_vs_" + strIndepVarName)
           self.ARRAY_CLUSTQ_MPV = clustChargeData[:,2]
        elif strObsName == self.STROBSNAME_MEAN:
           #self.G2D_CLUSTQ_MEAN.Set( len(clustChargeData) )
           #self.G2D_CLUSTQ_MEAN.SetName("g2D_ClusterChargeMean_StripSize_vs_" + strIndepVarName)
           #self.G2D_CLUSTQ_MEAN.SetTitle("")
	   g2D_ClustQ_Obs.SetName("g2D_ClusterChargeMean_StripSize_vs_" + strIndepVarName)
           self.ARRAY_CLUSTQ_MEAN = clustChargeData[:,2]
        elif strObsName == self.STROBSNAME_SIGMA:
           #self.G2D_CLUSTQ_SIGMA.Set( len(clustChargeData) )
           #self.G2D_CLUSTQ_SIGMA.SetName("g2D_ClusterChargeSigma_StripSize_vs_" + strIndepVarName)
           #self.G2D_CLUSTQ_SIGMA.SetTitle("")
	   g2D_ClustQ_Obs.SetName("g2D_ClusterChargeSigma_StripSize_vs_" + strIndepVarName)
           self.ARRAY_CLUSTQ_SIGMA = clustChargeData[:,2]
        else:
            print "Input Observable Name: " + strObsName
            print "Was not recognized, please cross-check and re-run"
            print "Exiting"
            return

        #Set the points
        for i in range(0, len(clustChargeData)):
           if strObsName == self.STROBSNAME_MPV:
               #self.G2D_CLUSTQ_MPV.SetPoint(i, self.ARRAY_HVORGAIN[i], self.ARRAY_CLUSTSIZE[i], self.ARRAY_CLUSTQ_MPV[i])
               g2D_ClustQ_Obs.SetPoint(i, self.ARRAY_HVORGAIN[i], self.ARRAY_CLUSTSIZE[i], self.ARRAY_CLUSTQ_MPV[i])
           elif strObsName == self.STROBSNAME_MEAN:
               #self.G2D_CLUSTQ_MEAN.SetPoint(i, self.ARRAY_HVORGAIN[i], self.ARRAY_CLUSTSIZE[i], self.ARRAY_CLUSTQ_MEAN[i])
               g2D_ClustQ_Obs.SetPoint(i, self.ARRAY_HVORGAIN[i], self.ARRAY_CLUSTSIZE[i], self.ARRAY_CLUSTQ_MEAN[i])
           elif strObsName == self.STROBSNAME_SIGMA:
               #self.G2D_CLUSTQ_SIGMA.SetPoint(i, self.ARRAY_HVORGAIN[i], self.ARRAY_CLUSTSIZE[i], self.ARRAY_CLUSTQ_SIGMA[i])
               g2D_ClustQ_Obs.SetPoint(i, self.ARRAY_HVORGAIN[i], self.ARRAY_CLUSTSIZE[i], self.ARRAY_CLUSTQ_SIGMA[i])

        #Reshape the arrays for later analysis
        self.ARRAY_HVORGAIN = np.unique(self.ARRAY_HVORGAIN)
        self.ARRAY_CLUSTSIZE = np.unique(self.ARRAY_CLUSTSIZE)

        if strObsName == self.STROBSNAME_MPV:
           self.ARRAY_CLUSTQ_MPV	= np.reshape(self.ARRAY_CLUSTQ_MPV,(len(self.ARRAY_HVORGAIN),len(self.ARRAY_CLUSTSIZE)),order='F')
        elif strObsName == self.STROBSNAME_MEAN:
           self.ARRAY_CLUSTQ_MEAN	= np.reshape(self.ARRAY_CLUSTQ_MEAN,(len(self.ARRAY_HVORGAIN),len(self.ARRAY_CLUSTSIZE)),order='F')
        elif strObsName == self.STROBSNAME_SIGMA:
           self.ARRAY_CLUSTQ_SIGMA	= np.reshape(self.ARRAY_CLUSTQ_SIGMA,(len(self.ARRAY_HVORGAIN),len(self.ARRAY_CLUSTSIZE)),order='F')

	#Store the Plot
        outputFile = TFile(outputfilename,strFileOpt,"",1)

	dir_Out = []
	if strFileOpt == "UPDATE":
	   dir_Out = outputFile.GetDirectory("ClusterChargeData")
	else:
	   dir_Out = outputFile.mkdir("ClusterChargeData")

	dir_Out.cd()
	g2D_ClustQ_Obs.Write()

	outputFile.Close()

        return

    #Here:
    #	outputfilename	->	Physical filename of the ROOT file to be created
    #	strFileOpt	->	Option of the root file, e.g. "RECREATE", "UPDATE", etc...
    #	strObsName	->	Observable to plot, acceptable inputs are from set {MPV, MEAN, SIGMA}
#    def storePlots(self, outputfilename, strFileOpt="RECREATE", strObsName="MPV"):
#        #Make the output file
#        outputFile = TFile(outputfilename,strFileOpt,"",1)
#
#        #Write the correct TGraph2D
#        if strObsName == self.STROBSNAME_MPV:
#           self.G2D_CLUSTQ_MPV.Write()
#        elif strObsName == self.STROBSNAME_MEAN:
#           self.G2D_CLUSTQ_MEAN.Write()
#        elif strObsName == self.STROBSNAME_SIGMA:
#           self.G2D_CLUSTQ_SIGMA.Write()
#        else:
#            print "Input Observable Name: " + strObsName
#            print "Was not recognized, please cross-check and re-run"
#            print "Exiting"
#            return
#
#        #Close the outputfile
#        outputFile.Close()
#
#        return
