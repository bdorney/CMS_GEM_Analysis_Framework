
import pandas as pd
import xlrd
from array import array
import numpy as np
import ROOT as r

import argparse
from optparse import OptionParser

parser = argparse.ArgumentParser()
parser.add_argument('filename', nargs='+')
args = parser.parse_args()

par1_Short= array( 'f' )
par1_Long= array( 'f' )
Err_par1_Short= array( 'f' )
Err_par1_Long= array( 'f' )
errX_Short=array('f')
errX_Long=array('f')
Detector_Short=array('f')
Detector_Long=array('f')

for f in args.filename:
	workbook = xlrd.open_workbook(str(f))
	ws = workbook.sheet_by_index(0)

	time= [ws.cell_value(i,1) for i in range(1,61)]
	pres= [ws.cell_value(i,2) for i in range(1,61)]
	temp= [ws.cell_value(i,3) for i in range(1,61)]
	atmP= [ws.cell_value(i,4) for i in range(1,61)]
	
	time = np.array(time)
	pres=np.array(pres)
	temp=np.array(temp)
	atmP=np.array(atmP)
	
	canv = r.TCanvas("canv","canv",700,600)
	canv.cd()

	pad1 = r.TPad("pad1","",0,0,1,1)
	pad2= r.TPad("pad2","",0,0,1,1)
	pad2.SetFillStyle(4000)
	pad2.SetFrameFillStyle(0)
	pad1.Draw()
	pad1.cd()

	#============Plot Inner Pressure vs Time ===============
	gr= r.TGraph(60,time,pres)
	gr.Draw("AP")

	# For X axis
	gr.GetXaxis().SetNdivisions(505)
	gr.GetXaxis().SetTitle("Time (s)")
	gr.GetXaxis().SetRangeUser(0,3600)
	gr.GetXaxis().SetTitleSize(0.06)
	gr.GetXaxis().SetLabelSize(0.05)
	gr.GetXaxis().SetLabelOffset(0.007)
	gr.GetXaxis().SetLabelFont(42)
	gr.GetXaxis().SetLabelColor(1)
	gr.GetXaxis().SetTitleOffset(0.9)
	gr.GetXaxis().SetAxisColor(1)
	r.TStyle().SetStripDecimals(r.kTRUE)
	gr.GetXaxis().SetTickLength(0.03)
	r.TStyle().SetPadTickX(1)

	# For Y axis
	gr.GetYaxis().SetTitle("#splitline{Pressure (mbar)}{ Temperature (C) }")
	gr.GetYaxis().SetNdivisions(508)
	gr.GetYaxis().SetTitleOffset(1.1)
	gr.GetYaxis().SetRangeUser(0,35)
	gr.GetYaxis().SetTitleSize(0.06)
	gr.GetYaxis().SetLabelSize(0.05)
	gr.GetYaxis().SetLabelOffset(0.007)
	gr.GetYaxis().SetLabelFont(42)
	gr.GetYaxis().SetLabelColor(1)
	gr.GetYaxis().SetTitleOffset(1.1)
	gr.GetYaxis().SetAxisColor(1)

	r.TStyle().SetStripDecimals(r.kTRUE)
	gr.GetXaxis().SetTickLength(0.03)
	r.TStyle().SetPadTickX(1)

	gr.SetTitle("")
	gr.SetMarkerStyle(20)
	gr.SetMarkerSize(0.8)
	gr.SetMarkerColor(r.kBlue)
	gr.SetLineColor(r.kBlue)
	gr.SetLineStyle(1)

	#============Plot Temperature vs Time ===============
	gr_T= r.TGraph(60,time,temp)
	gr_T.SetMarkerStyle(20)
	gr_T.SetMarkerSize(0.8)
	gr_T.SetMarkerColor(r.kOrange)
	gr_T.SetLineColor(r.kOrange)
	gr_T.SetLineStyle(1)
	gr_T.Draw("PSAME")

	#============Plot Atm Pressure vs Time ===============
	pad2.Draw()
	pad2.cd()
	gr_Atm= r.TGraph(60,time,atmP)
	gr_Atm.SetMarkerStyle(20)
	gr_Atm.SetMarkerSize(0.8)
	gr_Atm.SetMarkerColor(r.kGray)
	gr_Atm.SetLineColor(r.kGray)
	gr_Atm.SetLineStyle(1)
	gr_Atm.SetTitle("")
	gr_Atm.Draw("PAY+")
	gr_Atm.GetYaxis().SetRangeUser(900,1000)
	gr_Atm.GetYaxis().SetTitle("Atm Pressure (mbar)")
	gr_Atm.GetYaxis().SetTitleSize(0.05)
	gr_Atm.GetYaxis().SetTitleOffset(1.1)

	# For X axis
	gr_Atm.GetXaxis().SetRangeUser(0,3600)
	gr_Atm.GetXaxis().SetNdivisions(505)
	gr_Atm.GetXaxis().SetLabelOffset(999)
	gr_Atm.GetXaxis().SetLabelSize(0)
	gr_Atm.GetXaxis().SetTickLength(0)
	r.TStyle().SetPadTickX(1)

	# For Y axis
	gr_Atm.GetYaxis().SetNdivisions(508)
	gr_Atm.GetYaxis().SetTitleOffset(1.2)
	gr_Atm.GetYaxis().SetTitleSize(0.06)
	gr_Atm.GetYaxis().SetLabelSize(0.05)
	gr_Atm.GetYaxis().SetLabelOffset(0.007)
	gr_Atm.GetYaxis().SetLabelFont(42)
	gr_Atm.GetYaxis().SetLabelColor(1)
	gr_Atm.GetYaxis().SetTitleOffset(1.1)
	gr_Atm.GetYaxis().SetAxisColor(1)

	r.TStyle().SetStripDecimals(r.kTRUE)
	gr.GetXaxis().SetTickLength(0.03)
	r.TStyle().SetPadTickX(1)

	#===================Canvas styling==========================
	pad1.cd()
	r.gPad.SetLeftMargin(0.16)
	r.gPad.SetRightMargin(0.16)
	r.gPad.SetTopMargin(0.08)
	r.gPad.SetBottomMargin(0.14)

	pad2.cd()
	r.gPad.SetLeftMargin(0.16)
	r.gPad.SetRightMargin(0.16)
	r.gPad.SetTopMargin(0.08)
	r.gPad.SetBottomMargin(0.14)

	# For the Legend
	pad2.cd()
	leg= r.TLegend()
	leg.SetX1NDC(0.55)
	leg.SetX2NDC(0.80)
	leg.SetY1NDC(0.17)
	leg.SetY2NDC(0.42)
	leg.SetBorderSize(0)
	leg.SetFillStyle(0)
	leg.SetTextSize(0.04)
	leg.AddEntry(gr,"Pressure","lep")
	leg.AddEntry(gr_T,"Temperature","lep")
	leg.AddEntry(gr_Atm,"Atm Pressure","lep")
	leg.Draw()

	# For the Pad
	r.TStyle().SetPadBorderMode(0);
	r.TStyle().SetPadBorderSize(1);
	r.TStyle().SetPadColor(r.kWhite);
	r.TStyle().SetPadGridX(r.false);
	r.TStyle().SetPadGridY(r.false);
	r.TStyle().SetGridColor(0);
	r.TStyle().SetGridStyle(3);
	r.TStyle().SetGridWidth(1);

	# For the frame:

	r.TStyle().SetFrameBorderMode(0);
	r.TStyle().SetFrameBorderSize(1);
	r.TStyle().SetFrameFillColor(0);
	r.TStyle().SetFrameFillStyle(0);
	r.TStyle().SetFrameLineColor(1);
	r.TStyle().SetFrameLineStyle(1);
	r.TStyle().SetFrameLineWidth(1);

	#========= Latex lines : logo etc =============

	logo= r.TLatex(0.6,0.8,"")
	logo.SetNDC()
	logo.DrawLatex(0.17,0.93,"#scale[1.2]{CMS} #font[52]{Preliminary}")
	logo.SetTextFont(4)
	pad2.Update()
	
	#==========================================================
	b="xlsm"
	a = str(f)
	a= a.replace(b,"png")
	canv.SaveAs(str(a))

	#============== Fit the Pres vs time Curve ===============

	gr.Fit("expo")
	fit_f=gr.GetFunction("expo") 
	Gcharacter=int(f.index('G'))
	Det_Type=f[int(Gcharacter+7):int(Gcharacter+8)]
	Det_Num=float(f[int(Gcharacter+17):int(Gcharacter+18)])

	if Det_Type=="S":
		par1_Short.append(fit_f.GetParameter(1))
		Err_par1_Short.append(fit_f.GetParError(1))
		errX_Short.append(0.0)
		Detector_Short.append(Det_Num)
	elif Det_Type=="L":
		par1_Long.append(fit_f.GetParameter(1))
		Err_par1_Long.append(fit_f.GetParError(1))
		errX_Long.append(0.0)
		Detector_Long.append(Det_Num)
		pass
	pass

