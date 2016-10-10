//
//  PlotterGeneric.cpp
//  
//
//  Created by Brian L Dorney on 05/10/16.
//
//

#include "PlotterGeneric.h"

using std::cout;
using std::endl;
using std::get;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::tuple;

using QualityControl::Timing::getString;

using namespace QualityControl::Plotter;

//Constructor
PlotterGeneric::PlotterGeneric() :
    m_canv(new TCanvas),
    m_tdrStyle(new TStyle) {
        m_bIsPrelim = true;
        m_bSaveCanvases = true;
        m_iLogoPos = 10;
        
        m_strOutputFile_Name = "PlotterOutput.root";
        m_strOutputFile_Option = "UPDATE";
        
        lumi.setDrawLogo(false);
        lumi.setPreliminary(true);
        
        setStyle();
} //End PlotterGeneric()

//If m_canvInfo isn't set this is gonna be a short trip...
void PlotterGeneric::plotAndStore(){
    //Initialize the canvas
    initCanv();
    
    //Make the Legend
    TLegend leg(m_canvInfo.m_fLegNDCPos_X1, m_canvInfo.m_fLegNDCPos_Y1, m_canvInfo.m_fLegNDCPos_X2, m_canvInfo.m_fLegNDCPos_Y2 );
    
    //Add all defined plots
    for (auto iterPlot = m_canvInfo.m_map_infoPlot.begin(); iterPlot != m_canvInfo.m_map_infoPlot.end(); ++iterPlot) {
        
        //Add "same" to the draw option if it is not present already
        /*if ( std::distance(m_canvInfo.m_map_infoPlot.begin(), iterPlot) > 0
            && ( (*iterPlot).second.m_strOptionDraw.find("same") == std::string::npos 
                || (*iterPlot).second.m_strOptionDraw.find("SAME") == std::string::npos ) ) {
            (*iterPlot).second.m_strOptionDraw = "same" + (*iterPlot).second.m_strOptionDraw;
        }*/

        addPlot(leg, (*iterPlot).second);
    } //End Loop Over Input Plots

    //Draw plots
    drawPlots();
    
    //Draw each latex line
    for (int i=0; i<m_canvInfo.m_vec_LatexNPos.size(); ++i) {
        drawLatex(m_canvInfo.m_vec_LatexNPos[i]);
    }

    //Draw legend
    if (m_canvInfo.m_bDrawLeg) {
        leg.Draw("same");
    }
    
    //Draw "CMS"
    drawCMS();
    
    //Update
    m_canv->Update();
    m_canv->RedrawAxis();
    m_canv->GetFrame()->Draw();

    //Save output
    write2RootFile();
    if (m_bSaveCanvases) {
        save2ImgFile(m_canv);
    }
    
    return;
} //End PlotterGeneric::plotAndStore()

void PlotterGeneric::drawLatex(std::tuple<float, float, std::string> tupleTexLine){
    //Define the latexLine
    TLatex latex;
    
    //Set the Latex style
    latex.SetTextFont(42);
    latex.SetTextAngle(0);
    latex.SetTextColor(kBlack);
    latex.SetTextSize(0.04);
    latex.SetTextAlign(12);
    
    //Draw
    m_canv->cd();
    latex.DrawLatexNDC(get<0>(tupleTexLine), get<1>(tupleTexLine), (get<2>(tupleTexLine) ).c_str() );
    
    return;
} //End PlotterGeneric::drawLatex()

//Generates a TCanvas based on the user input
//Set additional style based on:
//  https://ghm.web.cern.ch/ghm/plots/MacroExample/myMacro.C
//std::shared_ptr<TCanvas> PlotterGeneric::getCanvas(){
void PlotterGeneric::initCanv(){
    //Update the canvas name
    string strTempName = m_canvInfo.m_strName;
    strTempName += "_";
    strTempName += getString(m_canvInfo.m_iSize_X);
    strTempName += "_";
    strTempName += getString(m_canvInfo.m_iSize_Y);
    strTempName += "_";
    strTempName += getString(m_iLogoPos);
    if(m_bIsPrelim) {               strTempName+= "_prelim"; }
    if( m_iLogoPos%10==0 ){         strTempName += "_out"; }
    else if( m_iLogoPos%10==1 ){    strTempName += "_left"; }
    else if( m_iLogoPos%10==2 ){    strTempName += "_center"; }
    else if( m_iLogoPos%10==3 ){    strTempName += "_right"; }
    
    m_canv = make_shared<TCanvas>(new TCanvas(strTempName.c_str(),m_canvInfo.m_strTitle.c_str(),m_canvInfo.m_iSize_X,m_canvInfo.m_iSize_Y ) );
    m_canv->SetName( strTempName.c_str() );

    //Grid
    m_canv->cd()->SetGridx(m_canvInfo.m_bGrid_X);
    m_canv->cd()->SetGridy(m_canvInfo.m_bGrid_Y);
    
    //Log
    m_canv->cd()->SetLogx(m_canvInfo.m_bLog_X);
    m_canv->cd()->SetLogy(m_canvInfo.m_bLog_Y);
    
    //Determine values for margins
    /*float fMargin_Top   = 0.08;// * m_canvInfo.m_iSize_Y;
    float fMargin_Bot   = 0.12;// * m_canvInfo.m_iSize_Y;
    float fMargin_Lf    = 0.12;// * m_canvInfo.m_iSize_X;
    float fMargin_Rt    = 0.04;// * m_canvInfo.m_iSize_X;*/
    
    //Set additional style:
    m_canv->SetFillColor(0);
    m_canv->SetBorderMode(0);
    m_canv->SetFrameFillStyle(0);
    m_canv->SetFrameBorderMode(0);
    m_canv->SetLeftMargin( m_canvInfo.m_fMargin_Lf );
    m_canv->SetRightMargin( m_canvInfo.m_fMargin_Rt );
    m_canv->SetTopMargin( m_canvInfo.m_fMargin_Top );
    m_canv->SetBottomMargin( m_canvInfo.m_fMargin_Bot );
    m_canv->SetTickx(0);
    m_canv->SetTicky(0);
    
    return;
} //End getCanvas

