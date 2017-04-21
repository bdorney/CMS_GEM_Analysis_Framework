import sys, os
import numpy as np
from scipy import interpolate
from Utilities import *
from ROOT import gROOT, TCanvas, TDirectory, TFile, TGraph2D

class ClusterChargeAnalysisSuite:
    def __init__(self, params_gain=PARAMS_GAIN(), calcGain=False, debug=False ):
	self.DEBUG              = debug
	self.TRANSFORM2GAIN	= calcGain

	self.ARRAY_HVORGAIN	= np.zeros(0) #Array of HV Points or Gain Cluster Charge Measurements were taken as
	self.ARRAY_CLUSTSIZE	= np.zeros(0) #Array of Cluster Charge Measurements were taken as
	self.ARRAY_CLUSTQ_MPV	= np.zeros(0) #Array of MIP Cluster Charge Landua MPV, shape will be (col x row) = ( len(self.ARRAY_CLUSTSIZE) x len(self.ARRAY_HVPTS) )
	self.ARRAY_CLUSTQ_MEAN	= np.zeros(0) #Array of MIP Cluster Charge Landau Mean, shape as above
	self.ARRAY_CLUSTQ_SIGMA	= np.zeros(0) #Array of MIP Cluster Charge Landua Sigma, shape as above

        self.GAIN_CALCULATOR    = params_gain

	self.G2D_CLUSTQ_MPV	= TGraph2D() #Cluster Charge - MPV;   (x,y,z) = (HV or Gain, Clust Size, MPV   in fC)
	self.G2D_CLUSTQ_MEAN	= TGraph2D() #Cluster Charge - Mean;  (x,y,z) = (HV or Gain, Clust Size, Mean  in fC)
	self.G2D_CLUSTQ_SIGMA	= TGraph2D() #Cluster Charge - Sigma; (x,y,z) = (HV or Gain, Clust Size, Sigma in fC)

	self.STROBSNAME_MPV	= "MPV"
	self.STROBSNAME_MEAN	= "MEAN"
	self.STROBSNAME_SIGMA	= "SIGMA"

	return

    #Load data from file
    #Note X & Y coordinate points are expected to be identical, only Z values should change
    def loadData(self,  inputfilename_MPV="data/TestBeam2013_FNAL_ClustCharge/ClustChargeData_MPV.txt",
			inputfilename_MEAN="data/TestBeam2013_FNAL_ClustCharge/ClustChargeData_Mean.txt",
			inputfilename_SIGMA="data/TestBeam2013_FNAL_ClustCharge/ClustChargeData_Sigma.txt",
			iSkip=2):
	#Load the data from an input file
	clustChargeData_MPV	= np.loadtxt(inputfilename_MPV,skiprows=iSkip)
	clustChargeData_Mean	= np.loadtxt(inputfilename_Mean,skiprows=iSkip)
	clustChargeData_Sigma	= np.loadtxt(inputfilename_Sigma,skiprows=iSkip)

	#Get the HVORGAIN & CLUSTSIZE values, note these are expected to be the same across the three files
	self.ARRAY_HVORGAIN = np.unique(clustChargeData_MPV[:,0])
	self.ARRAY_CLUSTSIZE= np.unique(clustChargeData_MPV[:,1])

	#Strip out non-unique members of HVORGAIN & CLUSTSIZE
	

HV	= data[:,0]
Size	= data[:,1]
MPV	= data[:,2]


