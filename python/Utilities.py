#Imports
import sys, os
import numpy as np
import root_numpy as rp
#import ROOT
from ROOT import TGraph2D, TKey, TFile, TDirectory, TDirectoryFile, TArrayD

#=====================Functions=====================

#Turns an array into a dictionary
def array2Dict(array):
    return dict(((a,b),c) for (a,b,c) in set(map(tuple,array)))

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

#Returns a numpy array of form:
#   [[x1, y1, z1],
#    [x2, y2, z2],
#    ...
#    ...
#    [xN, yN, zN]]
#
#Here:  g2D         -> input TGraph2D object
#       round_xy    -> if the x & y values should be rounded
#       decPts    -> decimal points to round xy arrays to
def getDataTGraph2D(g2D, round_xy=True, decPts=3):
    array_fPx = rp.array( TArrayD(g2D.GetN(), g2D.GetX() ) )
    array_fPy = rp.array( TArrayD(g2D.GetN(), g2D.GetY() ) )
    array_fPz = rp.array( TArrayD(g2D.GetN(), g2D.GetZ() ) )

    if round_xy:
        array_fPx = np.round(array_fPx, decimals=decPts)
        array_fPy = np.round(array_fPy, decimals=decPts)

    return np.column_stack((array_fPx, array_fPy, array_fPz))

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
    __slots__ = ['BOUNDARIES_SET','IETA', 'SECTPOS', 'SECTSIZE', 'NBCONNECT', 'LIST_IPHI_BOUNDARIES']
    
    def __init__(self):
	self.BOUNDARIES_SET = False

        self.IETA       = -1
        self.NBCONNECT  = 3     	#Number of Phi Sectors within Eta Sector
	self.NBCONNECT_NSTRIP = 128	#Number of strips in a Phi Sector 
        self.SECTPOS    = 0.0
        self.SECTSIZE   = 0.0
	
	self.LIST_IPHI_BOUNDARIES = []  #Boundaries across  

    	return

    #Determines the Phi boundaries within an Eta Sector
    def calcBoundaries(self):
        #Calculate the iphi sector boundaries
        for i in range(0, self.NBCONNECT+1):
            self.LIST_IPHI_BOUNDARIES.append(-0.5 * self.SECTSIZE + i * self.SECTSIZE / self.NBCONNECT)

	self.BOUNDARIES_SET = True

        return

    #Determines the Phi boundaries within an Eta Sector
    def getiPhiIndex(self, fPos):
	for idx in range(0, self.NBCONNECT+1):
            if self.LIST_IPHI_BOUNDARIES[idx] <= fPos and fPos <= self.LIST_IPHI_BOUNDARIES[idx+1]:
		return idx+1

    #Returns the strips in a given partition of the detector centered on fPos
    def getStripRange(self, fPos, iSlices=32):
	iThisSlice = self.getSliceIdx(fPos, iSlices)
	
	#print "PARAMS_ETASECTOR::getStripRange() - fPos = " + str(fPos)
	#print "PARAMS_ETASECTOR::getStripRange() - iSlices = " + str(iSlices)
	#print "PARAMS_ETASECTOR::getStripRange() - Current Slice = " + str(iThisSlice)
	#print "PARAMS_ETASECTOR::getStripRange() - (self.NBCONNECT_NSTRIP / iSlices) = " + str(self.NBCONNECT_NSTRIP / iSlices)
	#print "PARAMS_ETASECTOR::getStripRange() - First Strip = " + str((self.NBCONNECT_NSTRIP / iSlices) * (iThisSlice -1))
	#print "PARAMS_ETASECTOR::getStripRange() - Last Strip = " + str((self.NBCONNECT_NSTRIP / iSlices) * (iThisSlice))

	return [ (self.NBCONNECT_NSTRIP / iSlices) * (iThisSlice - 1), (self.NBCONNECT_NSTRIP / iSlices) * (iThisSlice) - 1 ] #Strip number goes 0 to 127

    #def getSliceIdx(self, iphi, fPos, iSlices=32):
    def getSliceIdx(self, fPos, iSlices=32):
	iPhi = self.getiPhiIndex(fPos)
	fSliceWidth = (self.LIST_IPHI_BOUNDARIES[iPhi]-self.LIST_IPHI_BOUNDARIES[iPhi-1])
	fStepSize = fSliceWidth / iSlices
	thisSlice = int(np.round(( 1. / fStepSize) * (self.LIST_IPHI_BOUNDARIES[iPhi] + 0.5 * fStepSize - fPos)))

	if thisSlice > iSlices:
	    print "PARAMS_ETASECTOR::getSliceIdx - fPhi_Min = " + str(self.LIST_IPHI_BOUNDARIES[iPhi-1])
	    print "PARAMS_ETASECTOR::getSliceIdx - fPos = " + str(fPos)
	    print "PARAMS_ETASECTOR::getSliceIdx - fPhi_Max = " + str(self.LIST_IPHI_BOUNDARIES[iPhi])
	    print "PARAMS_ETASECTOR::getSliceIdx - iPhi = " + str(iPhi)
	    print "PARAMS_ETASECTOR::getSliceIdx - fSliceWidth = " + str(fSliceWidth)
	    print "PARAMS_ETASECTOR::getSliceIdx - Distance From Max Edge = " + str((self.LIST_IPHI_BOUNDARIES[iPhi] - fPos))
	    print "PARAMS_ETASECTOR::getSliceIdx - Slice = " + str(thisSlice)

	#return iSlices - 0.5 * ( fSliceWidth / (self.LIST_IPHI_BOUNDARIES[self.getiPhiIndex(fPos)] - fPos) )
	#return int(np.round(2. * ((self.LIST_IPHI_BOUNDARIES[iPhi] - fPos) * (iSlices / fSliceWidth) )))
	return thisSlice

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
                
		detgeo.calcBoundaries()

                self.LIST_DET_GEO_PARAMS.append(detgeo)
            else:
                continue
        
        #Print the geometry to the user
        if debug:
            print "Loaded Mapping:"
            for idx in range(0,len(self.LIST_DET_GEO_PARAMS)):
                strLine = str(idx) + "\t" + str(self.LIST_DET_GEO_PARAMS[idx].IETA) + "\t"
                #strLine = strLine + str(self.LIST_DET_GEO_PARAMS[idx].IETA) + "\t"
                strLine = strLine + str(self.LIST_DET_GEO_PARAMS[idx].SECTPOS) + "\t"
                strLine = strLine + str(self.LIST_DET_GEO_PARAMS[idx].SECTSIZE) + "\t"
                strLine = strLine + str(self.LIST_DET_GEO_PARAMS[idx].NBCONNECT) + "\n"
                
                print strLine
    
        return

    #Given an eta sector, determines the Phi boundaries within an Eta Sector
    def calcROSectorBoundaries(self, params_eta=PARAMS_ETASECTOR()):
	#Calc iPhi Sector Boundaries if they have not been calculated	
	if params_eta.BOUNDARIES_SET == False:
	    params_eta.calcBoundaries()

        return params_eta.LIST_IPHI_BOUNDARIES

    #Given a specific ieta index, determines the Phi boundaries within an Eta Sector
    def calcROSectorBoundariesByEta(self, iEta=4):
        return self.calcROSectorBoundaries(self.LIST_DET_GEO_PARAMS[iEta-1])

    #Given an input (x,y) coordinate, return a tuple that is the (ieta,iphi) coordinate
    def getiEtaiPhIndex(self, fPos_X, fPos_Y):
	for idx in range(0, len(self.LIST_DET_GEO_PARAMS)):
	    fEtaLim_Low = 0.
	    fEtaLim_High= 0.

	    #Setup Evaluation Limits
	    if self.LIST_DET_GEO_PARAMS[idx].SECTPOS > 0:	#Case: iEta Y-Pos > 0
		fEtaLim_Low = self.LIST_DET_GEO_PARAMS[idx].SECTPOS - (0.1 * self.LIST_DET_GEO_PARAMS[idx].SECTPOS);
                fEtaLim_High= self.LIST_DET_GEO_PARAMS[idx].SECTPOS + (0.1 * self.LIST_DET_GEO_PARAMS[idx].SECTPOS);
	    else:						#Case: iEta Y-Pos <=0
		fEtaLim_Low = self.LIST_DET_GEO_PARAMS[idx].SECTPOS + (0.1 * self.LIST_DET_GEO_PARAMS[idx].SECTPOS);
                fEtaLim_High= self.LIST_DET_GEO_PARAMS[idx].SECTPOS - (0.1 * self.LIST_DET_GEO_PARAMS[idx].SECTPOS);

	    #print fEtaLim_Low, fPos_Y, fEtaLim_High

	    if(fEtaLim_Low < fPos_Y and fPos_Y < fEtaLim_High):
		#print self.LIST_DET_GEO_PARAMS[idx].LIST_IPHI_BOUNDARIES
		#print fPos_X

		return (idx+1, self.LIST_DET_GEO_PARAMS[idx].getiPhiIndex(fPos_X))

    #def getStripRange(self, fPos_X, fPos_Y, iSlices=32):
	#tuple_iEtaiPhi = self.getiEtaiPhIndex(fPos_X, fPos_Y)
	#list_StripRange = self.LIST_DET_GEO_PARAMS[tuple_iEtaiPhi[0]-1].getStripRange(fPos_X, iSlices)
	#iThisSlice = self.LIST_DET_GEO_PARAMS[tuple_iEtaiPhi[0]-1].getSliceIdx(tuple_iEtaiPhi[1], fPos_X, iSlices)

	#print fPos_X, fPos_Y, tuple_iEtaiPhi[0], tuple_iEtaiPhi[1], list_StripRange, iThisSlice

	#return list_StripRange
