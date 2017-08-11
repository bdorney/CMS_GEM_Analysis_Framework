from subprocess import CalledProcessError
from wrappers import runCommand
from options import parser

args = parser.parse_args()
cmd = ["python","/Users/anastasia/desktop/QC_Plotting_Tools/Produce_Config_File.py"]

for f in args.file:
	cmd.append(str(f))
	pass

cmd.append("--CanvTitleX=Time #left(s#right)")
cmd.append("--CanvTitleY=Pressure #left(mbar#right)")
cmd.append("--SelectColumnX= 1")
cmd.append("--SelectColumnY= 2")
cmd.append("--SelectRowStart= 1")
cmd.append("--SelectRowEnd= 61")
cmd.append("--CanvRangeX= 0,3600")
cmd.append("--CanvRangeY= 0,35")
cmd.append("--OutputName= QC3_LS2_Pres_vs_Time_AllDet")

runCommand(cmd)