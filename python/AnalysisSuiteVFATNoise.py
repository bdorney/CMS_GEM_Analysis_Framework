
# -*- coding: utf-8 -*-

"""
    Created on Wednesday May 3
    
    @author: Brian L. Dorney
"""

import sys, os
import numpy as np
import root_numpy as rp
from Utilities import *
from ROOT import TFile

class AnalysisSuiteVFATNoise:

    __slots__ = ['CONVERT2fC',
		 'DEBUG',
		 'DICT_MAPPING',
		 #'DICT_NOISE',
		 #'DICT_PED',
		 #'DICT_THRESH',
		 'DICT_VFAT_DATA',
		 'E2fC',
		 'NSTRIPS',
		 'OFFSET',
                 'PARAMS_DET_DUT',
		 'VCALDAQ2E']

    def __init__(self, 
		 detector,
		 inputMappingFile="data/sliceTestAna/p5SCurves/vfatPos2iEtaiPhi.txt", 
		 inputSCurveFile="data/sliceTestAna/p5SCurves/analyzedSCurves/ztrim3fitData_GEMINIm28L1.root",
		 #outputfilename="AnaSuiteVFATNoiseOutput.root", 
		 #outputfileoption="RECREATE", 
		 convert2fC=True,
		 debug=False):

	#Store debugging information
	self.DEBUG = debug

	#Set number of strips in the front-end (e.g. VFAT)
	self.NSTRIPS = 128

	#Convert electrons to fC?
	self.CONVERT2fC = convert2fC
	self.E2fC = 1.6021766e-4 #converts 1 e (elementary charge) to fC

	#Initialize the Detector
	self.PARAMS_DET_DUT = detector

	#Initialize the VFAT Position to (ieta,iphi) Mapping
	array_Mapping = np.loadtxt(inputMappingFile,skiprows=1)
        #self.DICT_MAPPING = {(idx[0],idx[1]):[idx[2]] for idx in array_Mapping}
        self.DICT_MAPPING = {(idx[0],idx[1]):idx[2] for idx in array_Mapping}

	#Give the user the mapping that was created
        if self.DEBUG:
	    print "(ieta, iphi) VFAT"
            for key in self.DICT_MAPPING:
		print key, self.DICT_MAPPING[key]

	#Initialize the noise offset & VCAL DAC to electrons value,for now just use the value Cameron gave
	self.OFFSET = 4993.2073
	self.VCALDAQ2E = 314

	#Load the S-Curve data
	#list_bNames = ["vfatN","threshold","noise","pedestal"]
	list_bNames = ["vfatN","ROBstr","mask","threshold","noise","pedestal"]
	#array_VFATSCurveData = rp.root2array(inputSCurveFile,treename="scurveFitTree",branches=list_bNames, selection="mask != 1")
	array_VFATSCurveData = rp.root2array(inputSCurveFile,treename="scurveFitTree",branches=list_bNames)

	#Initialize the VFAT S-Curve maps
	#self.DICT_NOISE	= {idx:[] for idx in np.unique(array_VFATSCurveData[list_bNames[0]])}
	#self.DICT_PED	= {idx:[] for idx in np.unique(array_VFATSCurveData[list_bNames[0]])}
	#self.DICT_THRESH= {idx:[] for idx in np.unique(array_VFATSCurveData[list_bNames[0]])}

	#This dictionary has VFAT position as the key value, returns a structured numpy array for each where the idx inside the array is the ROBstr #
	self.DICT_VFAT_DATA = {idx:self.initVFATArray(array_VFATSCurveData.dtype) for idx in np.unique(array_VFATSCurveData[list_bNames[0]])}

	#Fill the VFAT S-Curve maps
	for dataPt in array_VFATSCurveData:
	    #self.DICT_NOISE[dataPt['vfatN']].append(dataPt['noise']*self.VCALDAQ2E)
	    #self.DICT_PED[dataPt['vfatN']].append(dataPt['pedestal']*self.VCALDAQ2E)
	    #self.DICT_THRESH[dataPt['vfatN']].append(dataPt['threshold']*self.VCALDAQ2E)

	    self.DICT_VFAT_DATA[dataPt['vfatN']][dataPt['ROBstr']]['mask']	= dataPt['mask']
	    self.DICT_VFAT_DATA[dataPt['vfatN']][dataPt['ROBstr']]['threshold']	= dataPt['threshold']	* self.VCALDAQ2E
	    self.DICT_VFAT_DATA[dataPt['vfatN']][dataPt['ROBstr']]['noise']	= dataPt['noise']	* self.VCALDAQ2E
	    self.DICT_VFAT_DATA[dataPt['vfatN']][dataPt['ROBstr']]['pedestal']	= dataPt['pedestal']	* self.VCALDAQ2E
	        
        return

    def initVFATArray(self, array_dtype):
	list_dtypeTuple = []

	for idx in range(0,len(array_dtype)):
	    if array_dtype.names[idx] == 'vfatN':	continue
	    if array_dtype.names[idx] == 'ROBstr':	continue
	    list_dtypeTuple.append((array_dtype.names[idx],array_dtype[idx]))

	return np.zeros(self.NSTRIPS, dtype=list_dtypeTuple)

    #Returns <Noise> for give (ieta,iphi) sector over a given strip range [iStripFirst, iStripLast]
    def getValByEtaPhiNoiseAvg(self, ieta, iphi, iStripFirst=0, iStripLast=127):
	return self.getValByVFATPosNoiseAvg(self.DICT_MAPPING[(ieta,iphi)],iStripFirst,iStripLast)

    #Returns sigma_Noise for give (ieta,iphi) sector over a given strip range [iStripFirst, iStripLast]
    def getValByEtaPhiNoiseStdDev(self, ieta, iphi, iStripFirst=0, iStripLast=127):
	return self.getValByVFATPosNoiseStdDev(self.DICT_MAPPING[(ieta,iphi)],iStripFirst,iStripLast)

    #Returns <Pedestal> for give (ieta,iphi) sector over a given strip range [iStripFirst, iStripLast]
    def getValByEtaPhiPedestalAvg(self, ieta, iphi, iStripFirst=0, iStripLast=127):
	return self.getValByVFATPosPedestalAvg(self.DICT_MAPPING[(ieta,iphi)],iStripFirst,iStripLast)

    #Returns sigma_Pedestal for give (ieta,iphi) sector over a given strip range [iStripFirst, iStripLast]
    def getValByEtaPhiPedestalStdDev(self, ieta, iphi, iStripFirst=0, iStripLast=127):
	return self.getValByVFATPosPedestalStdDev(self.DICT_MAPPING[(ieta,iphi)],iStripFirst,iStripLast)

    #Returns <Thresh> for give (ieta,iphi) sector over a given strip range [iStripFirst, iStripLast]
    def getValByEtaPhiThreshAvg(self, ieta, iphi, iStripFirst=0, iStripLast=127):
	return self.getValByVFATPosThreshAvg(self.DICT_MAPPING[(ieta,iphi)],iStripFirst,iStripLast)

    #Returns sigma_Thresh for give (ieta,iphi) sector over a given strip range [iStripFirst, iStripLast]
    def getValByEtaPhiThreshStdDev(self, ieta, iphi, iStripFirst=0, iStripLast=127):
	return self.getValByVFATPosThreshStdDev(self.DICT_MAPPING[(ieta,iphi)],iStripFirst,iStripLast)

    def getValByEtaPhiSliceMaskAvg(self, ieta, iphi, iStripFirst=0, iStripLast=127):
	return self.getValByVFATPosSliceMaskAvg(self.DICT_MAPPING[(ieta,iphi)],iStripFirst,iStripLast)

    #Returns <Noise> for give VFAT Position over a given strip range [iStripFirst, iStripLast]
    def getValByVFATPosNoiseAvg(self, vfatPos, iStripFirst=0, iStripLast=127):
	if self.CONVERT2fC:
	    #return np.mean(self.DICT_NOISE[vfatPos]) * self.E2fC
	    return np.mean(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['noise']) * self.E2fC
	else:
	    #return np.mean(self.DICT_NOISE[vfatPos])
	    return np.mean(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['noise'])

    #Returns sigma_Noise for give VFAT Position over a given strip range [iStripFirst, iStripLast]
    def getValByVFATPosNoiseStdDev(self, vfatPos, iStripFirst=0, iStripLast=127):
	if self.CONVERT2fC:
	    #return np.std(self.DICT_NOISE[vfatPos]) * self.E2fC
	    return np.std(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['noise']) * self.E2fC
	else:
	    #return np.std(self.DICT_NOISE[vfatPos])
	    return np.std(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['noise'])

    #Returns <Pedestal> for give VFAT Position over a given strip range [iStripFirst, iStripLast]
    def getValByVFATPosPedestalAvg(self, vfatPos, iStripFirst=0, iStripLast=127):
	if self.CONVERT2fC:
	    #return np.mean(self.DICT_PED[vfatPos]) * self.E2fC
	    return np.mean(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['pedestal']) * self.E2fC
	else:
	    #return np.mean(self.DICT_PED[vfatPos])
	    return np.mean(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['pedestal'])

    #Returns sigma_Pedestal for give VFAT Position over a given strip range [iStripFirst, iStripLast]
    def getValByVFATPosPedestalStdDev(self, vfatPos, iStripFirst=0, iStripLast=127):
	if self.CONVERT2fC:
	    #return np.std(self.DICT_PED[vfatPos]) * self.E2fC
	    return np.std(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['pedestal']) * self.E2fC
	else:
	    #return np.std(self.DICT_PED[vfatPos]) over a given strip range [iStripFirst, iStripLast]
	    return np.std(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['pedestal'])

    #Returns <Thresh> for give VFAT Position over a given strip range [iStripFirst, iStripLast]
    def getValByVFATPosThreshAvg(self, vfatPos, iStripFirst=0, iStripLast=127):
	#print "AnalysisSuiteVFATNoise::getValByVFATPosThreshAvg() - iStripFirst = {0}".format(iStripFirst)
	#print "AnalysisSuiteVFATNoise::getValByVFATPosThreshAvg() - iStripLast = {0}".format(iStripLast)

	if self.CONVERT2fC:
	    #return np.mean(self.DICT_THRESH[vfatPos]) * self.E2fC
	    return np.mean(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['threshold']) * self.E2fC
	else:
	    #return np.mean(self.DICT_THRESH[vfatPos])
	    return np.mean(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['threshold'])

    #Returns sigma_Thresh for give VFAT Position over a given strip range [iStripFirst, iStripLast]
    def getValByVFATPosThreshStdDev(self, vfatPos, iStripFirst=0, iStripLast=127):
	if self.CONVERT2fC:
	    #return np.std(self.DICT_THRESH[vfatPos]) * self.E2fC
	    return np.std(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['threshold']) * self.E2fC
	else:
	    #return np.std(self.DICT_THRESH[vfatPos])
	    return np.std(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['threshold'])

    #For a given range of the VFAT, return the weighted channel mask
    def getValByVFATPosSliceMaskAvg(self, vfatPos, iStripFirst=0, iStripLast=127):
	return (1. - np.mean(self.DICT_VFAT_DATA[vfatPos][iStripFirst:iStripLast+1]['mask']))