canv1 = r.TCanvas("canv1","canv1",700,600)
canv1.cd()
pad3 = r.TPad("pad3","",0,0,1,1)
pad3.Draw()
pad3.cd()

r.gPad.SetLeftMargin(0.16)
r.gPad.SetRightMargin(0.16)
r.gPad.SetTopMargin(0.08)
r.gPad.SetBottomMargin(0.14)

Detector_Short=np.array(Detector_Short)
errX_Short=np.array(errX_Short)
par1_Short=np.array(par1_Short)
Err_par1_Short=np.array(Err_par1_Short)

Detector_Long=np.array(Detector_Long)
errX_Long=np.array(errX_Long)
par1_Long=np.array(par1_Long)
Err_par1_Long=np.array(Err_par1_Long)

tgr_Short= r.TGraphErrors(len(Err_par1_Short),Detector_Short,par1_Short,errX_Short,Err_par1_Short)
tgr_Long= r.TGraphErrors(len(Err_par1_Long),Detector_Long,par1_Long,errX_Long,Err_par1_Long)	

tgr_Short.Draw("AP")
tgr_Long.Draw("SAMEP")

#//------For X axis---------//

tgr_Short.GetXaxis().SetNdivisions(505)
tgr_Short.GetXaxis().SetTitle("Detector #")
tgr_Short.GetXaxis().SetTitleSize(0.06)
tgr_Short.GetXaxis().SetLabelSize(0.05)
tgr_Short.GetXaxis().SetLabelOffset(0.007)
tgr_Short.GetXaxis().SetLabelFont(42)
tgr_Short.GetXaxis().SetLabelColor(1)
tgr_Short.GetXaxis().SetTitleOffset(0.9)
tgr_Short.GetXaxis().SetAxisColor(1)
tgr_Short.GetXaxis().SetTickLength(0.03)

