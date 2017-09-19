import xlrd
from array import array
import numpy as np
import ROOT as r
from PlotOptions import parser
import sys

if len(sys.argv)==1:
    parser.print_help()
    sys.exit(1)
    pass

def CyclicColor(DrawArg):
	if DrawArg % 7 == 0:
		ret_int='kRed'
		pass
	elif DrawArg % 7 == 1:
		ret_int='kRed+1'
		pass
	elif DrawArg % 7 == 2:
		ret_int='kRed+2'
		pass
	elif DrawArg % 7 == 3:
		ret_int='kRed+3'
		pass
	elif DrawArg % 7 == 4:
		ret_int='kBlue'
		pass
	elif DrawArg % 7 == 5:
		ret_int='kBlue+1'
		pass
	elif DrawArg % 7 == 6:
		ret_int='kBlue+2'
		pass
	return ret_int

(args,opts)=parser.parse_args()
text_file = open(args.OutputName+".cfg", "w")

DrawArg=0

text_file.write("[BEGIN_CANVAS]\n")
text_file.write("\tCanv_Axis_NDiv = '%s';#X,Y\n"%(args.AxisNDiv))
text_file.write("\tCanv_Dim= '%s';#X,Y\n"%(args.CanvDim))
text_file.write("\tCanv_DrawOpt = '%s';\n"%(args.CanvDrawOpt))
text_file.write("\tCanv_Grid_XY = '%s';\n"%(args.CanvGridXY))
if len(args.LatexLines) > 0:
    for line in args.LatexLines:
        text_file.write("\tCanv_Latex_Line = '%s';\n"%(line))
        pass
text_file.write("\tCanv_Legend_Dim_X = '%s';\n"%(args.CanvLegDimX))
text_file.write("\tCanv_Legend_Dim_Y = '%s';\n"%(args.CanvLegDimY))
text_file.write("\tCanv_Legend_Draw = '%s';\n"%(args.CanvLegDraw))
text_file.write("\tCanv_Log_XY = '%s';\n"%(args.CanvLogXY))
text_file.write("\tCanv_Logo_Pos = '%s';\n"%(args.CanvLogoPos))
text_file.write("\tCanv_Logo_Prelim = '%s';\n"%(args.CanvLogoPrelim))
text_file.write("\tCanv_Margin_Top = '%s';\n"%(args.CanvMarginTop))
text_file.write("\tCanv_Margin_Bot = '%s';\n"%(args.CanvMarginBot))
text_file.write("\tCanv_Margin_Lf = '%s';\n"%(args.CanvMarginLf))
text_file.write("\tCanv_Margin_Rt = '%s';\n"%(args.CanvMarginRt))
text_file.write("\tCanv_Name = '%s';\n"%(args.CanvName))
text_file.write("\tCanv_Plot_Type = '%s';\n"%(args.CanvPlotType))
text_file.write("\tCanv_Range_X = '%s';\n"%(args.CanvRangeX))
text_file.write("\tCanv_Range_Y = '%s';\n"%(args.CanvRangeY))
text_file.write("\tCanv_Title_Offset_X = '%s';\n"%(args.CanvTitleOffsetX))
text_file.write("\tCanv_Title_Offset_Y = '%s';\n"%(args.CanvTitleOffsetX))
text_file.write("\tCanv_Title_X = '%s';\n"%(args.CanvTitleX))
text_file.write("\tCanv_Title_Y = '%s';\n"%(args.CanvTitleY))

for filelist in args.file:
	workbook = xlrd.open_workbook(filelist)
	ws = workbook.sheet_by_index(args.SelectSheetNum)
	GEcharacter=int(filelist.index('GE'))
	legEntry=filelist[GEcharacter:GEcharacter+18]
	Marker_Style = int(20+DrawArg)

	text_file.write("\t\t[BEGIN_PLOT]\n" )
	text_file.write("\t\t\tPlot_Color = '%s';\n"%(CyclicColor(DrawArg)))
	text_file.write("\t\t\tPlot_LegEntry = '%s';\n"%(legEntry))
	text_file.write("\t\t\tPlot_Line_Size = '%s';\n"%(args.PlotLineSize))
	text_file.write("\t\t\tPlot_Line_Style = '%s';\n"%(args.PlotLineStyle))
	text_file.write("\t\t\tPlot_Marker_Size = '%s';\n"%(args.PlotMarkerSize))
	text_file.write("\t\t\tPlot_Marker_Style = '%s';\n"%(Marker_Style))
	text_file.write("\t\t\tPlot_Name = '%s';\n"%(str(legEntry)))
	text_file.write("\t\t\t[BEGIN_DATA]\n" )
	text_file.write("\t\t\t\tVAR_INDEP,VAR_DEP,VAR_INDEP_ERR,VAR_DEP_ERR\n" )

	for row in range(args.SelectRowStart,args.SelectRowEnd+1):
		if bool(args.YaxisScale):
			Y= float(ws.cell_value(row,args.SelectColumnY)/1000)
			pass
		else:
			Y= ws.cell_value(row,args.SelectColumnY)
			pass

		if bool(args.SetErrX):
			ErrX=ws.cell_value(row,args.SelectColumnErrX)
			pass
		else:
			ErrX=0.
			pass

		if bool(args.SetErrY):
			ErrY=ws.cell_value(row,args.SelectColumnErrY)
			pass
		else:
			ErrY=0.
			pass

		X= ws.cell_value(row,args.SelectColumnX) 
		text_file.write("\t\t\t\t"+str(X)+","+str(Y)+","+str(ErrX)+","+str(ErrY)+"\n")
		pass

	text_file.write("\t\t\t[END_DATA]\n" )

	if bool(args.Fit):
		text_file.write("\t\t\t[BEGIN_FIT]\n" )
		text_file.write("\t\t\t\tFit_Color = '%s';\n"%(CyclicColor(DrawArg)))
		text_file.write("\t\t\t\tFit_Formula = '%s';\n"%(args.FitFormula))
		text_file.write("\t\t\t\tFit_LegEntry = 'Fit_%s';\n"%(legEntry))
		text_file.write("\t\t\t\tFit_Line_Size = '%s';\n"%(args.FitLineSize))
		text_file.write("\t\t\t\tFit_Line_Style = '%s';\n"%(args.FitLineStyle))
		text_file.write("\t\t\t\tFit_Name = 'Fit_%s';\n"%(legEntry))
		text_file.write("\t\t\t\tFit_Option = '%s';\n"%(args.FitOption))
		text_file.write("\t\t\t\tFit_Param_IGuess = '%s';\n"%(args.FitParamIGuess))
		text_file.write("\t\t\t\tFit_Perform = '%s';\n"%(args.FitPerform))
		text_file.write("\t\t\t\tFit_Range = '%s';\n"%(args.FitRange))
		text_file.write("\t\t\t[END_FIT]\n" )
		pass

	DrawArg+=1
	text_file.write("\t\t[END_PLOT]\n" )

text_file.write("[END_CANVAS]\n" )
text_file.close()
