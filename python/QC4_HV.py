from subprocess import CalledProcessError
from wrappers import runCommand
from options import parser

args = parser.parse_args()
cmd = ["python","/Users/anastasia/desktop/QC_Plotting_Tools/Produce_Config_File.py"]

for f in args.file:
	cmd.append(str(f))
	pass

cmd.append("--CanvTitleX=Divider Current #left(#muA#right)")
cmd.append("--CanvTitleY=Applied Voltage #left(kV#right)")
cmd.append("--LatexLine2=0.62,0.20, R_{Equiv}~=~5.000~M#Omega")
cmd.append("--SelectColumnX= 3")
cmd.append("--SelectColumnY= 1")
cmd.append("--SelectRowStart= 3")
cmd.append("--SelectRowEnd= 37")
cmd.append("--CanvRangeX= 0,1000")
cmd.append("--CanvRangeY= 0,7")
cmd.append("--YaxisScale= True")
cmd.append("--OutputName= QC4_LS2_V_vs_Imon_AllDet")

runCommand(cmd)