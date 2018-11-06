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

cmd.append("--CanvTitleX=Divider Current #left(#muA#right)")
cmd.append("--CanvTitleY=Spurious Signal R_{SS} #left(Hz#right)")
cmd.append("--LatexLines=0.62,0.86, #splitline{LS2}{Detector~Production}")
cmd.append("--LatexLines=0.62,0.27, Gas~=~CO_{2}")
cmd.append("--LatexLines=0.58,0.73, Readout~=~GEM3B")
cmd.append("--SelectColumnX=3")
cmd.append("--SelectColumnY=7")
cmd.append("--SelectRowStart=3")
cmd.append("--SelectRowEnd=36")
cmd.append("--CanvRangeX=0,1000")
cmd.append("--CanvRangeY=0,50")
cmd.append("--SetErrY")
cmd.append("--SelectColumnErrY=8")

if len(args.file)==1:
	filetype=filelist[int(filelist.index('.')):]
	filename = filelist
	filename= filename.replace(filetype,"")
	cmd.append("--OutputName=config_SS_vs_Imon_"+filename)
	pass
else:
	cmd.append("--OutputName=config_QC4_LS2_SS_vs_Imon_AllDet")
	pass

runCommand(cmd)
