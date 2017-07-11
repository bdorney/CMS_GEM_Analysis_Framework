import pandas as pd
import xlrd
from array import array
import numpy as np
from ROOT import TGraph
from ROOT import TMultiGraph
import ROOT as r
import argparse
from optparse import OptionParser
from options import parser
import sys

#parser = argparse.ArgumentParser(add_help=True)
#parser.add_argument('file', nargs='+')
#parser.add_argument('--test',action="store",dest="test",help="Select the type of test: QC3, QC4_HV QC4_SS")
if len(sys.argv)==1:
    parser.print_help()
    sys.exit(1)

args = parser.parse_args()



#if str(args.test)== 'QC4_HV':
	#CanvTitleX = 'Divider Current #left(#muA#right)'
	#CanvTitleY = 'Applied Voltage #left(kV#right)'
	#CanvRangeX= '0,1000'
	#CanvRangeY= '0,7'
	#ColumnX=int(3)
	#ColumnY= int(1)
	#pass

#if str(args.test)=='QC4_SS':
#	CanvTitleX = 'Divider Current #left(#muA#right)'
#	CanvTitleY = 'Spurious Signal R_{SS} #left(Hz#right)'
#	CanvRangeX= '0,1000'
#	CanvRangeY= '0,50'
#	ColumnX=int(3)
#	ColumnY= int(7)
#	pass

#if str(args.test)=='QC3':
#	CanvTitleX = 'Time #left(s#right)'
#	CanvTitleY = 'Pressure #left(mbar#right)'
#	CanvRangeX= '0,3600'
#	CanvRangeY= '0,35'
#	ColumnX=int(1)
#	ColumnY= int(2)
#	pass	



text_file = open(str(args.OutputName)+".cfg", "w")
j=0

text_file.write("[BEGIN_CANVAS]\n")
text_file.write("    Canv_Axis_NDiv = '"+str(args.AxisNDiv)+"'; #X,Y\n")
text_file.write("    Canv_Dim= '"+str(args.CanvDim)+"'; #X,Y\n")
text_file.write("    Canv_DrawOpt = '"+str(args.CanvDrawOpt)+"';\n")
text_file.write("    Canv_Grid_XY = '"+str(args.CanvGridXY)+"';\n")
text_file.write("    Canv_Latex_Line = '"+str(args.LatexLine)+"';\n")
text_file.write("    Canv_Latex_Line = '"+str(args.LatexLine1)+"';\n")
text_file.write("    Canv_Latex_Line = '"+str(args.LatexLine2)+"';\n")
text_file.write("    Canv_Legend_Dim_X = '"+str(args.CanvLegDimX)+"';\n")
text_file.write("    Canv_Legend_Dim_Y = '"+str(args.CanvLegDimY)+"';\n")
text_file.write("    Canv_Legend_Draw = '"+str(args.CanvLegDraw)+"'; \n")
text_file.write("    Canv_Log_XY = '"+str(args.CanvLogXY)+"';\n")
text_file.write("    Canv_Logo_Pos = '"+str(args.CanvLogoPos)+"'; #0, 11, 22, 33\n")
text_file.write("    Canv_Logo_Prelim = '"+str(args.CanvLogoPrelim)+"';\n")
text_file.write("    Canv_Margin_Top = '"+str(args.CanvMarginTop)+"';\n")
text_file.write("    Canv_Margin_Bot = '"+str(args.CanvMarginBot)+"';\n")
text_file.write("    Canv_Margin_Lf = '"+str(args.CanvMarginLf)+"';\n")
text_file.write("    Canv_Margin_Rt = '"+str(args.CanvMarginRt)+"';\n")
text_file.write("    Canv_Name = '"+str(args.CanvName)+"';\n")
text_file.write("    Canv_Plot_Type = '"+str(args.CanvPlotType)+"';\n")
text_file.write("    Canv_Range_X = '"+str(args.CanvRangeX)+"'; #X1, X2\n")
text_file.write("    Canv_Range_Y = '"+str(args.CanvRangeY)+"'; #Y1, Y2\n")
text_file.write("    Canv_Title_Offset_X = '"+str(args.CanvTitleOffsetX)+"';\n")
text_file.write("    Canv_Title_Offset_Y = '"+str(args.CanvTitleOffsetX)+"';\n")
text_file.write("    Canv_Title_X = '"+str(args.CanvTitleX)+"';\n")
text_file.write("    Canv_Title_Y = '"+str(args.CanvTitleY)+"';\n")
for f in args.file:
	
	workbook = xlrd.open_workbook(str(f))
	ws = workbook.sheet_by_index(int(args.SelectSheetNum))
	Gcharacter=int(f.index('G'))
	legEntry=f[int(Gcharacter):int(Gcharacter+18)]
	j=j+1
	Marker_Style = int(20+j)
	text_file.write("    [BEGIN_PLOT]\n" )
	text_file.write("        Plot_Color = 'kRed+"+str(j)+  "';\n" )
	text_file.write("        Plot_LegEntry = '"+str(legEntry)+"';\n" )
	text_file.write("        Plot_Line_Size = '1';\n" )
	text_file.write("        Plot_Line_Style = '1';\n" )
	text_file.write("        Plot_Marker_Size = '1';\n" )
	text_file.write("        Plot_Marker_Style = '"+str(Marker_Style)+"';\n" )
	text_file.write("        Plot_Name = ''; \n" )
	text_file.write("        [BEGIN_DATA]\n" )
	text_file.write("          VAR_INDEP,VAR_DEP,VAR_INDEP_ERR,VAR_DEP_ERR\n" )
	

	for i in range(int(args.SelectRowStart),int(args.SelectRowEnd)):
		if bool(args.YaxisScale)==True:
			Y= float(ws.cell_value(i,int(args.SelectColumnY))/1000)
		else:
			Y= ws.cell_value(i,int(args.SelectColumnY))

		if bool(args.SetErrX)==True:
			ErrX=ws.cell_value(i,int(args.SelectColumnErrX))
		else:
			ErrX=0.


		if bool(args.SetErrY)==True:
			ErrY=ws.cell_value(i,int(args.SelectColumnErrY))
		else:
			ErrY=0.


		X= ws.cell_value(i,int(args.SelectColumnX)) 

		text_file.write(str(X)+","+str(Y)+","+str(ErrX)+","+str(ErrY)+"\n")
		pass



	text_file.write("        [END_DATA]\n" )
	text_file.write("     [END_PLOT]\n" )



text_file.write("[END_CANVAS]\n" )
text_file.close()