//Makes the plots defined in m_canvInfo
//To be over-ridded by inherited classes
void PlotterGeneric::addPlot(TLegend & inputLegend, InfoPlot & plotInfo){
    
    cout<<"Brian it doesn't work, this method should be over-ridden\n";
    
    return;
} //End PlotterGeneric::addPlot

//Makes the plots defined in m_canvInfo
//To be over-ridded by inherited classes
void PlotterGeneric::drawPlots(){
    
    cout<<"Brian it doesn't work, this method should be over-ridden\n";
    
    return;
} //End PlotterGeneric::drawPlots

//Makes the plots defined in m_canvInfo
//To be over-ridded by inherited classes
void PlotterGeneric::initPlot(InfoPlot & plotInfo){
    
    cout<<"Brian it doesn't work, this method should be over-ridden\n";
    
    return;
} //End PlotterGeneric::initPlot

//Saves inputCanv as a *.png file
//The file is placed in the working directory
//The name of the file is the TName of the canvas
void PlotterGeneric::save2ImgFile(std::shared_ptr<TCanvas> inputCanvas){
    //Variable Declaration
    string strName = inputCanvas->GetName();
    
    inputCanvas->SaveAs( ( strName + ".pdf" ).c_str(), "RECREATE" );
    inputCanvas->SaveAs( ( strName + ".png" ).c_str(), "RECREATE" );
    
    return;
} //End PlotterGeneric::save2ImgFile()

