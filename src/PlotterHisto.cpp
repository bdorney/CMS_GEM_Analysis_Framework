//
//  PlotterHisto.cpp
//  
//
//  Created by Brian L Dorney on 05/10/16.
//
//

#include "PlotterHisto.h"

using std::cout;
using std::endl;
using std::make_shared;
using std::multimap;
using std::pair;
using std::shared_ptr;
using std::string;

using namespace QualityControl::Plotter;

//Constructor
PlotterHisto::PlotterHisto() {
    
    //Placeholder
    
} //End

void PlotterHisto::addPlot(TLegend & inputLegend, InfoPlot & plotInfo){
    //Initialize
    initPlot(plotInfo);
    
    //Check to make sure it initialized successfully
    if ( !(m_map_histos.count(plotInfo.m_strName) >= (m_map_iSameNameCount[plotInfo.m_strName]) ) ) {
        cout<<"PlotterHisto::makePlots() - Plot:\n";
        cout<<"\t"<<plotInfo.m_strName<<endl;
        cout<<"\tDid not initialize correctly, please cross-check input!\n";
        cout<<"\tExiting!!!\n";
        
        return;
    }
    
    //Get this plot
	pair<multimap<string,shared_ptr<TH1F> >::iterator, multimap<string,shared_ptr<TH1F> >::iterator> pair_iterThisPlotName = m_map_histos.equal_range(plotInfo.m_strName);
	multimap<string,shared_ptr<TH1F> >::iterator iterThisPlot = pair_iterThisPlotName.first;
	std::advance(iterThisPlot, (m_map_iSameNameCount[plotInfo.m_strName] - 1 ) );
	auto hPtr = ( (*iterThisPlot).second );
    
    //Set the Style
    hPtr->SetLineColor(plotInfo.m_iColor);
    hPtr->SetLineStyle(plotInfo.m_iStyleLine);
    hPtr->SetLineWidth(plotInfo.m_fSizeLine);
    
    hPtr->SetMarkerColor(plotInfo.m_iColor);
    hPtr->SetMarkerStyle(plotInfo.m_iStyleMarker);
    hPtr->SetMarkerSize(plotInfo.m_fSizeMarker);
    
    //Add to the Legend
    if(plotInfo.m_strLegEntry.length() > 0){
    	inputLegend.AddEntry(hPtr.get(), plotInfo.m_strLegEntry.c_str(), "LPE" );
    }

    return;
} //End PlotterHisto::addPlot()

void PlotterHisto::drawPlots(){
    for (auto iterPlot = m_map_histos.begin(); iterPlot != m_map_histos.end(); ++iterPlot) {
        //Draw (root hack to make sure axis alive)
        //m_canv->cd();
        //(*iterPlot).second->Draw( m_canvInfo.m_strOptionDraw.c_str() );
        
        //Normalize?
        if ( m_canvInfo.m_bNormalize ){
            (*iterPlot).second->Scale(1. / (*iterPlot).second->Integral() );
        }

        //Set Style - X axis
        (*iterPlot).second->GetXaxis()->SetTitle(m_canvInfo.m_strTitle_X.c_str() );
        (*iterPlot).second->GetXaxis()->SetNdivisions(m_canvInfo.m_iXAxis_NDiv);
        if( m_canvInfo.m_fXAxis_Title_Offset > 0 ){
            (*iterPlot).second->GetXaxis()->SetTitleOffset(m_canvInfo.m_fXAxis_Title_Offset);
        }
        if( m_canvInfo.m_bXAxis_UserRange ){
            (*iterPlot).second->GetXaxis()->SetRangeUser(m_canvInfo.m_fXAxis_Min, m_canvInfo.m_fXAxis_Max);
        }
        
        //Set Style - Y axis
        (*iterPlot).second->GetYaxis()->SetTitle(m_canvInfo.m_strTitle_Y.c_str() );
        (*iterPlot).second->GetYaxis()->SetNdivisions(m_canvInfo.m_iYAxis_NDiv);
        if( m_canvInfo.m_fYAxis_Title_Offset > 0 ){
            (*iterPlot).second->GetYaxis()->SetTitleOffset(m_canvInfo.m_fYAxis_Title_Offset);
        }
        if( m_canvInfo.m_bYAxis_UserRange ){
            (*iterPlot).second->GetYaxis()->SetRangeUser(m_canvInfo.m_fYAxis_Min, m_canvInfo.m_fYAxis_Max);
        }    
        
        //Add "same" to the draw option if it is not present already
        if ( std::distance(m_map_histos.begin(), iterPlot) > 0
            && ( m_canvInfo.m_strOptionDraw.find("same") == std::string::npos 
                || m_canvInfo.m_strOptionDraw.find("SAME") == std::string::npos ) ) {
            m_canvInfo.m_strOptionDraw = "same" + m_canvInfo.m_strOptionDraw;
        }

        //Draw (for realz)
        m_canv->cd();
        (*iterPlot).second->Draw( m_canvInfo.m_strOptionDraw.c_str() );
    }
    
    return;
} //End PlotterHisto::drawPlots()

