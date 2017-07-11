import datetime,os,sys
import subprocess
from subprocess import CalledProcessError
from wrappers import runCommand
from options import parser

#parser.add_argument('--file',dest="file", nargs='+')
args = parser.parse_args()
#print args.file

cmd1 = ["python","/Users/anastasia/desktop/QC_Plotting_Tools/Produce_Config_File.py"]
#cmd1.append("--infilename=%s"%(filename_thresh))
for f in args.file:
	cmd1.append("--file="+str(f))
	pass

cmd1.append("--CanvTitleX=Divider Current #left(#muA#right)")
cmd1.append("--CanvTitleY=Applied Voltage #left(kV#right)")
cmd1.append("--SelectColumnX= 3")
cmd1.append("--SelectColumnY= 1")
cmd1.append("--SelectRowStart= 3")
cmd1.append("--SelectRowEnd= 37")
cmd1.append("--CanvRangeX= 0,1000")
cmd1.append("--CanvRangeY= 0,7")
cmd1.append("--YaxisScale= True")


#cmd1.append("--fileScurveFitTree=%s"%(filename_scurve))
#cmd1.append("--vfatmask=0x0")

runCommand(cmd1)