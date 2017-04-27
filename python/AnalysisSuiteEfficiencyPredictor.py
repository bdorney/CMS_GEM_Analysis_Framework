
# -*- coding: utf-8 -*-

"""
    Created on Monday April 24
    
    @author: Brian L. Dorney
    """

#Imports
import sys, os
import numpy as np
#from scipy import interpolate

#Python Toolkit Imports
from AnalysisSuiteGainMap import *
from AnalysisSuiteClusterCharge import *
from Utilities import *

#ROOT Imports
from ROOT import gROOT, TF1, TFile, TGraph2D

class AnalysisSuiteEfficiencyPredictor:

    __slots__ = ['DEBUG',
                 'LIST_HVPTS',
                 'NAME_DET_DUT',
                 'NAME_DET_CLUSTQ',
                 'NAME_DET_CLUSTSIZE',
                 'PARAMS_DET_DUT',
                 'SECTOR_IETA_CLUSTSIZENORM',
                 'SECTOR_IETA_QC5',
                 'SECTOR_IPHI_CLUSTSIZENORM',
                 'SECTOR_IPHI_QC5',
                 'ANA_UNI_GRANULARITY',
                 'PARAMS_GAIN_DET_DUT',
                 'PARAMS_GAIN_DET_CLUSTQ',
                 'PARAMS_GAIN_DET_CLUSTSIZE',
                 'FILE_CLUSTQ_MEAN',
                 'FILE_CLUSTQ_MPV',
                 'FILE_CLUSTQ_SIGMA',
                 'FILE_DUT_MAPPING',
                 'FILE_MIP_AVG_CLUST_SIZE',
                 'FILE_OUTPUT',
                 'FILE_QC5_RESP_UNI',
                 'TOBJ_NAME_FUNC_AVGCLUSTSIZE',
                 'ANASUITECLUSTQ',
                 'ANASUITEGAIN',
                 'FUNC_AVG_MIP_CLUSTSIZE'
                 ]

    #Here inputfilename should be a tab delimited file in the form of:
    #   field_name  value
    #
    #The field_name is not case sensitive but the value is case sensitive (as it may be a filepaths!)
    def __init__(self, inputfilename="config/configEffPredictor.cfg", debug=False):
        #Set the debug flag
        self.DEBUG  = debug

        #HV Points To Perform Calculations for
        self.LIST_HVPTS = []

        #Detector Names
        self.NAME_DET_DUT               = "Detector"
        self.NAME_DET_CLUSTQ            = "GE11-III-FIT-0001"
        self.NAME_DET_CLUSTSIZE         = "GE11-IV-CERN-0001"
        
        #Declare Detector Container
        self.PARAMS_DET_DUT             = PARAMS_DET()

        self.SECTOR_IETA_CLUSTSIZENORM  = 5
        self.SECTOR_IETA_QC5            = 4
        
        self.SECTOR_IPHI_CLUSTSIZENORM  = 2
        self.SECTOR_IPHI_QC5            = 2

        self.ANA_UNI_GRANULARITY	= 32

        #Declare Gain Containers
        self.PARAMS_GAIN_DET_DUT        = PARAMS_GAIN()
        self.PARAMS_GAIN_DET_CLUSTQ     = PARAMS_GAIN()
        self.PARAMS_GAIN_DET_CLUSTSIZE  = PARAMS_GAIN()

        #File Names
        self.FILE_CLUSTQ_MEAN           = "" #File having ordered triplet of (V_Drift, ClustSize, Mean) data
        self.FILE_CLUSTQ_MPV            = "" #As above but for MPV
        self.FILE_CLUSTQ_SIGMA          = "" #As above but for Sigma

        self.FILE_DUT_MAPPING           = "" #Mapping config file for detector under test
        
        self.FILE_MIP_AVG_CLUST_SIZE    = "" #File having a TF1 which is a fit of MIP <ClustSize> vs. Gain
        
        self.FILE_OUTPUT                = "" #Output filename to be created
        
        self.FILE_QC5_RESP_UNI          = "" #Framework output file
        
        #TObject Names
        self.TOBJ_NAME_FUNC_AVGCLUSTSIZE= ""
        
        #Load the input file and set all variables
        list_strLines = []
        with open(inputfilename) as inputFile:
            list_strLines = inputFile.readlines()
        inputFile.close()

        #strip new line character ('\n') from the file
        list_strLines = [x.strip() for x in list_strLines]

        if self.DEBUG:
            print "Field_Name\tValue"

        #Set Variables
        for iPos in range(0,len(list_strLines)):
            #Get this (field_name,value) pair
            strLine = list_strLines[iPos].split("\t")
           
            #Skip if commented (e.g. first character of first member is "#")
            if strLine[0][0] == "#":
                continue
            
            #Transform to upper case
            strLine[0] = strLine[0].upper()

            #Print (field_name, value) pair
            if self.DEBUG:
                print strLine[0] + "\t" + strLine[1]

            #Set values
            if strLine[0] == "FILE_FRAMEWORK_OUTPUT":
                #self.ANASUITEGAIN.openInputFile(strLine[1])
                self.FILE_QC5_RESP_UNI = strLine[1]
            elif strLine[0] == "FILE_CLUSTQ_MEAN":
                self.FILE_CLUSTQ_MEAN = strLine[1]
            elif strLine[0] == "FILE_CLUSTQ_MPV":
                self.FILE_CLUSTQ_MPV = strLine[1]
            elif strLine[0] == "FILE_CLUSTQ_SIGMA":
                self.FILE_CLUSTQ_SIGMA = strLine[1]
            elif strLine[0] == "FILE_CLUSESIZE":
                self.FILE_MIP_AVG_CLUST_SIZE = strLine[1]
            elif strLine[0] == "FILE_OUTPUT":
                self.FILE_OUTPUT = strLine[1]
            elif strLine[0] == "DUT_GAIN_P0":
                self.PARAMS_GAIN_DET_DUT.GAIN_CURVE_P0 = float(strLine[1])
            elif strLine[0] == "DUT_GAIN_P0_ERR":
                self.PARAMS_GAIN_DET_DUT.GAIN_CURVE_P0_ERR = float(strLine[1])
            elif strLine[0] == "DUT_GAIN_P1":
                self.PARAMS_GAIN_DET_DUT.GAIN_CURVE_P1 = float(strLine[1])
            elif strLine[0] == "DUT_GAIN_P1_ERR":
                self.PARAMS_GAIN_DET_DUT.GAIN_CURVE_P1_ERR = float(strLine[1])
            elif strLine[0] == "DUT_IETA_CLUST_SIZE_NORM":
                self.SECTOR_IETA_CLUSTSIZENORM = int(strLine[1])
            elif strLine[0] == "DUT_IETA_QC5_GAIN_CAL":
                self.SECTOR_IETA_QC5 = int(strLine[1])
            elif strLine[0] == "DUT_IPHI_CLUST_SIZE_NORM":
                self.SECTOR_IPHI_CLUSTSIZENORM = int(strLine[1])
            elif strLine[0] == "DUT_IPHI_QC5_GAIN_CAL":
                self.SECTOR_IPHI_QC5 = int(strLine[1])
            elif strLine[0] == "DUT_MAPPING_FILE":
                #if self.DEBUG:
                    #print "Loading Mapping"
                
                #self.PARAMS_DET_DUT.loadMapping(strLine[1], self.DEBUG)
                self.FILE_DUT_MAPPING = strLine[1]
            elif strLine[0] == "DUT_QC5_RESP_UNI_HVPT":
                self.PARAMS_DET_DUT.DET_IMON_QC5_RESP_UNI = float(strLine[1])
            elif strLine[0] == "DUT_SERIAL_NUMBER":
                self.NAME_DET_DUT = strLine[1]
            elif strLine[0] == "DUT_SLICE_GRANULARITY":
                self.ANA_UNI_GRANULARITY = int(strLine[1])
            elif strLine[0] == "DET_CLUSTQ_SERIAL_NUMBER":
                self.NAME_DET_CLUSTQ = strLine[1]
            elif strLine[0] == "DET_CLUSTQ_GAIN_P0":
                self.PARAMS_GAIN_DET_CLUSTQ.GAIN_CURVE_P0 = float(strLine[1])
            elif strLine[0] == "DET_CLUSTQ_GAIN_P0_ERR":
                self.PARAMS_GAIN_DET_CLUSTQ.GAIN_CURVE_P0_ERR = float(strLine[1])
            elif strLine[0] == "DET_CLUSTQ_GAIN_P1":
                self.PARAMS_GAIN_DET_CLUSTQ.GAIN_CURVE_P1 = float(strLine[1])
            elif strLine[0] == "DET_CLUSTQ_GAIN_P1_ERR":
                self.PARAMS_GAIN_DET_CLUSTQ.GAIN_CURVE_P1_ERR = float(strLine[1])
            elif strLine[0] == "DET_CLUSTSIZE_SERIAL_NUMBER":
                self.NAME_DET_CLUSTSIZE = strLine[1]
            elif strLine[0] == "DET_CLUSTSIZE_TF1_TNAME":
                self.TOBJ_NAME_FUNC_AVGCLUSTSIZE = strLine[1]
            elif strLine[0] == "EFF_HVPT_LIST":
                self.LIST_HVPTS = strLine[1].split(",")
            else:
                print "Input Field Name:"
                print strLine[0]
                print "Not recognized, please cross-check input file:"
                print inputfilename

        del list_strLines

        #Declare Cluster Charge Analysis Suite
        self.ANASUITECLUSTQ = AnalysisSuiteClusterCharge(params_gain=self.PARAMS_GAIN_DET_CLUSTQ,
                                                         calcGain=True,
                                                         debug=self.DEBUG)

        #Load the Landau Cluster Charge Data
        if self.DEBUG:
            print "Loading Landau Cluster Charge Data"
        
        if len(self.FILE_CLUSTQ_MEAN) == 0:
            print "Landau Cluster Charge Mean Filename not found, problem!"
        if len(self.FILE_CLUSTQ_MPV) == 0:
            print "Landau Cluster Charge MPV Filename not found, problem!"
        if len(self.FILE_CLUSTQ_SIGMA) == 0:
            print "Landau Cluster Charge Sigma Filename not found, problem!"

        self.ANASUITECLUSTQ.loadData(inputfilename_MEAN=self.FILE_CLUSTQ_MEAN,
                                     inputfilename_MPV=self.FILE_CLUSTQ_MPV,
                                     inputfilename_SIGMA=self.FILE_CLUSTQ_SIGMA)

        #Interpolate the Landau Cluster Charge Data
        if self.DEBUG:
            print "Interpolating Landau Cluster Charge Data"

        self.ANASUITECLUSTQ.interpolateData()

        #Declare Gain Map Analysis Suite
        self.PARAMS_DET_DUT.DETPOS_IETA = self.SECTOR_IETA_QC5
        self.PARAMS_DET_DUT.DETPOS_IPHI = self.SECTOR_IPHI_QC5

        if self.DEBUG:
            print "Loading Mapping"

        self.PARAMS_DET_DUT.loadMapping(self.FILE_DUT_MAPPING, self.DEBUG)

        self.ANASUITEGAIN   = AnalysisSuiteGainMap(inputfilename=self.FILE_QC5_RESP_UNI,
                                                   outputfilename=self.FILE_OUTPUT,
                                                   outputfileoption="RECREATE",
                                                   params_gain=self.PARAMS_GAIN_DET_DUT,
                                                   params_det=self.PARAMS_DET_DUT,
                                                   debug=self.DEBUG)

        #Load the MIP average cluster size vs. gain formulat
        fileMIPAvgClustSize = TFile(self.FILE_MIP_AVG_CLUST_SIZE, "READ","", 1)

        if self.DEBUG:
            print "Loading function for MIP <Clust Size> vs. Gain"

        self.FUNC_AVG_MIP_CLUSTSIZE = fileMIPAvgClustSize.Get(self.TOBJ_NAME_FUNC_AVGCLUSTSIZE)

        #Tell user initialization completed successfully
        if self.DEBUG:
            print "Initialization completed successfully"

        return

    def loadGainCurveDetClustQ(self, strInputFileName="figures/GE11-III-FIT-0001/GE11-III-FIT-0001_QC5_EffGain_VDrift.root", strTObjName="fit_GE11-III-FIT-0001_EffGain_42"):
        #Load the input file
        inputFile = TFile(str(strInputFileName),"READ","", 1)

        #Get the TF1 Object
        func_Gain = inputFile.Get(strTObjName)

        #Set the parameters
        self.PARAMS_GAIN_DET_CLUSTSIZE.GAIN_CURVE_P0       = func_Gain.GetParameter(0)
        self.PARAMS_GAIN_DET_CLUSTSIZE.GAIN_CURVE_P0_ERR   = func_Gain.GetParError(0)
        self.PARAMS_GAIN_DET_CLUSTSIZE.GAIN_CURVE_P1       = func_Gain.GetParameter(1)
        self.PARAMS_GAIN_DET_CLUSTSIZE.GAIN_CURVE_P1_ERR   = func_Gain.GetParError(1)

        #Close the input file
        inputFile.Close() #This might fuck it

        return

    def loadGainCurveDetClustSize(self, strInputFileName="figures/figures/GE11-IV-CERN-0001/GE11-IV-CERN-0001_GIF_QC5_EffGain_Imon.root", strTObjName="fit_GE11-IV-CERN-0001_GIF_EffGain_52"):
        #Load the input file
        inputFile = TFile(str(strInputFileName),"READ","", 1)
        
        #Get the TF1 Object
        func_Gain = inputFile.Get(strTObjName)
        
        #Set the parameters
        self.PARAMS_GAIN_DET_CLUSTSIZE.GAIN_CURVE_P0       = func_Gain.GetParameter(0)
        self.PARAMS_GAIN_DET_CLUSTSIZE.GAIN_CURVE_P0_ERR   = func_Gain.GetParError(0)
        self.PARAMS_GAIN_DET_CLUSTSIZE.GAIN_CURVE_P1       = func_Gain.GetParameter(1)
        self.PARAMS_GAIN_DET_CLUSTSIZE.GAIN_CURVE_P1_ERR   = func_Gain.GetParError(1)
        
        #Close the input file
        inputFile.Close() #This might fuck it
    
        return

    #Calculate the gain map from the original QC5_Resp_Uni measurement
    def calcGainMap(self):
        #Make sure the right (ieta,iphi) sector is set for the gain map calculation
        self.ANASUITEGAIN.DETECTOR.DETPOS_IETA = self.SECTOR_IETA_QC5
        self.ANASUITEGAIN.DETECTOR.DETPOS_IPHI = self.SECTOR_IPHI_QC5
        
        #Average Readout Sector Fitted ADC Pk Positions
        if self.DEBUG:
            print "Starting Gain Map calculation for: " + self.NAME_DET_DUT
            print "averaging ADC PkPositions in sector: (" + str(self.SECTOR_IETA_QC5) + "," + str(self.SECTOR_IPHI_QC5) + ")"
        
        self.ANASUITEGAIN.avgROSectorADCPkPos()
        
        #Compute lambda
        if self.DEBUG:
            print "Computing lambda"
        
        self.ANASUITEGAIN.calcROSectorLambda()

        #Compute the Gain Map
        if self.DEBUG:
            print "Calculating Gain Map"
        
        self.ANASUITEGAIN.calcGainMap(self.NAME_DET_DUT)

        return
            
    #Compute the normalized average cluster size map from the original QC5_Resp_Uni measurement
    def calcNormAvgClustSizeMap(self):
        #Make sure right (ieta,iphi) sector set for normalize avg clust size map calculation
        #This should map with the sector for which the MIP cluster size vs. gain data is provided for
        self.ANASUITEGAIN.DETECTOR.DETPOS_IETA = self.SECTOR_IETA_CLUSTSIZENORM
        self.ANASUITEGAIN.DETECTOR.DETPOS_IPHI = self.SECTOR_IPHI_CLUSTSIZENORM

        #Average Readout Sector Fitted ADC Pk Positions
        if self.DEBUG:
            print "Starting Normalized Average Cluster Size Map calculation for: " + self.NAME_DET_DUT
            print "Averaging <Cluster Size> in sector: (" + str(self.SECTOR_IETA_CLUSTSIZENORM) + "," + str(self.SECTOR_IPHI_CLUSTSIZENORM) + ")"
                
        self.ANASUITEGAIN.avgROSectorAvgClustSize()

        #Compute Normalized Average Cluster Size Map
        if self.DEBUG:
            print "Calculating Normalized Average Cluster Size Map"

        self.ANASUITEGAIN.calcClusterSizeMap(self.NAME_DET_DUT)
    
        return

    def predictEff(self, fHVPt, iNEvtPerPt=100):
        #Calculate the original gain map of the detector, needed to determine gain at fHVOrGain
        self.calcGainMap()

        #Get the gain map at fHVPt
        if self.DEBUG:
            print "Calculating Gain Map at Input HV Pt: " + str(fHVPt)
        
        g2D_Map_Gain_HVPt = self.ANASUITEGAIN.calcGainMapHV(self.NAME_DET_DUT, fHVPt)

        #Calculate the original normalized cluster map of the detector
        self.calcNormAvgClustSizeMap()

        #Now interesting problem N_pts in g2D_Map_Gain_HVPt <= N_pts in self.ANASUITEGAIN.G2D_MAP_AVG_CLUST_SIZE_NORM
        #We can make some numpy arrays
        #Then the (clustPos,y) members from G2D_MAP_AVG_CLUST_SIZE_NORM that don't appear in g2D_Map_Gain_HVPt
        
        #Make the container for the gain data
        array_fPx = g2D_Map_Gain_HVPt.GetX()
        array_fPy = g2D_Map_Gain_HVPt.GetY()
        array_fPz = g2D_Map_Gain_HVPt.GetZ()
        
        data_Gain = np.column_stack((array_fPx,array_fPy,array_fPz))
        
        #Make the container for the normalized average cluster size container
        array_fPx = self.ANASUITEGAIN.G2D_MAP_AVG_CLUST_SIZE_NORM.GetX()
        array_fPy = self.ANASUITEGAIN.G2D_MAP_AVG_CLUST_SIZE_NORM.GetY()
        array_fPz = self.ANASUITEGAIN.G2D_MAP_AVG_CLUST_SIZE_NORM.GetZ()
        
        data_NormAvgClustSize = np.column_stack((array_fPx,array_fPy,array_fPz))
        
        #Remove points in (X,Y) from data_NormAvgClustSize that are not in data_Gain
        if self.DEBUG:
            print "Shape of data_Gain = " + str(np.shape( data_Gain ) )
            print "Shape of data_NormAvgClustSize = " + str(np.shape( data_NormAvgClustSize ) )
        
        data_NormAvgClustSize = data_NormAvgClustSize[np.logical_not(data_Gain[:,0] != data_NormAvgClustSize[:,0])]
        
        if self.DEBUG:
            print "Shape of data_Gain = " + str(np.shape( data_Gain ) )
            print "Shape of data_NormAvgClustSize = " + str(np.shape( data_NormAvgClustSize ) )
        
        for idx in range(0, len(data_NormAvgClustSize)):
            if data_NormAvgClustSize[idx][1] != data_Gain[idx][1]:
                print "Points not equal, unexpected!"
                print "Norm <Clust Size>:"
                print data_NormAvgClustSize[idx]
                print "Gain"
                print data_Gain[idx]
                            
        
        #How do we store the Landaus then? Want to see what I am generating
        #Idea was 2D histograms of ClustCharge vs. ClustPos, then we can do a 1D projection
        #How do we get the binning right?
        #We known analysis granularity so N_bins = 3*Ana_granularity
        #We know the size of the iEta row

        return

