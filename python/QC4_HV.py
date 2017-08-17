from subprocess import CalledProcessError
from wrappers import runCommand,envCheck
from options import parser
import os
import xlrd

(args,opts)=parser.parse_args()

envCheck('GEM_BASE')
dataPath  = os.getenv('GEM_BASE')
cmd = ["python","%s/python/Produce_Config_File.py"%(dataPath)]

for filelist in args.file:
	cmd.append("--file=%s"%(filelist))
	pass

cmd.append("--CanvTitleX=Divider Current #left(#muA#right)")
cmd.append("--CanvTitleY=Applied Voltage #left(kV#right)")
cmd.append("--SelectColumnX=3")
cmd.append("--SelectColumnY=1")
cmd.append("--SelectRowStart=3")
cmd.append("--SelectRowEnd=37")
cmd.append("--CanvRangeX=0,1000")
cmd.append("--CanvRangeY=0,7")
cmd.append("--YaxisScale")
cmd.append("--LatexLines=0.62,0.86, #splitline{LS2}{Detector~Production}")
cmd.append("--LatexLines=0.62,0.27, Gas~=~CO_{2}")

if len(args.file)==1:
	filetype=filelist[int(filelist.index('.')):]
	filename = filelist
	filename= filename.replace(filetype,"")
	cmd.append("--OutputName=config_V_vs_Imon_"+filename)

	workbook = xlrd.open_workbook(filelist)
	ws = workbook.sheet_by_index(args.SelectSheetNum)
	Req=ws.cell_value(1,16)
	cmd.append("--LatexLines=0.62,0.20, R_{Equiv}~=~%s~M#Omega"%Req)
	pass
else:
	cmd.append("--OutputName= config_QC4_LS2_V_vs_Imon_AllDet")
	pass

# Fit the Vmon vs Imon curves
if bool(args.Fit):
	cmd.append("--Fit")
	cmd.append("--FitFormula=[0]*x+[1]")
	cmd.append("--FitParamIGuess=%s,5"%(Req))
	cmd.append("--FitRange=0,1000")
	pass

runCommand(cmd)