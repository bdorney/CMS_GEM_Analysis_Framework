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

cmd.append("--CanvTitleX=Time #left(s#right)")
cmd.append("--CanvTitleY=Pressure #left(mbar#right)")
cmd.append("--SelectColumnX= 1")
cmd.append("--SelectColumnY= 2")
cmd.append("--SelectRowStart= 1")
cmd.append("--SelectRowEnd= 61")
cmd.append("--CanvRangeX= 0,3600")
cmd.append("--CanvRangeY= 0,35")

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
	cmd.append("--OutputName= config_QC3_LS2_Pres_vs_Time_AllDet")
	pass

runCommand(cmd)