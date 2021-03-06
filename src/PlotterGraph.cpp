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
using std::multimap;
using std::pair;
using std::shared_ptr;
using std::string;

using namespace QualityControl::Plotter;

//Constructor
PlotterGraph::PlotterGraph() : m_mgraph_Obs(new TMultiGraph) {
    
    m_mgraph_Obs->SetName("m_mgraph_Obs");
} //End

void PlotterGraph::addPlot(TLegend & inputLegend, InfoPlot & plotInfo){
    //Initialize
    initPlot(plotInfo);
    
    //Check to make sure it initialized successfully
    if ( !(m_map_graphs.count(plotInfo.m_strName) >= (m_map_iSameNameCount[plotInfo.m_strName]) ) ) {
        cout<<"PlotterGraph::makePlots() - Plot:\n";
        cout<<"\t"<<plotInfo.m_strName<<endl;
        cout<<"\tDid not initialize correctly, please cross-check input!\n";
        cout<<"\tExiting!!!\n";
        
        return;
    }
    
    //Get this plot
	pair<multimap<string,shared_ptr<TGraph> >::iterator, multimap<string,shared_ptr<TGraph> >::iterator> pair_iterThisPlotName = m_map_graphs.equal_range(plotInfo.m_strName);
	multimap<string,shared_ptr<TGraph> >::iterator iterThisPlot = pair_iterThisPlotName.first;
	std::advance(iterThisPlot, (m_map_iSameNameCount[plotInfo.m_strName] - 1 ) );
	auto graphPtr = ( (*iterThisPlot).second );

    //auto graphPtr = m_map_graphs[plotInfo.m_strName];
    
    //Set the Style
    graphPtr->SetLineColor(plotInfo.m_iColor);
    graphPtr->SetLineStyle(plotInfo.m_iStyleLine);
    graphPtr->SetLineWidth(plotInfo.m_fSizeLine);
    
    graphPtr->SetMarkerColor(plotInfo.m_iColor);
    graphPtr->SetMarkerStyle(plotInfo.m_iStyleMarker);
    graphPtr->SetMarkerSize(plotInfo.m_fSizeMarker);
    
    //Add to the multigraph
    m_mgraph_Obs->Add( graphPtr.get() );
    
    //Add to the Legend
    if(plotInfo.m_strLegEntry.length() > 0){
    	inputLegend.AddEntry(graphPtr.get(), plotInfo.m_strLegEntry.c_str(), "LPE" );
    }

    //Draw the plot
    //m_canv->cd();
    //graphPtr->Draw(plotInfo.m_strOptionDraw.c_str() );
    
    return;
} //End PlotterGraph::addPlot()

void PlotterGraph::drawPlots(){
    //Ensure axis is drawn!
    if( m_canvInfo.m_strOptionDraw.find("A") == std::string::npos){
        m_canvInfo.m_strOptionDraw += "A";
    }
    
    //Draw (root hack)
    m_canv->cd();
    m_mgraph_Obs->Draw( m_canvInfo.m_strOptionDraw.c_str() );

    //Set Style - X axis
    m_mgraph_Obs->GetXaxis()->SetTitle(m_canvInfo.m_strTitle_X.c_str() );
    m_mgraph_Obs->GetXaxis()->SetNdivisions(m_canvInfo.m_iXAxis_NDiv);
    if( m_canvInfo.m_fXAxis_Title_Offset > 0 ){
        m_mgraph_Obs->GetXaxis()->SetTitleOffset(m_canvInfo.m_fXAxis_Title_Offset);
    }
	if( m_canvInfo.m_bXAxis_UserRange ){
	    m_mgraph_Obs->GetXaxis()->SetRangeUser(m_canvInfo.m_fXAxis_Min, m_canvInfo.m_fXAxis_Max);
	}    

    //Set Style - Y axis
    m_mgraph_Obs->GetYaxis()->SetTitle(m_canvInfo.m_strTitle_Y.c_str() );
    m_mgraph_Obs->GetYaxis()->SetNdivisions(m_canvInfo.m_iYAxis_NDiv);
    if( m_canvInfo.m_fYAxis_Title_Offset > 0 ){
        m_mgraph_Obs->GetYaxis()->SetTitleOffset(m_canvInfo.m_fYAxis_Title_Offset);
    }
    if( m_canvInfo.m_bYAxis_UserRange ){
	    m_mgraph_Obs->GetYaxis()->SetRangeUser(m_canvInfo.m_fYAxis_Min, m_canvInfo.m_fYAxis_Max);
	}    

    //Draw (for realz)
    m_canv->cd();
    m_mgraph_Obs->Draw( m_canvInfo.m_strOptionDraw.c_str() );
    
    return;
} //End PlotterGraph::drawPlots()

