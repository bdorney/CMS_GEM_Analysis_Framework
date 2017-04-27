#Imports
import sys, os
import numpy as np
#import ROOT
from ROOT import TKey, TFile, TDirectory, TDirectoryFile

#=====================Functions=====================

#From: https://root-forum.cern.ch/t/loop-over-all-objects-in-a-root-file/10807/4
def getall(d, basepath="/"):
    "Generator function to recurse into a ROOT file/dir and yield (path, obj) pairs"
    for key in d.GetListOfKeys():
        kname = key.GetName()
        if key.IsFolder():
            # TODO: -> "yield from" in Py3
            for i in getall(d.Get(kname), basepath+kname+"/"):
                yield i
        else:
            yield basepath+kname, d.Get(kname)

#Use Median absolute deviation (MAD) to reject outliers)
#See: http://stackoverflow.com/questions/22354094/pythonic-way-of-detecting-outliers-in-one-dimensional-observation-data
#And also: http://www.itl.nist.gov/div898/handbook/eda/section3/eda35h.htm
def rejectOutliers(arrayData, thresh=3.5):
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

#=====================Classes=====================

#---------------------PARAMS_GAIN---------------------
#Class for storing gain parameters
#Gain is defined as:
#
#   G(x) = exp([0]*x+[1]) where x is an HV setpoint (either V_Drift in Volts or Divider Current in uA)
#
class PARAMS_GAIN:
    __slots__ = ['GAIN_CURVE_P0','GAIN_CURVE_P0_ERR','GAIN_CURVE_P1','GAIN_CURVE_P1_ERR']
    
    def __init__(self, gain_p0=1, gain_p0_err=1, gain_p1=1, gain_p1_err=1):
        self.GAIN_CURVE_P0      = gain_p0
        self.GAIN_CURVE_P0_ERR  = gain_p0_err
        self.GAIN_CURVE_P1      = gain_p1
        self.GAIN_CURVE_P1_ERR  = gain_p1_err
        
        return
    
    #G(x) = exp([0]*x+[1]) where x is hvPt
    def calcGain(self, hvPt):
        return np.exp(self.GAIN_CURVE_P0 * hvPt + self.GAIN_CURVE_P1)
    
    #G(x) = exp([0]*x+[1]) where x is hvPt
    def calcGainErr(self, hvPt):
        return self.calcGain(hvPt)*np.sqrt(np.square(self.GAIN_CURVE_P0_ERR * hvPt)+np.square(self.GAIN_CURVE_P1_ERR))

#---------------------PARAMS_ETASECTOR---------------------
#Class for storing Eta Sector Parameters
class PARAMS_ETASECTOR:
    __slots__ = ['IETA', 'SECTPOS', 'SECTSIZE', 'NBCONNECT']
    
    def __init__(self):
        self.IETA       = -1
        self.SECTPOS    = 0.0
        self.SECTSIZE   = 0.0
        self.NBCONNECT  = 3     #Number of Phi Sectors within Eta Sector
    
    	return

#---------------------PARAMS_PD---------------------
#Class for storing discharge probability parameters
class PARAMS_PD:
    __slots__ = ['PD_CONST', 'PD_SLOPE']
    
    def __init__(self, const=-2.12136e+01, slope=2.49075e-05):
        self.PD_CONST = const
        self.PD_SLOPE = slope
        
        return
    
    #PD(gain) = exp(slope*gain+Const)
    def calcPD(self, gain):
        return np.exp(self.PD_SLOPE*gain+self.PD_CONST)

    #Note the error is not straight forward since we reported an 99% C.L. on the error band
    #Suggest against attempting to do something like what is shown in PARAMS_GAIN.calcGainErr()

#---------------------PARAMS_DET---------------------
#Class for storing detector mapping information
class PARAMS_DET:
    __slots__ = ['DETPOS_IETA', 'DETPOS_IPHI', 'LIST_DET_GEO_PARAMS', 'DET_IMON_QC5_RESP_UNI']
    
    def __init__(self, ieta=4, iphi=2, imon0=600):
        self.DETPOS_IETA            = ieta          #iEta Position QC5_Gain_Cal performed in
        self.DETPOS_IPHI            = iphi          #iPhi "                                 "
        
        self.LIST_DET_GEO_PARAMS    = []
        
        self.DET_IMON_QC5_RESP_UNI  = imon0         #Imon value QC5_Resp_Uni was performed at
        
        return
    
    def reset(self, debug=False):
        self.DETPOS_IETA = -1
        self.DETPOS_IPHI = -1
        self.DET_IMON_QC5_RESP_UNI = -1
        
        del self.LIST_DET_GEO_PARAMS[:]
    
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
                detgeo = PARAMS_ETASECTOR()
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
    
        return

    #Given an eta sector, determines the Phi boundaries within an Eta Sector
    def calcROSectorBoundaries(self, params_eta=PARAMS_ETASECTOR()):
        #Calculate the iphi sector boundaries
        list_boundaries = []
        for i in range(0, params_eta.NBCONNECT+1):
            list_boundaries.append(-0.5 * params_eta.SECTSIZE + i * params_eta.SECTSIZE / params_eta.NBCONNECT)

        return list_boundaries

    #Given a specific ieta index, determines the Phi boundaries within an Eta Sector
    def calcROSectorBoundariesByEta(self, iEta=4):
        return self.calcROSectorBoundaries(self.LIST_DET_GEO_PARAMS[iEta-1])
