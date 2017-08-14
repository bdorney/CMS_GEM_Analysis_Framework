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
text_file.write("\tCanv_Axis_NDiv = '"+args.AxisNDiv+"'; #X,Y\n")
text_file.write("\tCanv_Dim= '"+args.CanvDim+"'; #X,Y\n")
text_file.write("\tCanv_DrawOpt = '"+args.CanvDrawOpt+"';\n")
text_file.write("\tCanv_Grid_XY = '"+args.CanvGridXY+"';\n")
text_file.write("\tCanv_Latex_Line = '"+args.LatexLine+"';\n")
text_file.write("\tCanv_Latex_Line = '"+args.LatexLine1+"';\n")
text_file.write("\tCanv_Latex_Line = '"+args.LatexLine2+"';\n")
text_file.write("\tCanv_Latex_Line = '"+args.LatexLine3+"';\n")
text_file.write("\tCanv_Latex_Line = '"+args.LatexLine4+"';\n")
text_file.write("\tCanv_Latex_Line = '"+args.LatexLine5+"';\n")
text_file.write("\tCanv_Legend_Dim_X = '"+args.CanvLegDimX+"';\n")
text_file.write("\tCanv_Legend_Dim_Y = '"+args.CanvLegDimY+"';\n")
text_file.write("\tCanv_Legend_Draw = '"+args.CanvLegDraw+"'; \n")
text_file.write("\tCanv_Log_XY = '"+args.CanvLogXY+"';\n")
text_file.write("\tCanv_Logo_Pos = '"+args.CanvLogoPos+"'; #0, 11, 22, 33\n")
text_file.write("\tCanv_Logo_Prelim = '"+args.CanvLogoPrelim+"';\n")
text_file.write("\tCanv_Margin_Top = '"+args.CanvMarginTop+"';\n")
text_file.write("\tCanv_Margin_Bot = '"+args.CanvMarginBot+"';\n")
text_file.write("\tCanv_Margin_Lf = '"+args.CanvMarginLf+"';\n")
text_file.write("\tCanv_Margin_Rt = '"+args.CanvMarginRt+"';\n")
text_file.write("\tCanv_Name = '"+args.CanvName+"';\n")
text_file.write("\tCanv_Plot_Type = '"+args.CanvPlotType+"';\n")
text_file.write("\tCanv_Range_X = '"+args.CanvRangeX+"'; #X1, X2\n")
text_file.write("\tCanv_Range_Y = '"+args.CanvRangeY+"'; #Y1, Y2\n")
text_file.write("\tCanv_Title_Offset_X = '"+args.CanvTitleOffsetX+"';\n")
text_file.write("\tCanv_Title_Offset_Y = '"+args.CanvTitleOffsetX+"';\n")
text_file.write("\tCanv_Title_X = '"+args.CanvTitleX+"';\n")
text_file.write("\tCanv_Title_Y = '"+args.CanvTitleY+"';\n")

for file in args.file:
	workbook = xlrd.open_workbook(file)
	ws = workbook.sheet_by_index(args.SelectSheetNum)
	Gcharacter=int(file.index('G'))
	legEntry=file[Gcharacter:Gcharacter+18]
	Marker_Style = int(20+j)

	text_file.write("\t\t[BEGIN_PLOT]\n" )
	text_file.write("\t\t\tPlot_Color = '"+str(Color(j))+ "';\n" )
	text_file.write("\t\t\tPlot_LegEntry = '"+str(legEntry)+"';\n" )
	text_file.write("\t\t\tPlot_Line_Size = '"+args.PlotLineSize+"';\n" )
	text_file.write("\t\t\tPlot_Line_Style = '"+args.PlotLineStyle+"';\n" )
	text_file.write("\t\t\tPlot_Marker_Size = '"+args.PlotMarkerSize+"';\n" )
	text_file.write("\t\t\tPlot_Marker_Style = '"+str(Marker_Style)+"';\n" )
	text_file.write("\t\t\tPlot_Name = '"+str(legEntry)+"'; \n" )
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