//Intializes the plots defined in m_canvInfo
void PlotterHisto::initPlot(InfoPlot & plotInfo){
    /*if( plotInfo.m_vec_DataPts.size() > 0 ){ //Case: Data Input
        
        cout<<"PlotterHisto::initPlot() - Data Input for Histogram Not Supported\n";
        cout<<"PlotterHisto::initPlot() - You should be using a graph instead!\n";
        
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
        //shared_ptr<TGraphErrors> graphPtr = make_shared<TGraphErrors>( *((TGraphErrors*) file_Input->Get( plotInfo.m_strName.c_str() ) ) );
        shared_ptr<TH1F> hPtr = make_shared<TH1F>( *((TH1F*) file_Input->Get( strTmpName.c_str() ) ) );
        
        //m_map_histos[plotInfo.m_strName]=hPtr;
        m_map_histos.insert( pair<string, shared_ptr<TH1F> >( plotInfo.m_strName, hPtr ) );
        (m_map_iSameNameCount[plotInfo.m_strName] == 0) ? m_map_iSameNameCount[plotInfo.m_strName] = 1 : m_map_iSameNameCount[plotInfo.m_strName]++;
        
        file_Input->Close();
    } //End Case: TObject Input
    else{
        cout<<"PlotterHisto::initPlots() - Input Case not understood, please cross-check input!\n";
    }*/
    
    //Get the plot
    shared_ptr<TH1F> hPtr = getPlot(plotInfo);
    
    //Store this plot in class's data member m_map_histos
    m_map_histos.insert( pair<string, shared_ptr<TH1F> >( plotInfo.m_strName, hPtr ) );
    (m_map_iSameNameCount[plotInfo.m_strName] == 0) ? m_map_iSameNameCount[plotInfo.m_strName] = 1 : m_map_iSameNameCount[plotInfo.m_strName]++;
    
    return;
} //End PlotterHisto::initPlots()

void PlotterHisto::performAndDrawFit(TLegend & inputLegend, InfoFit & fitInfo, InfoPlot & plotInfo){
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
    std::shared_ptr<TH1F> hPtr = getPlot(plotInfo);
    cout<<"====================================================================\n";
    cout<<"Fitting: " << plotInfo.m_strName << endl;
    cout<<"Function used: " << fitInfo.m_strFit_Name << endl;
    hPtr->Fit(func_plot.get(), fitInfo.m_strFit_Option.c_str() );
    hPtr.reset();
    cout<<"====================================================================\n";
    
    //Draw Fit
    m_canv->cd();
    func_plot->Draw("same");
    
    //Store (keeps pointers alive)
    m_map_fits[fitInfo.m_strFit_Name]=func_plot;
    
    return;
} //End PlotterGeneric::performAndDrawFit()

void PlotterHisto::write2RootFile(){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Initialize the file
    TFile * file_Output = new TFile(m_strOutputFile_Name.c_str(), m_strOutputFile_Option.c_str() );
    
    //Write TObjects
    m_canv->Write();
    for (auto iterPlot = m_map_histos.begin(); iterPlot != m_map_histos.end(); ++iterPlot) {
        (*iterPlot).second->Write();
    }
    for (auto iterFit = m_map_fits.begin(); iterFit != m_map_fits.end(); ++iterFit){
        (*iterFit).second->Write();
    }
    
    //Close the file
    file_Output->Close();
    
    return;
} //End PlotterHisto::write2RootFile()

//Intializes a plot defined in an input InfoPlot (aka plotInfo)
std::shared_ptr<TH1F> PlotterHisto::getPlot(InfoPlot & plotInfo){
    shared_ptr<TH1F> hPtr;
    
    if( plotInfo.m_vec_DataPts.size() > 0 ){ //Case: Data Input
        
        cout<<"PlotterHisto::initPlot() - Data Input for Histogram Not Supported\n";
        cout<<"PlotterHisto::initPlot() - You should be using a graph instead!\n";
        
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
        //shared_ptr<TGraphErrors> graphPtr = make_shared<TGraphErrors>( *((TGraphErrors*) file_Input->Get( plotInfo.m_strName.c_str() ) ) );
        hPtr = make_shared<TH1F>( *((TH1F*) file_Input->Get( strTmpName.c_str() ) ) );
        
        //m_map_histos[plotInfo.m_strName]=hPtr;
        m_map_histos.insert( pair<string, shared_ptr<TH1F> >( plotInfo.m_strName, hPtr ) );
        (m_map_iSameNameCount[plotInfo.m_strName] == 0) ? m_map_iSameNameCount[plotInfo.m_strName] = 1 : m_map_iSameNameCount[plotInfo.m_strName]++;
        
        file_Input->Close();
    } //End Case: TObject Input
    else{
        cout<<"PlotterHisto::initPlots() - Input Case not understood, please cross-check input!\n";
    }
    
    return hPtr;
} //End PlotterGraphErrors::getPlot()
