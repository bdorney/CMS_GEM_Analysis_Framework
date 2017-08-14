from subprocess import CalledProcessError
from wrappers import runCommand
from options import parser

args = parser.parse_args()

cmd = ["python","$GEM_BASE/python/Produce_Config_File.py"]

for f in args.file:
	cmd.append(f)
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
cmd.append("--SetErrY=True")
cmd.append("--SelectColumnErrY=8")


if len(args.file)==1:
	filetype=f[int(f.index('.')):]
	filename = f
	filename= filename.replace(filetype,"")
	cmd.append("--OutputName=SS_vs_Imon_"+filename)
else:
	cmd.append("--OutputName= QC4_LS2_SS_vs_Imon_AllDet")
	pass

runCommand(cmd)