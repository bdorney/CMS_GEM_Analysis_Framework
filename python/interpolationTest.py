import numpy as np
from scipy import interpolate
import matplotlib.pyplot as plt

data	= np.loadtxt('/afs/cern.ch/user/d/dorney/scratch0/CMS_GEM/CMS_GEM_Analysis_Framework/data/TestBeam2013_FNAL_ClustCharge/ClustChargeData_MPV.txt',skiprows=2)
HV	= data[:,0]
Size	= data[:,1]
MPV	= data[:,2]

HV = np.unique(HV)
Size = np.unique(Size)

MPV = np.reshape(MPV,(10,4),order='F')
print "len(MPV) = "
print len(MPV)

print "Initializing surf"
surf = []

print "Using surf"
surf = interpolate.interp2d(Size,HV,MPV,kind='cubic')

new_HV = np.arange(np.min(HV),np.max(HV)+10,10)
new_Size = np.arange(np.min(Size),np.max(Size)+0.25,0.25)
new_MPV = surf(new_Size, new_HV)

#print "new_MPV:"
#print new_MPV

print "surf(2.3,2983) = "
print surf(2.3,2983)

#plt.plot(Size, MPV[0,:], 'ro-', new_Size, new_MPV[0,:],'b-')
plt.plot(HV, MPV[:,0], 'ro-', new_HV, new_MPV[:,0],'b-')
plt.show()
