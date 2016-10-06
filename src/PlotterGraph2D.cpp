//
//  PlotterGraph2D.cpp
//  
//
//  Created by Brian L Dorney on 05/10/16.
//
//

#include "PlotterGraph2D.h"

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;
using std::string;

using namespace QualityControl::Plotter;

//Constructor
PlotterGraph2D::PlotterGraph2D() : m_g2D_Obs(new TGraph2D ) {
    
    //Placeholder
    
    //mgraph_Obs->SetName("mgraph_Obs");
} //End

void PlotterGraph2D::addPlot(TLegend & inputLegend, InfoPlot & plotInfo){
    //Initialize
    initPlot(plotInfo);
    
    //Check to make sure it initialized successfully
    //Should never be true since constructor initializes...
    if ( m_g2D_Obs == nullptr ) {
        cout<<"PlotterGraph2D::makePlots() - Plot:\n";
        cout<<"\t"<<plotInfo.m_strName<<endl;
        cout<<"\tDid not initialize correctly, please cross-check input!\n";
        cout<<"\tExiting!!!\n";
        
        return;
    }
    
    //Add to the Legend
    inputLegend.AddEntry(m_g2D_Obs.get(), plotInfo.m_strLegEntry.c_str(), "LPE" );
    
    return;
} //End PlotterGraph2D::addPlot()

void PlotterGraph2D::drawPlots(){
    //Draw (root hack so axis pointers exist)
    m_canv->cd();
    m_g2D_Obs->Draw( m_canvInfo.m_strOptionDraw.c_str() );

    //Set the style
    if (m_bMonoColor) {
        const Int_t NRGBs = 5;
        const Int_t NCont = 255;
        Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
        Double_t red[NRGBs]   = { 0.00, 0.00, 0.00, 0.00, 0.00 };
        Double_t green[NRGBs] = { 0.00, 0.00, 0.00, 0.00, 0.00 };
        Double_t blue[NRGBs]  = { 0.0, 0.25, 0.5, 0.75, 1.00 };
        TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
        m_tdrStyle->SetNumberContours(NCont);
        
        m_tdrStyle->cd();
    }

    m_g2D_Obs->GetXaxis()->SetTitle(m_canvInfo.m_strTitle_X.c_str() );
    if( m_canvInfo.m_bXAxis_UserRange ){
        m_g2D_Obs->GetXaxis()->SetRangeUser(m_canvInfo.m_fXAxis_Min, m_canvInfo.m_fXAxis_Max);
    }
    
    m_g2D_Obs->GetYaxis()->SetTitle(m_canvInfo.m_strTitle_Y.c_str() );
    if( m_canvInfo.m_bYAxis_UserRange ){
        m_g2D_Obs->GetYaxis()->SetRangeUser(m_canvInfo.m_fYAxis_Min, m_canvInfo.m_fYAxis_Max);
    }
    
    //Draw (for realz)
    m_canv->cd();
    m_g2D_Obs->Draw( m_canvInfo.m_strOptionDraw.c_str() );
    
    return;
} //End PlotterGraph2D::drawPlots()

//Intializes the plots defined in m_canvInfo
void PlotterGraph2D::initPlot(InfoPlot & plotInfo){
    if( plotInfo.m_vec_DataPts.size() > 0 ){ //Case: Data Input
        //Intialize
        shared_ptr<TGraph2D> g2DPtr(new TGraph2D( plotInfo.m_vec_DataPts.size() ) );
        m_g2D_Obs = g2DPtr;
        
        //Set the TName
        m_g2D_Obs->SetName( plotInfo.m_strName.c_str() );
        m_g2D_Obs->SetTitle( "" );

        //Fill the data
        int iPos;
        for (auto iterDataPt = plotInfo.m_vec_DataPts.begin(); iterDataPt != plotInfo.m_vec_DataPts.end(); ++iterDataPt) { //Loop Over Data
            iPos = std::distance(plotInfo.m_vec_DataPts.begin(), iterDataPt);
            
            m_g2D_Obs->SetPoint(iPos, (*iterDataPt).m_fX,  (*iterDataPt).m_fY, (*iterDataPt).m_fZ);
            //g2DPtr->SetPointError(iPos, (*iterDataPt).m_fX_Err, (*iterDataPt).m_fY_Err );
        } //End Loop Over Data
        
        //m_map_g2D[plotInfo.m_strName]=g2DPtr;
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
        
        //Get the Graph
        m_g2D_Obs = make_shared<TGraph2D>( *((TGraph2D*) file_Input->Get( strTmpName.c_str() ) ) );
        
        //m_map_g2D[plotInfo.m_strName]=g2DPtr;
        
        file_Input->Close();
    } //End Case: TObject Input
    else{
        cout<<"PlotterGraph2D::initPlots() - Input Case not understood, please cross-check input!\n";
    }
    
    return;
} //End PlotterGraph2D::initPlots()

void PlotterGraph2D::write2RootFile(){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Initialize the file
    TFile * file_Output = new TFile(m_strOutputFile_Name.c_str(), m_strOutputFile_Option.c_str() );
    
    //Write TObjects
    m_canv->Write();
    m_g2D_Obs->Write();
    
    //Close the file
    file_Output->Close();
    
    return;
} //End PlotterGraph2D::write2RootFile
