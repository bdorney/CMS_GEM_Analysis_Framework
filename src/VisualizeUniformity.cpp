//
//  VisualizeUniformity.cpp
//  
//
//  Created by Brian L Dorney on 18/02/16.
//
//

#include "DetectorMPGD.h"
#include "VisualizeUniformity.h"

using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;
using std::tuple;
using std::vector;

using Timing::getString;
using Timing::printROOTFileStatus;

using namespace Uniformity;

//Default Constructor
VisualizeUniformity::VisualizeUniformity(){
    bSaveCanvases = false;
    //strCanvIdent = strCanvIdentNoSpec = "Ana";
} //End Default Constructor

//Constructor with Setup & Detector inputs
VisualizeUniformity::VisualizeUniformity(Uniformity::AnalysisSetupUniformity inputSetup, Uniformity::DetectorMPGD inputDet){
    aSetup          = inputSetup;
    bSaveCanvases   = false;
    detMPGD         = inputDet;
    //strCanvIdent = strCanvIdentNoSpec = "Ana";
} //End Constructor with Setup & Detector inputs

void VisualizeUniformity::storeCanvasData(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the method below
    storeCanvasData(ptr_fileOutput, strObsName, strDrawOption);
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeCanvasData()

//Draws the distribution of an observable onto a single pad of canvas
//Takes a TFile * which the histograms are written to as input
void VisualizeUniformity::storeCanvasData(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    int iNumEta = detMPGD.getNumEtaSectors();
    
    SummaryStatistics statObs;
    //shared_ptr<TGraphErrors> gObs; //Observable to be drawn
    //std::vector<float> vec_fObs;
    //std::vector<float> vec_fObsVariance;
    
    //SectorEta etaSector;
    
    //TLegend *legObs = new TLegend(0.2,0.2,0.6,0.4);
    
    //Make the Canvas
    //------------------------------------------------------
    TCanvas canv_DetSum( ("canv_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "Dataset_AllEta" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 600, 600);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Get/Make the Summary Directory
    //------------------------------------------------------
    //Check to see if the directory exists already
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );
    
    //If the above pointer is null the directory does NOT exist, create it
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Setup the Legend
    //------------------------------------------------------
    //legObs->SetNColumns(2);
    //legObs->SetFillColor(kWhite);
    //legObs->SetLineColor(kBlack);
    
    //Loop Over the detector's Eta Sectors
    //------------------------------------------------------
    /*for (int iEta=1; iEta <= iNumEta; ++iEta) {
        //cout<<"=====NEXT ITERATION=====\n";

        int iOrigSize = vec_fObs.size();

        etaSector = detMPGD.getEtaSector(iEta);
        
        std::vector<float> vec_fObsThisEta = getObsData(strObsName, etaSector); //Temporary container for valid r-value
        
        vec_fObs.resize( vec_fObs.size() + vec_fObsThisEta.size() );

        //cout<<"BEFORE COPY\n";
        //cout<<"vec_fObs.size() = " << vec_fObs.size() << endl;
        //cout<<"vec_fObsThisEta.size() = " << vec_fObsThisEta.size() << endl;

        auto iterObs = std::next(vec_fObs.begin(), iOrigSize );
        std::copy(vec_fObsThisEta.begin(),vec_fObsThisEta.end(), iterObs );
        
        //cout<<"AFTER COPY\n";
        //cout<<"VisualizeUniformity::storeCanvasData() - vec_fObsThisEta.size() = " << vec_fObsThisEta.size() << endl;
        //cout<<"VisualizeUniformity::storeCanvasData() - vec_fObs.size() = " << vec_fObs.size() << endl;
    }*/ //End Loop Over Detector's Eta Sector
    
    //Determine the average of the dataset
    //------------------------------------------------------
    //float fAvg = std::accumulate(vec_fObs.begin(), vec_fObs.end(), 0. ) / vec_fObs.size();

    //vec_fObsVariance.resize( vec_fObs.size() );
    //std::transform(vec_fObs.begin(), vec_fObs.end(), vec_fObsVariance.begin(), [fAvg](float x) { return x - fAvg; });

    //float fStdDev = std::sqrt( std::inner_product(vec_fObsVariance.begin(), vec_fObsVariance.end(), vec_fObsVariance.begin(), 0.0) /  vec_fObsVariance.size() );

    //Create Distribution
    //------------------------------------------------------
    /*std::shared_ptr<TH1F> hObs = std::make_shared<TH1F>( TH1F( getNameByIndex(-1, -1, -1, "h", strObsName + "Dataset" ).c_str(), "", 40, fAvg - 5. * fStdDev, fAvg + 5. * fStdDev) );
    hObs->Sumw2();
    hObs->GetXaxis()->SetTitle( strObsName.c_str() );
    hObs->GetYaxis()->SetTitle( "N" );

    for(int i=0; i < vec_fObs.size(); ++i){
        hObs->Fill(vec_fObs[i]);
    }*/

    //Get the requested dataset
    //------------------------------------------------------
    statObs = getObsData(strObsName);
    
    //Fit hObs
    //------------------------------------------------------
    /*std::shared_ptr<TF1> funcObs = std::make_shared<TF1>( TF1( getNameByIndex(-1, -1, -1, "func", strObsName + "Dataset" ).c_str(), "gaus(0)", fAvg - 5. * fStdDev, fAvg + 5. * fStdDev) );
    funcObs->SetParameter(0, hObs->GetBinContent( hObs->GetMaximumBin() ) );
    funcObs->SetParameter(1, hObs->GetMean() );
    funcObs->SetParameter(2, hObs->GetRMS() );

    hObs->Fit(funcObs.get(),"QM","", fAvg - 5. * fStdDev, fAvg + 5. * fStdDev );*/

    //Draw hObs
    //------------------------------------------------------
    canv_DetSum.cd();
    //hObs->Draw( strDrawOption.c_str() );
    statObs.hDist->Draw( strDrawOption.c_str() );
    //funcObs->Draw("same");
    statObs.fitDist->Draw("same");

    //Setup the TLatex for "CMS Preliminary"
    //------------------------------------------------------
    TLatex latex_CMSPrelim, latex_Obs_Mean, latex_Obs_StdDev, latex_Obs_PerErr;
    latex_CMSPrelim.SetTextSize(0.05);
    latex_CMSPrelim.DrawLatexNDC(0.1, 0.905, "CMS Preliminary" );
    
    latex_Obs_Mean.SetTextSize(0.03);
    latex_Obs_Mean.DrawLatexNDC(0.15, 0.8, ("#mu = " + getString( statObs.fitDist->GetParameter(1) ) ).c_str() );
    
    latex_Obs_StdDev.SetTextSize(0.03);
    latex_Obs_StdDev.DrawLatexNDC(0.15, 0.75, ("#sigma = " + getString( statObs.fitDist->GetParameter(2) ) ).c_str() );
    
    latex_Obs_PerErr.SetTextSize(0.03);
    latex_Obs_PerErr.DrawLatexNDC(0.15, 0.7, ("#frac{#sigma}{#mu} = " + getString( statObs.fitDist->GetParameter(2) / statObs.fitDist->GetParameter(1) ) ).c_str() );
    
    //Draw the Legend
    //------------------------------------------------------
    //legObs->Draw("same");
    
    //Write the Canvas to the File
    //------------------------------------------------------
    dir_Summary->cd();
    canv_DetSum.Write();
    if (bSaveCanvases) { save2png(canv_DetSum); }
    statObs.hDist->Write();
    statObs.fitDist->Write();
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End VisualizeUniformity::storeCanvasData()

//Draws a given observable onto a single pad of canvas
//Takes a std::string which stores the physical filename as input
void VisualizeUniformity::storeCanvasGraph(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the method below
    storeCanvasGraph(ptr_fileOutput, strObsName, strDrawOption, bShowPhiSegmentation);
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeCanvasGraph()

//Draws a given observable onto a single pad of canvas
//Takes a TFile * which the histograms are written to as input
void VisualizeUniformity::storeCanvasGraph(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    int iNumEta = detMPGD.getNumEtaSectors();
    
    shared_ptr<TGraphErrors> gObs; //Observable to be drawn
    
    SectorEta etaSector;
    
    TLegend *legObs = new TLegend(0.2,0.2,0.6,0.4);

    TMultiGraph *mgraph_Obs = new TMultiGraph( ( "mgraph_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "_AllEta" ).c_str(), "");
	//TMultiGraph *mgraph_Obs = new TMultiGraph( ( "mgraph_" + strCanvIdentNoSpec + "_" + strObsName + "_AllEta" ).c_str(), "");
    
    vector<shared_ptr<TGraphErrors> > vec_gObs;

    //Make the Canvas
    //------------------------------------------------------
    TCanvas canv_DetSum( ("canv_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "_AllEta" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 600, 600);
    //TCanvas canv_DetSum( ("canv_" + strCanvIdentNoSpec + "_" + strObsName + "_AllEta" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 600, 600);

    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Get/Make the Summary Directory
    //------------------------------------------------------
    //Check to see if the directory exists already
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );
    
    //If the above pointer is null the directory does NOT exist, create it
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Setup the Legend
    //------------------------------------------------------
    legObs->SetNColumns(2);
    legObs->SetFillColor(kWhite);
    legObs->SetLineColor(kBlack);
    
    //Loop Over the detector's Eta Sectors
    //------------------------------------------------------
    for (int iEta=1; iEta <= iNumEta; ++iEta) {
        //Get the histogram & draw it
        etaSector = detMPGD.getEtaSector(iEta);
        gObs = getObsGraph(strObsName, etaSector);
        
        //Debugging
        //cout<<"gObs = " << gObs << endl;
        
        gObs->SetLineColor( Timing::getCyclicColor(iEta) );
        gObs->SetMarkerColor( Timing::getCyclicColor(iEta) );
        
        legObs->AddEntry(gObs.get(), ( "i#eta = " + getString(iEta) ).c_str(), "LPE");
        
        vec_gObs.push_back(gObs);			//Need to keep this pointer alive outside of Loop?
        
        mgraph_Obs->Add( gObs.get() );
    } //End Loop Over Detector's Eta Sector
    
    //Draw mgraph_Obs
    //------------------------------------------------------
    canv_DetSum.cd();
    mgraph_Obs->Draw( strDrawOption.c_str() );
    
    //Setup the TLatex for "CMS Preliminary"
    //------------------------------------------------------
    TLatex latex_CMSPrelim;
    latex_CMSPrelim.SetTextSize(0.05);
    latex_CMSPrelim.DrawLatexNDC(0.1, 0.905, "CMS Preliminary" );
    
    //Setup the iPhi designation
    //------------------------------------------------------
    //etaSector should be set here based on the last iteration of the above loop
    if(bShowPhiSegmentation){ //Case: Show iPhi Segmentation
        for(auto iterPhi = etaSector.map_sectorsPhi.begin(); iterPhi != etaSector.map_sectorsPhi.end(); ++iterPhi){
            //Ensure the canvas is the active canvas (it should be already but who knows...)
            canv_DetSum.cd();
            
            //Declare the TLatex
            TLatex latex_PhiSector;
            
            //Determine the iPhi index
            int iPhiPos = std::distance( etaSector.map_sectorsPhi.begin(), iterPhi);

            //Draw the TLatex
            latex_PhiSector.SetTextSize(0.05);
            latex_PhiSector.DrawLatexNDC(0.125 + 0.875 * ( (iPhiPos) / (float)etaSector.map_sectorsPhi.size() ), 0.8, ( "i#phi = " + getString(3 - iPhiPos) ).c_str() );
            
            //Segment the Plot with lines
            if (iPhiPos < (etaSector.map_sectorsPhi.size() - 1) ) { //Case: Not the Last Phi Segment Yet
                TLine line_PhiSeg;
                
                line_PhiSeg.SetLineStyle(2);
                line_PhiSeg.SetLineWidth(2);
                
                line_PhiSeg.DrawLineNDC( ( (iPhiPos+1) / (float)etaSector.map_sectorsPhi.size() ), 0., ( (iPhiPos+1) / (float)etaSector.map_sectorsPhi.size() ), 1. );
            } //End Case: Not the Last Phi Segment Yet
        } //End Loop Over Sector Phi
    } //End Case: Show iPhi Segmentation
    
    //Draw the Legend
    //------------------------------------------------------
    legObs->Draw("same");
    
    //Write the Canvas to the File
    //------------------------------------------------------
    dir_Summary->cd();
    canv_DetSum.Write();
    if (bSaveCanvases) { save2png(canv_DetSum); }
    mgraph_Obs->Write();
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End VisualizeUniformity::storeCanvasGraph()

//Makes a 2D plot of a given observable in the detector's active area
//Takes a std::string which stores the physical filename as input
void VisualizeUniformity::storeCanvasGraph2D(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bNormalize){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph2D","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph2D", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph2D", "\tExiting; Nothing has been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the method below
    storeCanvasGraph2D(ptr_fileOutput, strObsName, strDrawOption, bNormalize);
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeCanvasGraph2D()

//Makes a 2D plot of a given observable in the detector's active area
//Takes a TFile *, which the canvas is writtent to, as input
void VisualizeUniformity::storeCanvasGraph2D(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bNormalize){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    double dAvg = 0.;
    
    int iNumEta = detMPGD.getNumEtaSectors();
    
    shared_ptr<TGraphErrors> gObs; //Observable to be drawn
    
    SectorEta etaSector;
    
    //std::vector<shared_ptr<TGraphErrors> > vec_gObs;
	vector<tuple<double,double,double> > vec_tup3DPt;

    TGraph2D *g2DObs = new TGraph2D();   //Two dimmensional graph
    
    //Make the Canvas
    //------------------------------------------------------
    TCanvas canv_DetSum( ("canv_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "2D_AllEta" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 600, 600);

    if (bNormalize) {
        canv_DetSum.SetName( ("canv_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "Normalized2D_AllEta" ).c_str() );
    }
    
    //Set the name of the g2DObs
    //------------------------------------------------------
    if (bNormalize) {
        g2DObs->SetName( ("g2D_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "Normalized_AllEta").c_str() );
    }
    else{
        g2DObs->SetName( ("g2D_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "_AllEta").c_str() );
    }
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph2D","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph2D", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasGraph2D", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly

    //Get/Make the Summary Directory
    //------------------------------------------------------
    //Check to see if the directory exists already
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );
    
    //If the above pointer is null the directory does NOT exist, create it
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Loop Over the detector's Eta Sectors to fill vec_tup3DPt
    //------------------------------------------------------
    for (int iEta=1; iEta <= iNumEta; ++iEta) {
        //Get the histogram & draw it
        etaSector = detMPGD.getEtaSector(iEta);
        gObs = getObsGraph(strObsName, etaSector);
        
        //Fill the points of the 2D graph
        //cout<<"iEta\ti\tL.B.\tU.B.\tPx\tPy\tPz\n";
        double dPx=0, dPy=etaSector.fPos_Y, dObs=0;
        for (int i = ( (iEta-1) * gObs->GetN() ); i < (iEta * gObs->GetN() ); ++i) { //Loop Over Points of gObs
            gObs->GetPoint( (iEta * gObs->GetN() ) - i,dPx, dObs);
            
            //Debugging
            //cout<<iEta<<"\t"<<i<<"\t"<<( (iEta-1) * gObs->GetN() )<<"\t"<<(iEta * gObs->GetN() )<<"\t"<<dPx<<"\t"<<dPy<<"\t"<<dObs<<endl;

            //if(fabs(dObs) > 0) g2DObs->SetPoint(i,dPx, dPy, dObs);
            //if( !(dPx == dObs) ) g2DObs->SetPoint(i,dPx, dPy, dObs);
	    
            //Need to prevent (0,Y,0)'s from being drawn (causes coincidence points in drawing, bad)
            //First we store all non (0,Y,0) points then in another loop we fill g2DObs
            if( !(dPx == dObs ) ) vec_tup3DPt.push_back( std::make_tuple(dPx, dPy, dObs) );
        } //End Loop Over Points of gObs
    } //End Loop Over Detector's Eta Sector
    
    //Normalize elements in vec_tup3DPt to the average of the elements of vec_tup3DPt
    //------------------------------------------------------
    if ( bNormalize ) { //Case: User wants normalization
        //Determine average of strObsName
        //std::tuple<double, double, double> tup_Init = std::make_tuple(0.,0.,0.);
        //tup_Init = std::accumulate(vec_tup3DPt.begin(), vec_tup3DPt.end(), tup_Init, Uniformity::addTuple);
        //dAvg = std::get<2>(tup_Init) / vec_tup3DPt.size();
        
        dAvg = getObsData(strObsName).fMean;
        
        //Normalize elements of vec_tup3DPt to average
        std::transform(vec_tup3DPt.begin(), vec_tup3DPt.end(), vec_tup3DPt.begin(), Uniformity::divides(dAvg) );
    } //End Case: User wants normalization
    
	//Loop Over vec_tup3DPt and set members to g2DObs
    //------------------------------------------------------
    for( int i=0; i < vec_tup3DPt.size(); ++i){ //Loop over vec_tup3DPt
        g2DObs->SetPoint(i,std::get<0>(vec_tup3DPt[i]), std::get<1>(vec_tup3DPt[i]), std::get<2>(vec_tup3DPt[i]));
    } //End Loop over vec_tup3DPt

    //Make the color Palette
    //------------------------------------------------------
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
    
    //Draw the g2DObs
    //------------------------------------------------------
    canv_DetSum.cd();
	g2DObs->SetTitle("");    
	g2DObs->GetXaxis()->SetTitle( gObs->GetXaxis()->GetTitle() );
    g2DObs->GetYaxis()->SetTitle( "Position Y #left(mm#right)" );
    g2DObs->GetZaxis()->SetTitle( gObs->GetYaxis()->GetTitle() );
    g2DObs->UseCurrentStyle();
	//g2DObs->SetNpx(200);
	//g2DObs->SetNpy(200);
    g2DObs->Draw( strDrawOption.c_str() );
    //g2DObs->Draw( "TRI1" );
    
    //Setup the TLatex for "CMS Preliminary"
    //------------------------------------------------------
    TLatex latex_CMSPrelim;
    latex_CMSPrelim.SetTextSize(0.05);
    latex_CMSPrelim.DrawLatexNDC(0.1, 0.905, "CMS Preliminary" );
    	
	/*cout<<"Points Stored In 2D Graph\n";
	cout<<"====================================================\n";
	cout<<"i\tPx\tPy\tPz\n";
	Double_t *Px = g2DObs->GetX(), *Py = g2DObs->GetY(), *Pz = g2DObs->GetZ();
	for(int i=0; i<g2DObs->GetN(); ++i){
		//double Px, Py, Pz;

		//g2DObs->GetPoint(i,Px,Py,Pz);

		cout<<i<<"\t"<<Px[i]<<"\t"<<Py[i]<<"\t"<<Pz[i]<<endl;
	}*/

    //Write the Canvas to the File
    //------------------------------------------------------
    dir_Summary->cd();
    canv_DetSum.Write();
    if (bSaveCanvases) { save2png(canv_DetSum); }
    g2DObs->Write();
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End VisualizeUniformity::storeCanvasGraph2D

//Draws a given observable onto a single pad of canvas
//Takes a std::string which stores the physical filename as input
void VisualizeUniformity::storeCanvasHisto(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);

    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the method below
    //------------------------------------------------------
    storeCanvasHisto(ptr_fileOutput, strObsName, strDrawOption, bShowPhiSegmentation);
     
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeCanvasHisto()

//Draws a given observable onto a single pad of canvas
//Takes a TFile * which the histograms are written to as input
void VisualizeUniformity::storeCanvasHisto(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    int iNumEta = detMPGD.getNumEtaSectors();
    
    shared_ptr<TH1F> hObs; //Observable to be drawn
    
    SectorEta etaSector;
    
    std::vector<shared_ptr<TH1F> > vec_hObs;
    
    TLegend *legObs = new TLegend(0.2,0.2,0.6,0.4);
    
    //Make the Canvas
    //------------------------------------------------------
	//cout<<"VisualizeUniformity::storeCanvasHisto() - detMPGD.getNameNoSpecial() = " << detMPGD.getNameNoSpecial() << endl;
    TCanvas canv_DetSum( ("canv_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "_AllEta" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 600, 600);
    //TCanvas canv_DetSum( ("canv_" + strCanvIdentNoSpec + "_" + strObsName + "_AllEta" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 600, 600);

    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Get/Make the Summary Directory
    //------------------------------------------------------
    //Check to see if the directory exists already
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );
    
    //If the above pointer is null the directory does NOT exist, create it
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Setup the Legend
    //------------------------------------------------------
    legObs->SetNColumns(2);
    legObs->SetFillColor(kWhite);
    legObs->SetLineColor(kBlack);
    
    //Loop Over the detector's Eta Sectors
    //------------------------------------------------------
    for (int iEta=1; iEta <= iNumEta; ++iEta) {
        //Get the histogram & draw it
        etaSector = detMPGD.getEtaSector(iEta);
        hObs = getObsHisto(strObsName, etaSector);
        
        hObs->SetLineColor( Timing::getCyclicColor(iEta) );
        hObs->SetMarkerColor( Timing::getCyclicColor(iEta) );
        legObs->AddEntry(hObs.get(), ( "i#eta = " + getString(iEta) ).c_str(), "LPE");
        
        vec_hObs.push_back(hObs);			//Need to keep this pointer alive outside of Loop?
        
        canv_DetSum.cd();
        if( 1 == iEta ){
            vec_hObs[iEta-1]->Draw( strDrawOption.c_str() );
        }
        else{
            vec_hObs[iEta-1]->Draw( (strDrawOption + "same").c_str() );
        }
    } //End Loop Over Detector's Eta Sector
    
    //Setup the TLatex for "CMS Preliminary"
    //------------------------------------------------------
    TLatex latex_CMSPrelim;
    latex_CMSPrelim.SetTextSize(0.05);
    latex_CMSPrelim.DrawLatexNDC(0.1, 0.905, "CMS Preliminary" );
    
    //Setup the iPhi designation
    //------------------------------------------------------
    //etaSector should be set here based on the last iteration of the above loop
    if(bShowPhiSegmentation){ //Case: Show iPhi Segmentation
        for(auto iterPhi = etaSector.map_sectorsPhi.begin(); iterPhi != etaSector.map_sectorsPhi.end(); ++iterPhi){
            //Ensure the canvas is the active canvas (it should be already but who knows...)
            canv_DetSum.cd();
            
            //Declare the TLatex
            TLatex latex_PhiSector;
            
            //Determine the iPhi index
            int iPhiPos = std::distance( etaSector.map_sectorsPhi.begin(), iterPhi);

            //Draw the TLatex
            latex_PhiSector.SetTextSize(0.05);
            latex_PhiSector.DrawLatexNDC(0.125 + 0.875 * ( (iPhiPos) / (float)etaSector.map_sectorsPhi.size() ), 0.8, ( "i#phi = " + getString(3 - iPhiPos) ).c_str() );
            
            //Segment the Plot with lines
            if (iPhiPos < (etaSector.map_sectorsPhi.size() - 1) ) { //Case: Not the Last Phi Segment Yet
                TLine line_PhiSeg;
                
                line_PhiSeg.SetLineStyle(2);
                line_PhiSeg.SetLineWidth(2);
                
                line_PhiSeg.DrawLineNDC( ( (iPhiPos+1) / (float)etaSector.map_sectorsPhi.size() ), 0., ( (iPhiPos+1) / (float)etaSector.map_sectorsPhi.size() ), 1. );
            } //End Case: Not the Last Phi Segment Yet
        } //End Loop Over Sector Phi
    } //End Case: Show iPhi Segmentation
    
    //Draw the Legend
    //------------------------------------------------------
    legObs->Draw("same");
    
    //Write the Canvas to the File
    //------------------------------------------------------
    dir_Summary->cd();
    canv_DetSum.Write();
    if (bSaveCanvases) { save2png(canv_DetSum); }
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End VisualizeUniformity::storeCanvasHisto()

//Makes a 2D plot of a given observable in the detector's active area
//Takes a std::string which stores the physical filename as input
void VisualizeUniformity::storeCanvasHisto2D(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto2D","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto2D", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto2D", "\tExiting; Nothing has been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the method below
    storeCanvasHisto2D(ptr_fileOutput, strObsName, strDrawOption);
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeCanvasHisto2D()

//Makes a 2D plot of a given observable in the detector's active area
//Takes a TFile *, which the canvas is writtent to, as input
void VisualizeUniformity::storeCanvasHisto2D(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    int iNumEta = detMPGD.getNumEtaSectors();
    
    shared_ptr<TH1F> hObs; //Observable to be drawn
    
    SectorEta etaSector;
    
    vector<tuple<float,float,float> > vec_tup3DPt;
    
    TGraph2D *g2DObs = new TGraph2D();   //Two dimmensional graph
    
    //Make the Canvas
    //------------------------------------------------------
    TCanvas canv_DetSum( ("canv_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "2D_AllEta" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 600, 600);
    //TCanvas canv_DetSum( ("canv_" + strCanvIdentNoSpec + "_" + strObsName + "2D_AllEta" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 600, 600);

    //Set the name of the g2DObs
    //------------------------------------------------------
    g2DObs->SetName( ("g2D_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "_AllEta").c_str() );
    //g2DObs->SetName( ("g2D_" + strCanvIdentNoSpec + "_" + strObsName + "_AllEta").c_str() );
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto2D","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto2D", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasHisto2D", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Get/Make the Summary Directory
    //------------------------------------------------------
    //Check to see if the directory exists already
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );
    
    //If the above pointer is null the directory does NOT exist, create it
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Loop Over the detector's Eta Sectors to fill vec_tup3DPt
    //------------------------------------------------------
    for (int iEta=1; iEta <= iNumEta; ++iEta) {
        //Get the histogram & draw it
        etaSector = detMPGD.getEtaSector(iEta);
        hObs = getObsHisto(strObsName, etaSector);
        
        //Fill the points of the 2D graph
        //cout<<"iEta\ti\tL.B.\tU.B.\tPx\tPy\tPz\n";
        float fPx=0, fPy=etaSector.fPos_Y, fObs=0;
        for (int i = ( 1 + (iEta-1) * hObs->GetNbinsX() ); i < ( 1 + iEta * hObs->GetNbinsX() ); ++i) { //Loop Over Points of hObs
            fPx = hObs->GetBinCenter( (iEta * hObs->GetNbinsX() ) - i );
            fObs = hObs->GetBinContent( (iEta * hObs->GetNbinsX() ) - i );
            
            //Debugging
            cout<<iEta<<"\t"<<i<<"\t"<<( (iEta-1) * hObs->GetNbinsX() )<<"\t"<<(iEta * hObs->GetNbinsX() )<<"\t"<<fPx<<"\t"<<fPy<<"\t"<<fObs<<endl;
            
            //Need to prevent (0,Y,0)'s from being drawn (causes coincidence points in drawing, bad)
            //First we store all non (0,Y,0) points then in another loop we fill g2DObs
            if( !(fPx == fObs ) ) vec_tup3DPt.push_back( std::make_tuple(fPx, fPy, fObs) );
        } //End Loop Over Points of gObs
    } //End Loop Over Detector's Eta Sector
    
    //Loop Over vec_tup3DPt and set members to g2DObs
    //------------------------------------------------------
    for( int i=0; i < vec_tup3DPt.size(); ++i){ //Loop over vec_tup3DPt
        g2DObs->SetPoint(i,std::get<0>(vec_tup3DPt[i]), std::get<1>(vec_tup3DPt[i]), std::get<2>(vec_tup3DPt[i]));
    } //End Loop over vec_tup3DPt
    
    //Make the color Palette
    //------------------------------------------------------
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
    
    //Draw the g2DObs
    //------------------------------------------------------
    canv_DetSum.cd();
    g2DObs->SetTitle("");
    g2DObs->GetXaxis()->SetTitle( hObs->GetXaxis()->GetTitle() );
    g2DObs->GetYaxis()->SetTitle( "Position Y #left(mm#right)" );
    g2DObs->GetZaxis()->SetTitle( hObs->GetYaxis()->GetTitle() );
    g2DObs->UseCurrentStyle();
    //g2DObs->SetNpx(200);
    //g2DObs->SetNpy(200);
    g2DObs->Draw( strDrawOption.c_str() );
    canv_DetSum.SetTheta(90);
    canv_DetSum.SetPhi(0.05);
    
    //Setup the TLatex for "CMS Preliminary"
    //------------------------------------------------------
    TLatex latex_CMSPrelim;
    latex_CMSPrelim.SetTextSize(0.05);
    latex_CMSPrelim.DrawLatexNDC(0.1, 0.905, "CMS Preliminary" );
    
    cout<<"Points Stored In 2D Graph\n";
    cout<<"====================================================\n";
    cout<<"i\tPx\tPy\tPz\n";
    Double_t *Px = g2DObs->GetX(), *Py = g2DObs->GetY(), *Pz = g2DObs->GetZ();
    for(int i=0; i<g2DObs->GetN(); ++i){
        cout<<i<<"\t"<<Px[i]<<"\t"<<Py[i]<<"\t"<<Pz[i]<<endl;
    }
    
    //Write the Canvas to the File
    //------------------------------------------------------
    dir_Summary->cd();
    canv_DetSum.Write();
    if (bSaveCanvases) { save2png(canv_DetSum); }
    g2DObs->Write();
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End VisualizeUniformity::storeCanvasHisto2D()

//This method is longer than I'd like it to be
//But it seems that TCanvas doesn't perpetuate its drawn TObject's
//So passing it to another method by reference keeps the TCanvas alive, but ends up being blank with nothing drawn on it =/
//Draws the distribution pointed to by inputObjPtr on a pad of inputCanvas
//inputCanvas is split into two columns;
//The Pad is created when this method is called; iEta and iNumEta define the pad position automatically
//Odd (even) values of iEta are on the left (right)
//The SectorEta is used to determine the location of the SectorPhi's
//Takes a std::string which stores the physical filename as input
void VisualizeUniformity::storeCanvasHistoSegmented(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);

    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasHistoSegmented","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeUniformity","storeCanvasHistoSegmented", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasHistoSegmented", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the method below
    //------------------------------------------------------
    storeCanvasHistoSegmented(ptr_fileOutput, strObsName, strDrawOption, bShowPhiSegmentation);
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeCanvasHistoSegmented()

//This method is longer than I'd like it to be
//But it seems that TCanvas doesn't perpetuate its drawn TObject's
//So passing it to another method by reference keeps the TCanvas alive, but ends up being blank with nothing drawn on it =/
//Draws the distribution pointed to by inputObjPtr on a pad of inputCanvas
//inputCanvas is split into two columns;
//The Pad is created when this method is called; iEta and iNumEta define the pad position automatically
//Odd (even) values of iEta are on the left (right)
//The SectorEta is used to determine the location of the SectorPhi's
//Takes a TFile * which the histograms are written to as input
void VisualizeUniformity::storeCanvasHistoSegmented(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation){
    //Variable Declaration
    int iNumEta = detMPGD.getNumEtaSectors();
    
    float fMaxBinVal = -1;
    
    float fXPad_Low;
    float fXPad_High;
    
    float fYPad_Low;
    float fYPad_High;
    
    shared_ptr<TH1F> hObs; //Observable to be drawn
    
    SectorEta etaSector;
    
    std::vector<shared_ptr<TH1F> > vec_hObs;
    std::vector<TPad *> vec_padSectorObs;
    
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Make the Canvas
    //------------------------------------------------------
	//cout<<"VisualizeUniformity::storeCanvasHisto() - detMPGD.getName() = " << detMPGD.getName() << endl;
    //cout<<"VisualizeUniformity::storeCanvasHisto() - detMPGD.getNameNoSpecial() = " << detMPGD.getNameNoSpecial() << endl;
    TCanvas canv_DetSum( ("canv_" + detMPGD.getNameNoSpecial() + "_" + strObsName + "_AllEta_Segmented" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 1000, 2500);
    //TCanvas canv_DetSum( ("canv_" + strCanvIdentNoSpec + "_" + strObsName + "_AllEta_Segmented" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 1000, 2500);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeCanvasHistoSegmented","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeUniformity","storeCanvasHistoSegmented", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeCanvasHistoSegmented", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Get/Make the Summary Directory
    //------------------------------------------------------
    //Check to see if the directory exists already
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );
    
    //If the above pointer is null the directory does NOT exist, create it
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Loop Over the detector's Eta sectors to determine the Y-Axis range to be used for all plots
    for (int iEta=1; iEta <= iNumEta; ++iEta) {
        //Get the histogram & draw it
        etaSector = detMPGD.getEtaSector(iEta);
        hObs = getObsHisto(strObsName, etaSector);
        vec_hObs.push_back(hObs);			//Need to keep this pointer alive outside of Loop?

        if ( hObs->GetBinContent( hObs->GetMaximumBin() ) > fMaxBinVal ) { //Case: Check for the Maximum Bin Value
            fMaxBinVal = hObs->GetBinContent( hObs->GetMaximumBin() );
        } //End Case: Check for the Maximum Bin Value
    } //End Loop Over Detector's Eta Sectors
    
    //Round fMaxBinVal to the nearest power of ten
    //------------------------------------------------------
    //fMaxBinVal = Uniformity::ceilPowerTen(fMaxBinVal, 0);
    //fMaxBinVal = Uniformity::ceilPowerTen(fMaxBinVal, 1, 0);
    fMaxBinVal = std::ceil(1.1 * (fMaxBinVal / 1e3) ) * 1e3;

    //Loop Over the detector's Eta Sectors to make the TCanvas
    //------------------------------------------------------
    for (int iEta=1; iEta <= iNumEta; ++iEta) {
        //Get the SectorEta
        etaSector = detMPGD.getEtaSector(iEta);
        
        //Determine the Pad Coordinates
        if (iEta % 2 != 0){ //Case: iEta is Odd
            fXPad_Low   = 0.02;
            fXPad_High  = 0.48;
        } //End Case: iEta is Odd
        else{ //Case: iEta is Even
            fXPad_Low   = 0.52;
            fXPad_High  = 0.98;
        } //End Case: iEta is Even
        
        //Determine the Pad Y-Coordinates (Y=0 is at the top of the pad!)
        fYPad_Low   = 1. - (1. / (0.5 * iNumEta) ) * ( std::ceil(iEta/2.) - 1);
        fYPad_High  = 1. - (1. / (0.5 * iNumEta) ) * ( std::ceil(iEta/2.) );
        
        //Debugging
        //cout<<iEta<<"\t"<<fYPad_Low<<"\t"<<fYPad_High<<endl;
        
        //Initialize the Pad
        TPad *pad_SectorObs = new TPad( ( getNameByIndex(iEta, -1, -1, "pad", "Obs" + getString(iEta) ) ).c_str() ,"",fXPad_Low,fYPad_Low,fXPad_High,fYPad_High,kWhite);
        vec_padSectorObs.push_back(pad_SectorObs);	//Need to keep this pointer alive outside of Loop?
        
        canv_DetSum.cd();
        vec_padSectorObs[iEta-1]->Draw();
        vec_padSectorObs[iEta-1]->cd();
        
        //Draw the histogram
        //hObs = getObsHisto(strObsName, etaSector);
        //vec_hObs.push_back(hObs);			//Need to keep this pointer alive outside of Loop?
        vec_hObs[iEta-1]->GetYaxis()->SetRangeUser(1e-1, fMaxBinVal);
        vec_hObs[iEta-1]->Draw( strDrawOption.c_str() );
        
        //Setup the TLatex for "CMS Preliminary"
        TLatex latex_CMSPrelim;
        latex_CMSPrelim.SetTextSize(0.05);
        if( 1 == iEta){
            latex_CMSPrelim.DrawLatexNDC(0.1, 0.905, "CMS Preliminary" );
        }
        
        //Setup the TLatex for this iEta sector
        TLatex latex_EtaSector;
        latex_EtaSector.SetTextSize(0.05);
        latex_EtaSector.DrawLatexNDC(0.125, 0.85, ( "i#eta = " + getString(iEta) ).c_str() );
        
        //Setup the iPhi designation
        if(bShowPhiSegmentation){ //Case: Show iPhi Segmentation
            for(auto iterPhi = etaSector.map_sectorsPhi.begin(); iterPhi != etaSector.map_sectorsPhi.end(); ++iterPhi){
                //Ensure the pad is the active pad (it should be already but who knows...)
                vec_padSectorObs[iEta-1]->cd();
                
                //Declare the TLatex
                TLatex latex_PhiSector;
                
                //Determine the iPhi index
                int iPhiPos = std::distance( etaSector.map_sectorsPhi.begin(), iterPhi);

                //Draw the TLatex
                latex_PhiSector.SetTextSize(0.05);
                latex_PhiSector.DrawLatexNDC(0.125 + 0.875 * ( (iPhiPos) / (float)etaSector.map_sectorsPhi.size() ), 0.8, ( "i#phi = " + getString(3 - iPhiPos) ).c_str() );
                
                //Segment the Plot with lines
                if (iPhiPos < (etaSector.map_sectorsPhi.size() - 1) ) { //Case: Not the Last Phi Segment Yet
                    TLine line_PhiSeg;
                    
                    line_PhiSeg.SetLineStyle(2);
                    line_PhiSeg.SetLineWidth(2);
                    
                    line_PhiSeg.DrawLineNDC( ( (iPhiPos+1) / (float)etaSector.map_sectorsPhi.size() ), 0., ( (iPhiPos+1) / (float)etaSector.map_sectorsPhi.size() ), 1. );
                } //End Case: Not the Last Phi Segment Yet
            } //End Loop Over Sector Phi
        } //End Case: Show iPhi Segmentation
    } //End Loop Over Detector's Eta Sector
    
    //Write the Canvas to the File
    //------------------------------------------------------
    dir_Summary->cd();
    canv_DetSum.Write();
    if (bSaveCanvases) { save2png(canv_DetSum); }
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End VisualizeUniformity::storeCanvasHistoSegmented()

//For each member of the input map storeListOfCanvasesGraph is called
//Takes a std::string which stores the physical filename as input
//map_strObsNameAndDrawOpt
//  first   -> Obs Name
//  second  -> Draw option
void VisualizeUniformity::storeListOfCanvasesGraph(std::string & strOutputROOTFileName, std::string strOption, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    storeListOfCanvasesGraph(ptr_fileOutput, map_strObsNameAndDrawOpt, bShowPhiSegmentation);
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeListOfCanvasesGraph

//For each member of the input map storeListOfCanvasesGraph is called
//Takes a TFile * which the histograms are written to as input
//map_strObsNameAndDrawOpt
//  first   -> Obs Name
//  second  -> Draw option
void VisualizeUniformity::storeListOfCanvasesGraph(TFile * file_InputRootFile, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    for (auto iterMap = map_strObsNameAndDrawOpt.begin(); iterMap != map_strObsNameAndDrawOpt.end(); ++iterMap) { //Loop over input observables
        
        storeCanvasGraph(file_InputRootFile, (*iterMap).first, (*iterMap).second, bShowPhiSegmentation);
    } //End Loop over input observables
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End VisualizeUniformity::storeListOfCanvasesGraph

//For each member of the input map storeListOfCanvasesHisto is called
//Takes a std::string which stores the physical filename as input
//map_strObsNameAndDrawOpt
//  first   -> Obs Name
//  second  -> Draw option
void VisualizeUniformity::storeListOfCanvasesHisto(std::string & strOutputROOTFileName, std::string strOption, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    storeListOfCanvasesHisto(ptr_fileOutput, map_strObsNameAndDrawOpt, bShowPhiSegmentation);
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeListOfCanvasesHisto

//For each member of the input map storeListOfCanvasesHisto is called
//Takes a TFile * which the histograms are written to as input
//map_strObsNameAndDrawOpt
//  first   -> Obs Name
//  second  -> Draw option
void VisualizeUniformity::storeListOfCanvasesHisto(TFile * file_InputRootFile, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    for (auto iterMap = map_strObsNameAndDrawOpt.begin(); iterMap != map_strObsNameAndDrawOpt.end(); ++iterMap) { //Loop over input observables
        
        storeCanvasHisto(file_InputRootFile, (*iterMap).first, (*iterMap).second, bShowPhiSegmentation);
    } //End Loop over input observables
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End VisualizeUniformity::storeListOfCanvasesHisto

//For each member of the input map storeListOfCanvasesHistoSegmented is called
//Takes a std::string which stores the physical filename as input
//map_strObsNameAndDrawOpt
//  first   -> Obs Name
//  second  -> Draw option
void VisualizeUniformity::storeListOfCanvasesHistoSegmented(std::string & strOutputROOTFileName, std::string strOption, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    storeListOfCanvasesHistoSegmented(ptr_fileOutput, map_strObsNameAndDrawOpt, bShowPhiSegmentation);
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeListOfCanvasesHistoSegmented

//For each member of the input map storeListOfCanvasesHistoSegmented is called
//Takes a TFile * which the histograms are written to as input
//map_strObsNameAndDrawOpt
//  first   -> Obs Name
//  second  -> Draw option
void VisualizeUniformity::storeListOfCanvasesHistoSegmented(TFile * file_InputRootFile, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation){
    //TFile does not manage objects
    TH1::AddDirectory(kFALSE);
    
    //Variable Declaration
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeListOfCanvasesGraph", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    for (auto iterMap = map_strObsNameAndDrawOpt.begin(); iterMap != map_strObsNameAndDrawOpt.end(); ++iterMap) { //Loop over input observables
        
        storeCanvasHistoSegmented(file_InputRootFile, (*iterMap).first, (*iterMap).second, bShowPhiSegmentation);
    } //End Loop over input observables
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End VisualizeUniformity::storeListOfCanvasesHistoSegmented

//Saves inputCanv as a *.png file
//The file is placed in the working directory
//The name of the file is the TName of the canvas
void VisualizeUniformity::save2png(TCanvas & inputCanvas){
    //Variable Declaration
    string strName = inputCanvas.GetName();
    
    //std::shared_ptr<TImage> img = std::make_shared<TImage>( &TImage::Create() );
    
    //img->FromPad( inputCanvas.cd() );
    //img->WriteImage( ( strName + ".png" ).c_str() );
    
    inputCanvas.SaveAs( ( strName + ".pdf" ).c_str(), "RECREATE" );
    inputCanvas.SaveAs( ( strName + ".png" ).c_str(), "RECREATE" );
    
    return;
} //End VisualizeUniformity::save2png()

/*std::vector<float> VisualizeUniformity::getObsData(std::string strObsName, Uniformity::SectorEta &inputEta){
    //Variable Declaration
    std::vector<float> ret_vec;
    
    std::transform(strObsName.begin(),strObsName.end(),strObsName.begin(),toupper);
    
    //=======================Fit Result Parameters=======================
    if (0 == strObsName.compare("RESPONSEFITPKPOS") ) { //Case: Fit Pk Pos
        //ret_mset = inputEta.mset_fClustADC_Fit_PkPos;
        ret_vec.resize( inputEta.mset_fClustADC_Fit_PkPos.size() );
        std::copy(inputEta.mset_fClustADC_Fit_PkPos.begin(), inputEta.mset_fClustADC_Fit_PkPos.end(), ret_vec.begin() );
    } //End Case: Fit Pk Pos
    //=======================Unrecognized Parameters=======================
    else{ //Case: Unrecognized Parameter
        cout<<"Uniformity::VisualizeUniformity::getObsHisto() - Parameter " << strObsName.c_str() << " not recognized!!!\n";
    } //End Case: Unrecognized Parameter
    
    //Debugging
    //cout<<"ret_graph = " << ret_graph << endl;
    
    return ret_vec;
}*/ //End VisualizeUniformity::getObsGraph()

SummaryStatistics VisualizeUniformity::getObsData(std::string strObsName){
    //Variable Declaration
    SummaryStatistics ret_stat;
    
    std::transform(strObsName.begin(),strObsName.end(),strObsName.begin(),toupper);
    
    //=======================Fit Result Parameters=======================
    if (0 == strObsName.compare("RESPONSEFITPKPOS") ) { //Case: Fit Pk Pos
        //ret_mset = inputEta.mset_fClustADC_Fit_PkPos;
        ret_stat = detMPGD.getStatPkPos();
    } //End Case: Fit Pk Pos
    else if (0 == strObsName.compare("RESPONSEFITPKRES") ) { //Case: Fit Pk Resolution
        ret_stat = detMPGD.getStatPkRes();
    } //End Case: Fit Pk Resolution
    //=======================Unrecognized Parameters=======================
    else{ //Case: Unrecognized Parameter
        cout<<"Uniformity::VisualizeUniformity::getObsHisto() - Parameter " << strObsName.c_str() << " not recognized!!!\n";
    } //End Case: Unrecognized Parameter
    
    //Debugging
    //cout<<"ret_graph = " << ret_graph << endl;
    
    return ret_stat;
} //End VisualizeUniformity::getObsGraph()

std::shared_ptr<TGraphErrors> VisualizeUniformity::getObsGraph(std::string strObsName, Uniformity::SectorEta &inputEta){
    //Variable Declaration
    std::shared_ptr<TGraphErrors> ret_graph;
    
    std::transform(strObsName.begin(),strObsName.end(),strObsName.begin(),toupper);
    
    //=======================Fit Result Parameters=======================
    if (0 == strObsName.compare("RESPONSEFITCHI2") ) { //Case: Fit Norm Chi2
        ret_graph = inputEta.gEta_ClustADC_Fit_NormChi2;
    } //End Case: Fit Norm Chi2
    else if (0 == strObsName.compare("RESPONSEFITPKPOS") ) { //Case: Fit Pk Pos
        ret_graph = inputEta.gEta_ClustADC_Fit_PkPos;
    } //End Case: Fit Pk Pos
    else if (0 == strObsName.compare("RESPONSEFITPKRES") ) { //Case: Fit Pk Resolution
        ret_graph = inputEta.gEta_ClustADC_Fit_PkRes;
    } //End Case: Fit Pk Resolution
    //=======================Unrecognized Parameters=======================
    else{ //Case: Unrecognized Parameter
        cout<<"Uniformity::VisualizeUniformity::getObsHisto() - Parameter " << strObsName.c_str() << " not recognized!!!\n";
    } //End Case: Unrecognized Parameter
    
	//Debugging
    //cout<<"ret_graph = " << ret_graph << endl;

    return ret_graph;
} //End VisualizeUniformity::getObsGraph()

std::shared_ptr<TH1F> VisualizeUniformity::getObsHisto(std::string strObsName, Uniformity::SectorEta &inputEta){
    //Variable Declaration
    std::shared_ptr<TH1F> ret_histo;
    
    //Debugging
    //std::cout<<"Calling VisualizeUniformity::getRootObject()\n";
    
    std::transform(strObsName.begin(),strObsName.end(),strObsName.begin(),toupper);
    
    //=======================Cluster Parameters=======================
    if (0 == strObsName.compare("CLUSTADC") ) { //Case: Cluster ADC's
        ret_histo = inputEta.clustHistos.hADC;
    } //End Case: Cluster ADC's
    else if (0 == strObsName.compare("CLUSTMULTI") ) { //Case: Cluster Multi
        ret_histo = inputEta.clustHistos.hMulti;
    } //End Case: Cluster Multi
    else if (0 == strObsName.compare("CLUSTPOS") ) { //Case: Cluster Position
        ret_histo = inputEta.clustHistos.hPos;
    } //End Case: Cluster Position
    else if (0 == strObsName.compare("CLUSTSIZE") ) { //Case: Cluster Size
        ret_histo = inputEta.clustHistos.hSize;
    } //End Case: Cluster Size
    else if (0 == strObsName.compare("CLUSTTIME") ) { //Case: Cluster Time
        ret_histo = inputEta.clustHistos.hTime;
    } //End Case: Cluster Time
    //=======================Hit Parameters=======================
    else if (0 == strObsName.compare("HITADC") ) { //Case: Hit ADC
        ret_histo = inputEta.hitHistos.hADC;
    } //End Case: Hit ADC
    else if (0 == strObsName.compare("HITPOS") ) { //Case: Hit Position
	    ret_histo = inputEta.hitHistos.hPos;
	} //End Case: Hit Position
    else if (0 == strObsName.compare("HITTIME") ) { //Case: Hit Time
        ret_histo = inputEta.hitHistos.hTime;
    } //End Case: Hit Time
    //=======================Unrecognized Parameters=======================
    else{ //Case: Unrecognized Parameter
        cout<<"Uniformity::VisualizeUniformity::getObsHisto() - Parameter " << strObsName.c_str() << " not recognized!!!\n";
    } //End Case: Unrecognized Parameter
    
    return ret_histo;
} //End VisualizeUniformity::getObsHisto()


