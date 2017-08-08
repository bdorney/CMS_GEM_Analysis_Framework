import datetime,os,sys
import subprocess
from subprocess import CalledProcessError
from wrappers import runCommand
from options import parser

args = parser.parse_args()
#print args.file

cmd1 = ["python","/Users/anastasia/desktop/QC_Plotting_Tools/Produce_Config_File.py"]

#string = "--file="
#j=0
for f in args.file:
	cmd1.append(str(f))
	pass

#print string
	

cmd1.append("--CanvTitleX=Divider Current #left(#muA#right)")
cmd1.append("--CanvTitleY=Applied Voltage #left(kV#right)")
cmd1.append("--SelectColumnX= 3")
cmd1.append("--SelectColumnY= 1")
cmd1.append("--SelectRowStart= 3")
cmd1.append("--SelectRowEnd= 37")
cmd1.append("--CanvRangeX= 0,1000")
cmd1.append("--CanvRangeY= 0,7")
cmd1.append("--YaxisScale= True")
cmd1.append("--OutputName= QC4_LS2_V_vs_Imon_AllDet")




runCommand(cmd1)