MPV = np.reshape(MPV,(10,4),order='F')


    #Here:
    #	inputfilename	->	Text file that contains ordered triplets of the data in (x,y,z) format
    #	iSkip		->	Number of lines at start of inputfilename that should be skipped
    #	strObsName	->	Observable to plot, acceptable inputs are from set {MPV, MEAN, SIGMA}
    def makePlot(self, inputfilename, iSkip=2, strObsName="MPV"):
	#Load the data from an input text file
	clustChargeData = np.loadtxt(inputfilename, skiprows=iSkip)

	#Splice the clustChargeData into HVORGAIN & CLUSTSIZE
	#array_X = clustChargeData[:,0]
	#array_Y = clustChargeData[:,1]
	#array_Z = clustChargeData[:,2]
	self.ARRAY_HVORGAIN = clustChargeData[:,0]
	self.ARRAY_CLUSTSIZE= clustChargeData[:,1]

	#Print the shape if requested
	if self.DEBUG:
	    print "Shape of clustChargeData = " + str(clustChargeData.shape)
	    #print "Shape of array_X = " + str(array_X.shape)
	    #print "Shape of array_Y = " + str(array_Y.shape)
	    #print "Shape of array_Z = " + str(array_Z.shape)
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
	if strObsName == self.STROBSNAME_MPV:
	   self.G2D_CLUSTQ_MPV.Set( len(clustChargeData) )
	   self.G2D_CLUSTQ_MPV.SetName("g2D_ClusterChargeMPV_StripSize_vs_" + strIndepVarName)
	   self.G2D_CLUSTQ_MPV.SetTitle("")
	   self.ARRAY_CLUSTQ_MPV = clustChargeData[:,2]
	elif strObsName == self.STROBSNAME_MEAN:
	   self.G2D_CLUSTQ_MEAN.Set( len(clustChargeData) )
	   self.G2D_CLUSTQ_MEAN.SetName("g2D_ClusterChargeMean_StripSize_vs_" + strIndepVarName)
	   self.G2D_CLUSTQ_MEAN.SetTitle("")
	   self.ARRAY_CLUSTQ_MEAN = clustChargeData[:,2]
	elif strObsName == self.STROBSNAME_SIGMA:
	   self.G2D_CLUSTQ_SIGMA.Set( len(clustChargeData) )
	   self.G2D_CLUSTQ_SIGMA.SetName("g2D_ClusterChargeSigma_StripSize_vs_" + strIndepVarName)
	   self.G2D_CLUSTQ_SIGMA.SetTitle("")
	   self.ARRAY_CLUSTQ_SIGMA = clustChargeData[:,2]
	else:
	    print "Input Observable Name: " + strObsName
	    print "Was not recognized, please cross-check and re-run"
	    print "Exiting"
	    return

	#Set the points
	for i in range(0, len(clustChargeData)):
	   if strObsName == self.STROBSNAME_MPV:
	       #self.G2D_CLUSTQ_MPV.SetPoint(i, array_X[i], array_Y[i], array_Z[i])
	       self.G2D_CLUSTQ_MPV.SetPoint(i, self.ARRAY_HVORGAIN[i], self.ARRAY_CLUSTSIZE[i], self.ARRAY_CLUSTQ_MPV[i])
	   elif strObsName == self.STROBSNAME_MEAN:
	       #self.G2D_CLUSTQ_MEAN.SetPoint(i, array_X[i], array_Y[i], array_Z[i])
	       self.G2D_CLUSTQ_MEAN.SetPoint(i, self.ARRAY_HVORGAIN[i], self.ARRAY_CLUSTSIZE[i], self.ARRAY_CLUSTQ_MEAN[i])
	   elif strObsName == self.STROBSNAME_SIGMA:
	       #self.G2D_CLUSTQ_SIGMA.SetPoint(i, array_X[i], array_Y[i], array_Z[i])
	       self.G2D_CLUSTQ_SIGMA.SetPoint(i, self.ARRAY_HVORGAIN[i], self.ARRAY_CLUSTSIZE[i], self.ARRAY_CLUSTQ_SIGMA[i])

	#Reshape the arrays for later analysis
	self.ARRAY_HVORGAIN = np.unique(self.ARRAY_HVORGAIN)
	self.ARRAY_CLUSTSIZE = np.unique(self.ARRAY_CLUSTSIZE)

	self.ARRAY_CLUSTQ_MPV	= np.reshape(self.ARRAY_CLUSTQ_MPV,(len(self.ARRAY_HVORGAIN),len(self.ARRAY_CLUSTSIZE)),order='F')
	self.ARRAY_CLUSTQ_MEAN	= np.reshape(self.ARRAY_CLUSTQ_MEAN,(len(self.ARRAY_HVORGAIN),len(self.ARRAY_CLUSTSIZE)),order='F')
	self.ARRAY_CLUSTQ_SIGMA	= np.reshape(self.ARRAY_CLUSTQ_SIGMA,(len(self.ARRAY_HVORGAIN),len(self.ARRAY_CLUSTSIZE)),order='F')

	return

    #Here:
    #	outputfilename	->	Physical filename of the ROOT file to be created
    #	strFileOpt	->	Option of the root file, e.g. "RECREATE", "UPDATE", etc...
    #	strObsName	->	Observable to plot, acceptable inputs are from set {MPV, MEAN, SIGMA}
    def storePlots(self, outputfilename, strFileOpt="RECREATE", strObsName="MPV"):
	#Make the output file
	outputFile = TFile(outputfilename,strFileOpt,"",1)

	#Write the correct TGraph2D
	if strObsName == self.STROBSNAME_MPV:
	   self.G2D_CLUSTQ_MPV.Write()
	elif strObsName == self.STROBSNAME_MEAN:
	   self.G2D_CLUSTQ_MEAN.Write()
	elif strObsName == self.STROBSNAME_SIGMA:
	   self.G2D_CLUSTQ_SIGMA.Write()
	else:
	    print "Input Observable Name: " + strObsName
	    print "Was not recognized, please cross-check and re-run"
	    print "Exiting"
	    return

	#Close the outputfile
	outputFile.Close()

	return
