import xlrd
from array import array
import numpy as np
import ROOT as r
from options import parser
import sys

if len(sys.argv)==1:
    parser.print_help()
    sys.exit(1)
    pass

def Color(j):
	if j % 7 == 0:
		ret_int='kRed'
		pass
	elif j % 7 == 1:
		ret_int='kRed+1'
		pass
	elif j % 7 == 2:
		ret_int='kRed+2'
		pass
	elif j % 7 == 3:
		ret_int='kRed+3'
		pass
	elif j % 7 == 4:
		ret_int='kBlue'
		pass
	elif j % 7 == 5:
		ret_int='kBlue+1'
		pass
	elif j % 7 == 6:
		ret_int='kBlue+2'
		pass
	return ret_int

args = parser.parse_args()
text_file = open(args.OutputName+".cfg", "w")

j=0

text_file.write("[BEGIN_CANVAS]\n")
text_file.write("\tCanv_Axis_NDiv = '%s';#X,Y\n"%(args.AxisNDiv))
text_file.write("\tCanv_Dim= '%s';#X,Y\n"%(args.CanvDim))
text_file.write("\tCanv_DrawOpt = '%s';\n"%(args.CanvDrawOpt))
text_file.write("\tCanv_Grid_XY = '%s';\n"%(args.CanvGridXY))
text_file.write("\tCanv_Latex_Line = '%s';\n"%(args.LatexLine))
text_file.write("\tCanv_Latex_Line = '%s';\n"%(args.LatexLine1))
text_file.write("\tCanv_Latex_Line = '%s';\n"%(args.LatexLine2))
text_file.write("\tCanv_Latex_Line = '%s';\n"%(args.LatexLine3))
text_file.write("\tCanv_Latex_Line = '%s';\n"%(args.LatexLine4))
text_file.write("\tCanv_Latex_Line = '%s';\n"%(args.LatexLine5))
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

for file in args.file:
	workbook = xlrd.open_workbook(file)
	ws = workbook.sheet_by_index(args.SelectSheetNum)
	Gcharacter=int(file.index('G'))
	legEntry=file[Gcharacter:Gcharacter+18]
	Marker_Style = int(20+j)

	text_file.write("\t\t[BEGIN_PLOT]\n" )
	text_file.write("\t\t\tPlot_Color = '%s';\n"%(Color(j)))
	text_file.write("\t\t\tPlot_LegEntry = '%s';\n"%(legEntry))
	text_file.write("\t\t\tPlot_Line_Size = '%s';\n"%(args.PlotLineSize))
	text_file.write("\t\t\tPlot_Line_Style = '%s';\n"%(args.PlotLineStyle))
	text_file.write("\t\t\tPlot_Marker_Size = '%s';\n"%(args.PlotMarkerSize))
	text_file.write("\t\t\tPlot_Marker_Style = '%s';\n"%(Marker_Style))
	text_file.write("\t\t\tPlot_Name = '%s';\n"%(str(legEntry)))
	text_file.write("\t\t\t[BEGIN_DATA]\n" )
	text_file.write("\t\t\tVAR_INDEP,VAR_DEP,VAR_INDEP_ERR,VAR_DEP_ERR\n" )
	j=j+1

	for i in range(args.SelectRowStart,args.SelectRowEnd):
		if bool(args.YaxisScale):
			Y= float(ws.cell_value(i,args.SelectColumnY)/1000)
		else:
			Y= ws.cell_value(i,args.SelectColumnY)
			pass

		if bool(args.SetErrX)==True:
			ErrX=ws.cell_value(i,args.SelectColumnErrX)
		else:
			ErrX=0.
			pass

		if bool(args.SetErrY):
			ErrY=ws.cell_value(i,args.SelectColumnErrY)
		else:
			ErrY=0.
			pass

		X= ws.cell_value(i,args.SelectColumnX) 
		text_file.write(str(X)+","+str(Y)+","+str(ErrX)+","+str(ErrY)+"\n")
		pass

	text_file.write("\t\t\t[END_DATA]\n" )
	text_file.write("\t\t[END_PLOT]\n" )

text_file.write("[END_CANVAS]\n" )
text_file.close()