//Sets the style
//Follows the tdrStyle.C macro available from:
//  https://ghm.web.cern.ch/ghm/plots/
void PlotterGeneric::setStyle(){
    m_tdrStyle->SetCanvasBorderMode(0);
    m_tdrStyle->SetCanvasColor(kWhite);
    m_tdrStyle->SetCanvasDefH(600); //Height of canvas
    m_tdrStyle->SetCanvasDefW(600); //Width of canvas
    m_tdrStyle->SetCanvasDefX(0);   //POsition on screen
    m_tdrStyle->SetCanvasDefY(0);
    
    // For the Pad:
    m_tdrStyle->SetPadBorderMode(0);
    // m_tdrStyle->SetPadBorderSize(Width_t size = 1);
    m_tdrStyle->SetPadColor(kWhite);
    m_tdrStyle->SetPadGridX(false);
    m_tdrStyle->SetPadGridY(false);
    m_tdrStyle->SetGridColor(0);
    m_tdrStyle->SetGridStyle(3);
    m_tdrStyle->SetGridWidth(1);
    
    // For the frame:
    m_tdrStyle->SetFrameBorderMode(0);
    m_tdrStyle->SetFrameBorderSize(1);
    m_tdrStyle->SetFrameFillColor(0);
    m_tdrStyle->SetFrameFillStyle(0);
    m_tdrStyle->SetFrameLineColor(1);
    m_tdrStyle->SetFrameLineStyle(1);
    m_tdrStyle->SetFrameLineWidth(1);
    
    // For the histo:
    // m_tdrStyle->SetHistFillColor(1);
    // m_tdrStyle->SetHistFillStyle(0);
    m_tdrStyle->SetHistLineColor(1);
    m_tdrStyle->SetHistLineStyle(0);
    m_tdrStyle->SetHistLineWidth(1);
    // m_tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
    // m_tdrStyle->SetNumberContours(Int_t number = 20);
    
    m_tdrStyle->SetEndErrorSize(2);
    // m_tdrStyle->SetErrorMarker(20);
    //m_tdrStyle->SetErrorX(0.);
    
    m_tdrStyle->SetMarkerStyle(20);
    
    //For the fit/function:
    m_tdrStyle->SetOptFit(1);
    m_tdrStyle->SetFitFormat("5.4g");
    m_tdrStyle->SetFuncColor(2);
    m_tdrStyle->SetFuncStyle(1);
    m_tdrStyle->SetFuncWidth(1);
    
    //For the date:
    m_tdrStyle->SetOptDate(0);
    // m_tdrStyle->SetDateX(Float_t x = 0.01);
    // m_tdrStyle->SetDateY(Float_t y = 0.01);
    
    // For the statistics box:
    m_tdrStyle->SetOptFile(0);
    m_tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
    m_tdrStyle->SetStatColor(kWhite);
    m_tdrStyle->SetStatFont(42);
    m_tdrStyle->SetStatFontSize(0.025);
    m_tdrStyle->SetStatTextColor(1);
    m_tdrStyle->SetStatFormat("6.4g");
    m_tdrStyle->SetStatBorderSize(1);
    m_tdrStyle->SetStatH(0.1);
    m_tdrStyle->SetStatW(0.15);
    // m_tdrStyle->SetStatStyle(Style_t style = 1001);
    // m_tdrStyle->SetStatX(Float_t x = 0);
    // m_tdrStyle->SetStatY(Float_t y = 0);
    
    // Margins:
    m_tdrStyle->SetPadTopMargin(0.05);
    m_tdrStyle->SetPadBottomMargin(0.13);
    m_tdrStyle->SetPadLeftMargin(0.16);
    m_tdrStyle->SetPadRightMargin(0.02);
    
    // For the Global title:
    
    m_tdrStyle->SetOptTitle(0);
    m_tdrStyle->SetTitleFont(42);
    m_tdrStyle->SetTitleColor(1);
    m_tdrStyle->SetTitleTextColor(1);
    m_tdrStyle->SetTitleFillColor(10);
    m_tdrStyle->SetTitleFontSize(0.05);
    // m_tdrStyle->SetTitleH(0); // Set the height of the title box
    // m_tdrStyle->SetTitleW(0); // Set the width of the title box
    // m_tdrStyle->SetTitleX(0); // Set the position of the title box
    // m_tdrStyle->SetTitleY(0.985); // Set the position of the title box
    // m_tdrStyle->SetTitleStyle(Style_t style = 1001);
    // m_tdrStyle->SetTitleBorderSize(2);
    
    // For the axis titles:
    
    m_tdrStyle->SetTitleColor(1, "XYZ");
    m_tdrStyle->SetTitleFont(42, "XYZ");
    m_tdrStyle->SetTitleSize(0.06, "XYZ");
    // m_tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
    // m_tdrStyle->SetTitleYSize(Float_t size = 0.02);
    m_tdrStyle->SetTitleXOffset(0.9);
    m_tdrStyle->SetTitleYOffset(1.25);
    // m_tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset
    
    // For the axis labels:
    
    m_tdrStyle->SetLabelColor(1, "XYZ");
    m_tdrStyle->SetLabelFont(42, "XYZ");
    m_tdrStyle->SetLabelOffset(0.007, "XYZ");
    m_tdrStyle->SetLabelSize(0.05, "XYZ");
    
    // For the axis:
    
    m_tdrStyle->SetAxisColor(1, "XYZ");
    //m_tdrStyle->SetStripDecimals(kTRUE);
    m_tdrStyle->SetStripDecimals(kFALSE);
    m_tdrStyle->SetTickLength(0.03, "XYZ");
    m_tdrStyle->SetNdivisions(510, "XYZ");
    m_tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
    m_tdrStyle->SetPadTickY(1);
    
    // Change for log plots:
    m_tdrStyle->SetOptLogx(0);
    m_tdrStyle->SetOptLogy(0);
    m_tdrStyle->SetOptLogz(0);
    
    // Postscript options:
    m_tdrStyle->SetPaperSize(20.,20.);
    // m_tdrStyle->SetLineScalePS(Float_t scale = 3);
    // m_tdrStyle->SetLineStyleString(Int_t i, const char* text);
    // m_tdrStyle->SetHeaderPS(const char* header);
    // m_tdrStyle->SetTitlePS(const char* pstitle);
    
    // m_tdrStyle->SetBarOffset(Float_t baroff = 0.5);
    // m_tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
    // m_tdrStyle->SetPaintTextFormat(const char* format = "g");
    // m_tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
    // m_tdrStyle->SetTimeOffset(Double_t toffset);
    // m_tdrStyle->SetHistMinimumZero(kTRUE);
    
    m_tdrStyle->SetHatchesLineWidth(5);
    m_tdrStyle->SetHatchesSpacing(0.05);
    
    // Change for Palette
    /*const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };*/
    PlotColorPaletteRGB palette_RGB;
    TColor::CreateGradientColorTable(palette_RGB.m_NRGBs, palette_RGB.m_stops, palette_RGB.m_red, palette_RGB.m_green, palette_RGB.m_blue, palette_RGB.m_NCont);
    m_tdrStyle->SetNumberContours(palette_RGB.m_NCont);
    
    m_tdrStyle->cd();
} //End PlotterGeneric::setStyle()

//Saves all TObjects to an output ROOT file
//To be over-ridded by inherited classes
void PlotterGeneric::write2RootFile(){
    
    cout<<"Brian it doesn't work, this method should be over-ridden\n";
    
    return;
} //End PlotterGeneric::makePlots
