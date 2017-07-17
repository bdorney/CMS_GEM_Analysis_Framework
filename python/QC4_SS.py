import datetime,os,sys
import subprocess
from subprocess import CalledProcessError
from wrappers import runCommand
from options import parser


args = parser.parse_args()
#print args.file

cmd1 = ["python","/Users/anastasia/desktop/QC_Plotting_Tools/Produce_Config_File.py"]
#cmd1.append("--infilename=%s"%(filename_thresh))
for f in args.file:
	cmd1.append(str(f))
	pass

cmd1.append("--CanvTitleX=Divider Current #left(#muA#right)")
cmd1.append("--CanvTitleY=Spurious Signal R_{SS} #left(Hz#right)")
cmd1.append("--LatexLine2=0.58,0.73, Readout~=~GEM3B")
cmd1.append("--SelectColumnX=3")
cmd1.append("--SelectColumnY=7")
cmd1.append("--SelectRowStart=3")
cmd1.append("--SelectRowEnd=37")
cmd1.append("--CanvRangeX= 0,1000")
cmd1.append("--CanvRangeY= 0,50")
cmd1.append("--SetErrY=True")
cmd1.append("--SelectColumnErrY=8")
cmd1.append("--OutputName= QC4_LS2_SS_vs_Imon_AllDet")
#cmd1.append("--YaxisScale= True")



runCommand(cmd1)