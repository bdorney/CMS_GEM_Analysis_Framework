//
//  PlotterHisto2D.cpp
//  
//
//  Created by Brian L Dorney on 05/10/16.
//
//

#include "PlotterHisto2D.h"

using std::cout;
using std::endl;
using std::get;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::tuple;

using namespace QualityControl::Plotter;

//Constructor
PlotterHisto2D::PlotterHisto2D() : hObs2D(new TH2F ) {
    
    //Placeholder
    
} //End

void PlotterHisto2D::addPlot(TLegend & inputLegend, InfoPlot & plotInfo){
    //Initialize
    initPlot(plotInfo);
    
    //Check to make sure it initialized successfully
    //Should never be true since constructor initializes...
    if ( hObs2D == nullptr ) {
        cout<<"PlotterHisto2D::makePlots() - Plot:\n";
        cout<<"\t"<<plotInfo.m_strName<<endl;
        cout<<"\tDid not initialize correctly, please cross-check input!\n";
        cout<<"\tExiting!!!\n";
        
        return;
    }
    
    //Add to the Legend
    inputLegend.AddEntry(hObs2D.get(), plotInfo.m_strLegEntry.c_str(), "LPE" );
    
    return;
} //End PlotterHisto2D::addPlot()

void PlotterHisto2D::drawPlots(){
    //Draw (root hack so axis pointers exist)
    m_canv->cd();
    hObs2D->Draw();

    //Make the palette monocolored?
    if (m_bMonoColor) {
        PlotColorPaletteMono palette_Mono;
        TColor::CreateGradientColorTable(palette_Mono.m_NRGBs, palette_Mono.m_stops, palette_Mono.m_red, palette_Mono.m_green, palette_Mono.m_blue, palette_Mono.m_NCont);
        m_tdrStyle->SetNumberContours(palette_Mono.m_NCont);
        
        m_tdrStyle->cd();
    }

    //hack to get the axis drawn correctly
    //auto histo = hObs2D->GetHistogram();

    //Set Style - X axis
    hObs2D->GetXaxis()->SetTitle(m_canvInfo.m_strTitle_X.c_str() );
    hObs2D->GetXaxis()->SetNdivisions(m_canvInfo.m_iXAxis_NDiv);
    if( m_canvInfo.m_fXAxis_Title_Offset > 0 ){
        hObs2D->GetXaxis()->SetTitleOffset(m_canvInfo.m_fXAxis_Title_Offset);
    }
    if( m_canvInfo.m_bXAxis_UserRange ){
        hObs2D->GetXaxis()->SetRangeUser(m_canvInfo.m_fXAxis_Min, m_canvInfo.m_fXAxis_Max);
    }
    
    //Set Style - X axis
    hObs2D->GetYaxis()->SetTitle(m_canvInfo.m_strTitle_Y.c_str() );
    hObs2D->GetYaxis()->SetNdivisions(m_canvInfo.m_iYAxis_NDiv);
    if( m_canvInfo.m_fYAxis_Title_Offset > 0 ){
        hObs2D->GetYaxis()->SetTitleOffset(m_canvInfo.m_fYAxis_Title_Offset);
    }
    if( m_canvInfo.m_bYAxis_UserRange ){
        hObs2D->GetYaxis()->SetRangeUser(m_canvInfo.m_fYAxis_Min, m_canvInfo.m_fYAxis_Max);
    }
    
    //Set Style - X axis
    hObs2D->GetZaxis()->SetTitle(m_canvInfo.m_strTitle_Z.c_str() );
    //hObs2D->GetZaxis()->SetDecimals(true);
    if( m_canvInfo.m_fZAxis_Title_Offset > 0 ){
        hObs2D->GetZaxis()->SetTitleOffset(m_canvInfo.m_fZAxis_Title_Offset);
    }
    if( m_canvInfo.m_bZAxis_UserRange ){
        hObs2D->GetZaxis()->SetRangeUser(m_canvInfo.m_fZAxis_Min, m_canvInfo.m_fZAxis_Max);
    }

    //Draw (for realz)
    m_canv->cd();
    hObs2D->Draw( m_canvInfo.m_strOptionDraw.c_str() );
    
	string strTempDrawOpt = m_canvInfo.m_strOptionDraw;
	transform(strTempDrawOpt.begin(),strTempDrawOpt.end(),strTempDrawOpt.begin(), toupper);

	//Draw Axis labels in special case
    /*if( strTempDrawOpt.find("TRI2Z") != std::string::npos 
	|| strTempDrawOpt.find("COLZ") != std::string::npos 
	|| strTempDrawOpt.find("SURF") != std::string::npos ){
		//Draw X-Axis Label
		float fAxisTitlePos_Y = 0.4 * m_canv->GetBottomMargin();
		tuple<float,float,string> tuple_texLine = std::make_tuple(-1,fAxisTitlePos_Y,m_canvInfo.m_strTitle_X);
		drawLatex(tuple_texLine,0);

		//Draw Y-Axis Label
		float fAxisTitlePos_X = 0.15 * m_canv->GetLeftMargin();
		tuple_texLine = std::make_tuple(fAxisTitlePos_X,-1,m_canvInfo.m_strTitle_Y);
		drawLatex(tuple_texLine,90);

		//Draw Z-Axis Label
		fAxisTitlePos_X = 1. - 0.15 * m_canv->GetRightMargin();
		tuple_texLine = std::make_tuple(fAxisTitlePos_X,-1,m_canvInfo.m_strTitle_Z);
		drawLatex(tuple_texLine,90);
	}*/
    
    return;
} //End PlotterHisto2D::drawPlots()

//Intializes the plots defined in m_canvInfo
void PlotterHisto2D::initPlot(InfoPlot & plotInfo){
    if( plotInfo.m_vec_DataPts.size() > 0 ){ //Case: Data Input
        cout<<"PlotterHisto2D::initPlot() - Data Input for Histogram Not Supported\n";
        cout<<"PlotterHisto2D::initPlot() - You should be using a graph instead!\n";
    } //End Case: Data Input
    else if ( plotInfo.m_strFileName.length() > 0 ){ //Case: TObject Input
        //TFile does not manage objects
        TH1::AddDirectory(kFALSE);
        
        TFile * file_Input = new TFile(plotInfo.m_strFileName.c_str(), "READ" );
        
        //Setup TObject name w/path
        string strTmpName = plotInfo.m_strName;
        if (plotInfo.m_strFilePath.length() > 0) {
            strTmpName = plotInfo.m_strFilePath + "/" + strTmpName;
        }
       
        //cout<<"strTmpName = " << strTmpName << endl;
 
        //Get the Graph
        hObs2D = make_shared<TH2F>( *((TH2F*) file_Input->Get( strTmpName.c_str() ) ) );
        
        //m_map_g2D[plotInfo.m_strName]=g2DPtr;
        
        file_Input->Close();
    } //End Case: TObject Input
    else{
        cout<<"PlotterHisto2D::initPlots() - Input Case not understood, please cross-check input!\n";
    }
    
    return;
} //End PlotterHisto2D::initPlots()

void PlotterHisto2D::write2RootFile(){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Initialize the file
    TFile * file_Output = new TFile(m_strOutputFile_Name.c_str(), m_strOutputFile_Option.c_str() );
    
    //Write TObjects
    m_canv->Write();
    hObs2D->Write();
    
    //Close the file
    file_Output->Close();
    
    return;
} //End PlotterHisto2D::write2RootFile()
