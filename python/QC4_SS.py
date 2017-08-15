from subprocess import CalledProcessError
from wrappers import runCommand,envCheck
from options import parser
import os

args = parser.parse_args()
envCheck('GEM_BASE')
dataPath  = os.getenv('GEM_BASE')
cmd = ["python","%s/python/Produce_Config_File.py"%(dataPath)]

for filelist in args.file:
	cmd.append(filelist)
	pass

cmd.append("--CanvTitleX=Divider Current #left(#muA#right)")
cmd.append("--CanvTitleY=Spurious Signal R_{SS} #left(Hz#right)")
cmd.append("--LatexLine2=0.58,0.73, Readout~=~GEM3B")
cmd.append("--SelectColumnX=3")
cmd.append("--SelectColumnY=7")
cmd.append("--SelectRowStart=3")
cmd.append("--SelectRowEnd=37")
cmd.append("--CanvRangeX= 0,1000")
cmd.append("--CanvRangeY= 0,50")
cmd.append("--SetErrY=true")
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