//Intializes the plots defined in m_canvInfo
void PlotterGraph::initPlot(InfoPlot & plotInfo){
    /*if( plotInfo.m_vec_DataPts.size() > 0 ){ //Case: Data Input
        //Intialize
        shared_ptr<TGraph> graphPtr(new TGraph(plotInfo.m_vec_DataPts.size() ) );
        
        //Set the TName
        graphPtr->SetName( plotInfo.m_strName.c_str() );
        graphPtr->SetTitle( "" );
        
        //Fill the data
        int iPos;
        for (auto iterDataPt = plotInfo.m_vec_DataPts.begin(); iterDataPt != plotInfo.m_vec_DataPts.end(); ++iterDataPt) { //Loop Over Data
            iPos = std::distance(plotInfo.m_vec_DataPts.begin(), iterDataPt);
            
            graphPtr->SetPoint(iPos, (*iterDataPt).m_fX,  (*iterDataPt).m_fY );
        } //End Loop Over Data
        
        //m_map_graphs[plotInfo.m_strName]=graphPtr;
        m_map_graphs.insert( pair<string, shared_ptr<TGraph> >( plotInfo.m_strName, graphPtr ) );
        (m_map_iSameNameCount[plotInfo.m_strName] == 0) ? m_map_iSameNameCount[plotInfo.m_strName] = 1 : m_map_iSameNameCount[plotInfo.m_strName]++;
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
        
        //TGraph graph
        shared_ptr<TGraph> graphPtr = make_shared<TGraph>( *((TGraph*) file_Input->Get( strTmpName.c_str() ) ) );
        
        //m_map_graphs[plotInfo.m_strName]=graphPtr;
        m_map_graphs.insert( pair<string, shared_ptr<TGraph> >( plotInfo.m_strName, graphPtr ) );
        (m_map_iSameNameCount[plotInfo.m_strName] == 0) ? m_map_iSameNameCount[plotInfo.m_strName] = 1 : m_map_iSameNameCount[plotInfo.m_strName]++;
        
        file_Input->Close();
    } //End Case: TObject Input
    else{
        cout<<"PlotterGraph::initPlots() - Input Case not understood, please cross-check input!\n";
    }*/
    
    //Get the plot
    shared_ptr<TGraph> graphPtr = getPlot(plotInfo);

    //Store this plot in class's data member m_map_graphs
    m_map_graphs.insert( pair<string, shared_ptr<TGraph> >( plotInfo.m_strName, graphPtr ) );
    (m_map_iSameNameCount[plotInfo.m_strName] == 0) ? m_map_iSameNameCount[plotInfo.m_strName] = 1 : m_map_iSameNameCount[plotInfo.m_strName]++;
    
    return;
} //End PlotterGraph::initPlots()

void PlotterGraph::performAndDrawFit(TLegend & inputLegend, InfoFit & fitInfo, InfoPlot & plotInfo){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Initialize the fit for this plot
    std::shared_ptr<TF1> func_plot = getFit(fitInfo);
    
    //Set the Style
    func_plot->SetLineColor( fitInfo.m_iColor );
    func_plot->SetLineStyle( fitInfo.m_iStyleLine );
    func_plot->SetLineWidth( fitInfo.m_fSizeLine );
    
    //Add to Legend
    if(fitInfo.m_strLegEntry.length() > 0){
	inputLegend.AddEntry(func_plot.get(), fitInfo.m_strLegEntry.c_str(), "L" );
    }

    //Perform Fit
    std::shared_ptr<TGraph> graphPtr = getPlot(plotInfo);
    cout<<"====================================================================\n";
    cout<<"Fitting: " << plotInfo.m_strName << endl;
    cout<<"Function used: " << fitInfo.m_strFit_Name << endl;
    graphPtr->Fit(func_plot.get(), fitInfo.m_strFit_Option.c_str() );
    graphPtr.reset();
    cout<<"====================================================================\n";
    
    //Draw Fit
    m_canv->cd();
    func_plot->Draw("same");
    
    //Store (keeps pointers alive)
    m_map_fits[fitInfo.m_strFit_Name]=func_plot;
    
	return;
} //End PlotterGraph::performAndDrawFit()

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
    for (auto iterFit = m_map_fits.begin(); iterFit != m_map_fits.end(); ++iterFit){
        (*iterFit).second->Write();
    }
    
    //Close the file
    file_Output->Close();
    
    return;
} //End PlotterGraph::write2RootFile

//Intializes a plot defined in an input InfoPlot (aka plotInfo)
std::shared_ptr<TGraph> PlotterGraph::getPlot(InfoPlot & plotInfo){
    shared_ptr<TGraph> graphPtr;
    
    if( plotInfo.m_vec_DataPts.size() > 0 ){ //Case: Data Input
        //Intialize
        graphPtr = make_shared<TGraph>( TGraph(plotInfo.m_vec_DataPts.size() ) );
        
        //Set the TName
        graphPtr->SetName( plotInfo.m_strName.c_str() );
        graphPtr->SetTitle( "" );
        
        //Fill the data
        int iPos;
        for (auto iterDataPt = plotInfo.m_vec_DataPts.begin(); iterDataPt != plotInfo.m_vec_DataPts.end(); ++iterDataPt) { //Loop Over Data
            iPos = std::distance(plotInfo.m_vec_DataPts.begin(), iterDataPt);
            
            graphPtr->SetPoint(iPos, (*iterDataPt).m_fX,  (*iterDataPt).m_fY );
        } //End Loop Over Data
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
        
        //TGraphErrors graph
        graphPtr = make_shared<TGraph>( *((TGraph*) file_Input->Get( strTmpName.c_str() ) ) );
        
        file_Input->Close();
    } //End Case: TObject Input
    else{
        cout<<"PlotterGraph::getPlot() - Input Case not understood, please cross-check input!\n";
    }
    
    return graphPtr;
} //End PlotterGraph::getPlot()
