#!/afs/cern.ch/sw/lcg/external/Python/2.7.4/x86_64-slc6-gcc48-opt/bin/python2.7

from subprocess import CalledProcessError
from wrappers import runCommand,envCheck
from PlotOptions import parser
import os

(args,opts)=parser.parse_args()

cmd = ["Produce_Config_File.py"]

for filelist in args.file:
	cmd.append("--file=%s"%(filelist))
	pass

cmd.append("--CanvTitleX=Time #left(s#right)")
cmd.append("--CanvTitleY=Pressure #left(mbar#right)")
cmd.append("--SelectColumnX=1")
cmd.append("--SelectColumnY=2")
cmd.append("--SelectRowStart=1")
cmd.append("--SelectRowEnd=60")
cmd.append("--CanvRangeX=0,3600")
cmd.append("--CanvRangeY=0,35")
cmd.append("--LatexLines=0.62,0.86, #splitline{LS2}{Detector~Production}")
cmd.append("--LatexLines=0.62,0.27, Gas~=~CO_{2}")

# Fit the Pressure vs Time curves
if bool(args.Fit):
	cmd.append("--Fit")
	cmd.append("--FitFormula=[0]*TMath::Exp([1]*x)")
	cmd.append("--FitParamIGuess=AMPLITUDE,6.2e-05")
	cmd.append("--FitRange=0,3600")
	pass

if len(args.file)==1:
	filetype=filelist[int(filelist.index('.')):]
	filename = filelist
	filename= filename.replace(filetype,"")
	cmd.append("--OutputName=config_"+filename)
	pass
else:
	cmd.append("--OutputName=config_QC3_LS2_Pres_vs_Time_AllDet")
	pass

runCommand(cmd)