#//------For Y axis---------//

tgr_Short.GetYaxis().SetTitle("Fit Par1")
tgr_Short.GetYaxis().SetNdivisions(508)
#tgr_Short.GetYaxis().SetRangeUser(-e-05,-e-03)
tgr_Short.GetYaxis().SetTitleSize(0.06)
tgr_Short.GetYaxis().SetLabelSize(0.05)
tgr_Short.GetYaxis().SetLabelOffset(0.007)
tgr_Short.GetYaxis().SetLabelFont(42)
tgr_Short.GetYaxis().SetLabelColor(1)
tgr_Short.GetYaxis().SetTitleOffset(1)
tgr_Short.GetYaxis().SetAxisColor(1)
tgr_Short.GetXaxis().SetTickLength(0.03)

tgr_Short.SetTitle("")
tgr_Short.SetMarkerStyle(24)
tgr_Short.SetMarkerSize(0.8)
tgr_Short.SetMarkerColor(r.kBlue)
tgr_Short.SetLineColor(r.kBlue)
tgr_Short.SetLineStyle(1)

tgr_Long.SetMarkerStyle(20)
tgr_Long.SetMarkerSize(0.8)
tgr_Long.SetMarkerColor(r.kRed)
tgr_Long.SetLineColor(r.kRed)
tgr_Long.SetLineStyle(1)

logo= r.TLatex(0.6,0.8,"")
logo.SetNDC()
logo.DrawLatex(0.17,0.93,"#scale[1.2]{CMS} #font[52]{Preliminary}")
logo.SetTextFont(4)

leg= r.TLegend()
leg.SetX1NDC(0.55)
leg.SetX2NDC(0.80)
leg.SetY1NDC(0.17)
leg.SetY2NDC(0.42)
leg.SetBorderSize(0)
leg.SetFillStyle(0)
leg.SetTextSize(0.04)
leg.AddEntry(tgr_Short,"Short Detectors","lep")
leg.AddEntry(tgr_Long,"Long Detectors","lep")
leg.Draw()

canv1.SaveAs("Par1_Comparison.png")




