from subprocess import CalledProcessError
from wrappers import runCommand
from options import parser

args = parser.parse_args()

cmd = ["python","$GEM_BASE/python/Produce_Config_File.py"]

for f in args.file:
	cmd.append(f)
	pass

cmd.append("--CanvTitleX=Divider Current #left(#muA#right)")
cmd.append("--CanvTitleY=Rate #left(Hz#right)")
cmd.append("--LatexLine=0.19,0.88, LS2~Detector~Production")
cmd.append("--LatexLine1=0.19,0.83, Gas~=~Ar/CO_{2}~#left(70/30#right)")
cmd.append("--LatexLine2=0.19,0.78, X-Ray~Target:~Ag")
cmd.append("--LatexLine3=0.19,0.73, X-Ray~V_{mon}~=~40~kV")
cmd.append("--LatexLine4=0.19,0.68, X-Ray~I_{mon}~~~=~5~#muA")
cmd.append("--LatexLine5=0.19,0.63, i#eta~=~4;~i#phi~=~2")
cmd.append("--SelectColumnX=4")
cmd.append("--SelectColumnY=7")
cmd.append("--SelectRowStart=29")
cmd.append("--SelectRowEnd=45")
cmd.append("--CanvRangeX= 0,750")
cmd.append("--CanvRangeY= 0,1800")
cmd.append("--SetErrY=True")
cmd.append("--SelectColumnErrY=8")


if len(args.file)==1:
	filetype=f[int(f.index('.')):]
	filename = f
	filename= filename.replace(filetype,"")
	cmd.append("--OutputName=Rate_vs_Imon_"+filename)
else:
	cmd.append("--OutputName= QC5_LS2_Rate_vs_Imon_AllDet")
	pass

runCommand(cmd)