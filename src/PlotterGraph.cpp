//
//  PlotterGraph.cpp
//  
//
//  Created by Brian L Dorney on 05/10/16.
//
//

#include "PlotterGraph.h"

using std::cout;
using std::endl;
using std::make_shared;
using std::shared_ptr;

using namespace QualityControl::Plotter;

//Constructor
PlotterGraph::PlotterGraph() : mgraph_Obs(new TMultiGraph) {
    
    mgraph_Obs->SetName("mgraph_Obs");
} //End

void PlotterGraph::addPlot(TLegend & inputLegend, InfoPlot & plotInfo){
    //Initialize
    initPlot(plotInfo);
    
    //Check to make sure it initialized successfully
    if ( !(m_map_graphs.count(plotInfo.m_strName) > 0 ) ) {
        cout<<"PlotterGraph::makePlots() - Plot:\n";
        cout<<"\t"<<plotInfo.m_strName<<endl;
        cout<<"\tDid not initialize correctly, please cross-check input!\n";
        cout<<"\tExiting!!!\n";
        
        return;
    }
    
    //Get this plot
    auto graphPtr = m_map_graphs[plotInfo.m_strName];
    
    //Set the Style
    graphPtr->SetLineColor(plotInfo.m_iColor);
    graphPtr->SetLineStyle(plotInfo.m_iStyleLine);
    graphPtr->SetLineWidth(plotInfo.m_fSizeLine);
    
    graphPtr->SetMarkerColor(plotInfo.m_iColor);
    graphPtr->SetMarkerStyle(plotInfo.m_iStyleMarker);
    graphPtr->SetMarkerSize(plotInfo.m_fSizeMarker);
    
    graphPtr->GetXaxis()->SetTitle(plotInfo.m_strTitle_X.c_str() );
    graphPtr->GetXaxis()->SetRangeUser(plotInfo.m_fXAxis_Min, plotInfo.m_fXAxis_Max);
    
    graphPtr->GetYaxis()->SetTitle(plotInfo.m_strTitle_Y.c_str() );
    graphPtr->GetYaxis()->SetRangeUser(plotInfo.m_fYAxis_Min, plotInfo.m_fYAxis_Max);
    
    //Add to the multigraph
    mgraph_Obs->Add( graphPtr.get() );
    
    //Add to the Legend
    inputLegend.AddEntry(graphPtr.get(), plotInfo.m_strLegEntry.c_str(), "LPE" );
    
    //Draw the plot
    //m_canv->cd();
    //graphPtr->Draw(plotInfo.m_strOptionDraw.c_str() );
    
    return;
} //End PlotterGraph::addPlot()

void PlotterGraph::drawPlots(){
    m_canv->cd();
    //(*m_map_graphs.begin()).second->Draw("AP");
    mgraph_Obs->Draw("samePE1");
    
    return;
} //End PlotterGraph::drawPlots()

//Intializes the plots defined in m_canvInfo
void PlotterGraph::initPlot(InfoPlot & plotInfo){
    if( plotInfo.m_vec_DataPts.size() > 0 ){ //Case: Data Input
        //Intialize
        shared_ptr<TGraphErrors> graphPtr(new TGraphErrors(plotInfo.m_vec_DataPts.size() ) );
        
        //Set the TName
        graphPtr->SetName( plotInfo.m_strName.c_str() );
        graphPtr->SetTitle( "" );        

        //Fill the data
        int iPos;
        for (auto iterDataPt = plotInfo.m_vec_DataPts.begin(); iterDataPt != plotInfo.m_vec_DataPts.end(); ++iterDataPt) { //Loop Over Data
            iPos = std::distance(plotInfo.m_vec_DataPts.begin(), iterDataPt);
            
            graphPtr->SetPoint(iPos, (*iterDataPt).m_fX,  (*iterDataPt).m_fY );
            graphPtr->SetPointError(iPos, (*iterDataPt).m_fX_Err, (*iterDataPt).m_fY_Err );
        } //End Loop Over Data
        
        m_map_graphs[plotInfo.m_strName]=graphPtr;
    } //End Case: Data Input
    else if ( plotInfo.m_strFileName.length() > 0 ){ //Case: TObject Input
        //TFile does not manage objects
        TH1::AddDirectory(kFALSE);
        
        TFile * file_Input = new TFile(plotInfo.m_strFileName.c_str(), "READ" );
        
        //TGraphErrors graph
        shared_ptr<TGraphErrors> graphPtr = make_shared<TGraphErrors>( *((TGraphErrors*) file_Input->Get( plotInfo.m_strName.c_str() ) ) );
        
        m_map_graphs[plotInfo.m_strName]=graphPtr;
        
        file_Input->Close();
    } //End Case: TObject Input
    else{
        cout<<"PlotterGraph::initPlots() - Input Case not understood, please cross-check input!\n";
    }
    
    return;
} //End PlotterGraph::initPlots()

void PlotterGraph::write2RootFile(){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Initialize the file
    TFile * file_Output = new TFile(m_strOutputFile_Name.c_str(), m_strOutputFile_Option.c_str() );
    
    //Write TObjects
    m_canv->Write();
    for (auto iterPlot = m_map_graphs.begin(); iterPlot != m_map_graphs.end(); ++iterPlot) {
        (*iterPlot).second->Write();
    }
    
    //Close the file
    file_Output->Close();
    
    return;
} //End PlotterGraph::write2RootFile
