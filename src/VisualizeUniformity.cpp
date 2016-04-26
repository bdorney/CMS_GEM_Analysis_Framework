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

using Timing::getString;
using Timing::printROOTFileStatus;

using namespace Uniformity;

//Constructor
VisualizeUniformity::VisualizeUniformity(Uniformity::AnalysisSetupUniformity inputSetup, Uniformity::DetectorMPGD inputDet){
    aSetup  = inputSetup;
    detMPGD = inputDet;
} //End Constructor

void VisualizeUniformity::storeHistos(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption){
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    std::shared_ptr<TCanvas> canv_StoredCanv;
    
    //Check if File Failed to Open Correctly
    //------------------------------------------------------
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("VisualizeUniformity","storeHistos","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("VisualizeUniformity","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("VisualizeUniformity","storeHistos", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Get/Make the Summary Directory
    //------------------------------------------------------
    //Check to see if the directory exists already
    TDirectory *dir_Summary = ptr_fileOutput->GetDirectory("Summary", false, "GetDirectory" );
    
    //If the above pointer is null the directory does NOT exist, create it
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = ptr_fileOutput->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Get the Canvas
    //------------------------------------------------------
    canv_StoredCanv = drawSectorEtaCanvas(strObsName, strDrawOption);
    
    //Write the Canvas to the File
    //------------------------------------------------------
    dir_Summary->cd();
    canv_StoredCanv->Write();
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeHistos()

std::shared_ptr<TCanvas>  VisualizeUniformity::drawSectorEtaCanvas(std::string &strObsName, std::string &strDrawOption){
    //Variable Declaration
    int iNumEta = detMPGD.getNumEtaSectors();
    
    shared_ptr<TCanvas> ret_Canvas;
    
    shared_ptr<TObject> tobjObs; //Observable to be drawn
    
    //Make the Canvas
    //------------------------------------------------------
    TCanvas canv_Obs( ("canv_" + strObsName + "_AllEta" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 1000, 2500);
    ret_Canvas = std::make_shared<TCanvas>(canv_Obs.Clone() );
    
    //Loop Over the detector's Eta Sectors
    for (int i=1; i <= iNumEta; ++i) {
        SectorEta etaSector = detMPGD.getEtaSector(i);
        
	cout<<"tobjObs = " << tobjObs << endl;

        tobjObs = getRootObject(strObsName, etaSector);
        
	cout<<"tobjObs = " << tobjObs << endl;
        
	drawSectorEtaObs(tobjObs, ret_Canvas, strDrawOption, i, iNumEta, etaSector);
    } //End Loop Over Detector's Eta Secto
    
    return ret_Canvas;
} //End VisualizeUniformity::drawSectorEtaCanvas()

//Draws the distribution pointed to by inputObjPtr on a pad of inputCanvas
//inputCanvas is split into two columns;
//The Pad is created when this method is called; iEta and iNumEta define the pad position automatically
//Odd (even) values of iEta are on the left (right)
//The SectorEta is used to determine the location of the SectorPhi's
void VisualizeUniformity::drawSectorEtaObs(shared_ptr<TObject> inputObjPtr, shared_ptr<TCanvas> inputCanvas, std::string &strDrawOption, int iEta, int iNumEta, SectorEta &inputEta){
    
    //Variable Declaration
    float fXPad_Low;
    float fXPad_High;
    
    float fYPad_Low;
    float fYPad_High;
    
    string strName = inputObjPtr->GetName();
    
    TLatex latex_EtaSector;
    
    TPad *pad_SectorObs;
    
    //Determine the Pad X-Coordinates
    //------------------------------------------------------
    if (iEta % 2 != 0){ //Case: iEta is Odd
        fXPad_Low   = 0.02;
        fXPad_High  = 0.48;
    } //End Case: iEta is Odd
    else{ //Case: iEta is Even
        fXPad_Low   = 0.52;
        fXPad_High  = 0.98;
    } //End Case: iEta is Even

    //Determine the Pad Y-Coordinates
    //------------------------------------------------------
    fYPad_Low   = (1. / iNumEta) * (iEta - 1);
    fYPad_High  = (1. / iNumEta) * (iEta);
    
    //Initialize the Pad
    //------------------------------------------------------
    pad_SectorObs = new TPad( ( getNameByIndex(iEta, -1, -1, "pad", "Obs" ) ).c_str() ,"",fXPad_Low,fYPad_Low,fXPad_High,fYPad_High,kWhite);
    
    //Draw the Object
    //------------------------------------------------------
    pad_SectorObs->cd();
    inputObjPtr->Draw( strDrawOption.c_str() );
    
    //Draw the TLatex - Eta
    //------------------------------------------------------
    latex_EtaSector.SetTextSize(0.03);
    latex_EtaSector.DrawLatexNDC(0.1, 0.9, ( "i#eta = " + getString(iEta) ).c_str() );
    
    //Draw the TLatex - Phi
    //------------------------------------------------------
    for(auto iterPhi = inputEta.map_sectorsPhi.begin(); iterPhi != inputEta.map_sectorsPhi.end(); ++iterPhi){
        //Ensure the pad is the active pad (it should be already but who knows...)
        pad_SectorObs->cd();
        
        //Declare the TLatex
        TLatex latex_PhiSector;
        
        //Determine the iPhi index
        int iPhiPos = std::distance( inputEta.map_sectorsPhi.begin(), iterPhi);
        
        //Draw the TLatex
        latex_PhiSector.SetTextSize(0.03);
        latex_PhiSector.DrawLatexNDC(0.1+(iPhiPos-1)*(iPhiPos / (float)inputEta.map_sectorsPhi.size() ), 0.8, ( "i#phi = " + getString(iPhiPos) ).c_str() );
        
        //Segment the Plot with lines
        if (iPhiPos < (inputEta.map_sectorsPhi.size() - 1) ) { //Case: Not the Last Phi Segment Yet
            TLine line_PhiSeg;
            
            line_PhiSeg.SetLineStyle(2);
            line_PhiSeg.SetLineWidth(2);
            
            line_PhiSeg.DrawLineNDC( (iPhiPos / (float)inputEta.map_sectorsPhi.size() ), 0., (iPhiPos / (float)inputEta.map_sectorsPhi.size() ), 1. );
        } //End Case: Not the Last Phi Segment Yet
    } //End Loop Over Sector Phi
    
    //Draw the Pad
    //------------------------------------------------------
    inputCanvas->cd();
    pad_SectorObs->Draw();
    
    return;
} //End VisualizeUniformity::drawSectorEtaObs()

std::shared_ptr<TObject> VisualizeUniformity::getRootObject(std::string &strObsName, Uniformity::SectorEta &inputEta){
    //Variable Declaration
    std::shared_ptr<TObject> ret_object;
    
    cout<<"Calling VisualizeUniformity::getRootObject()\n";
    
    //=======================Cluster Parameters=======================
    if (0 == strObsName.compare("CLUSTADC") ) { //Case: Cluster ADC's
        ret_object = inputEta.clustHistos.hADC;
	//ret_object = std::make_shared<TObject>( inputEta.clustHistos.hADC.get() );
    } //End Case: Cluster ADC's
    else if (0 == strObsName.compare("CLUSTMULTI") ) { //Case: Cluster Multi
        ret_object = inputEta.clustHistos.hMulti;
    } //End Case: Cluster Multi
    else if (0 == strObsName.compare("CLUSTPOS") ) { //Case: Cluster Position
        ret_object = inputEta.clustHistos.hPos;
    } //End Case: Cluster Position
    else if (0 == strObsName.compare("CLUSTSIZE") ) { //Case: Cluster Size
        ret_object = inputEta.clustHistos.hSize;
    } //End Case: Cluster Size
    else if (0 == strObsName.compare("CLUSTTIME") ) { //Case: Cluster Time
        ret_object = inputEta.clustHistos.hTime;
    } //End Case: Cluster Time
    //=======================Hit Parameters=======================
    else if (0 == strObsName.compare("HITADC") ) { //Case: Hit ADC
        ret_object = inputEta.hitHistos.hADC;
    } //End Case: Hit ADC
    else if (0 == strObsName.compare("HITPOS") ) { //Case: Hit Position
	cout<<"ret_object = " << ret_object << endl;
	cout<<"inputEta.hitHistos.hPos = " << inputEta.hitHistos.hPos << endl;
	cout<<"inputEta.hitHistos.hPos.get() = " << inputEta.hitHistos.hPos.get() << endl;
	cout<<"inputEta.map_sectorsPhi.size() = " << inputEta.map_sectorsPhi.size() << endl;

        ret_object = inputEta.hitHistos.hPos;
	cout<<"ret_object = " << ret_object << endl;
    } //End Case: Hit Position
    else if (0 == strObsName.compare("HITTIME") ) { //Case: Hit Time
        ret_object = inputEta.hitHistos.hTime;
    } //End Case: Hit Time
    //=======================Results Parameters=======================
    else if (0 == strObsName.compare("RESPONSEFITCHI2") ) { //Case: Fit Norm Chi2
        ret_object = inputEta.gEta_ClustADC_Fit_NormChi2;
    } //End Case: Fit Norm Chi2
    else if (0 == strObsName.compare("RESPONSEFITPKPOS") ) { //Case: Fit Pk Pos
        ret_object = inputEta.gEta_ClustADC_Fit_PkPos;
    } //End Case: Fit Pk Pos
    
    return ret_object;
} //End VisualizeUniformity::getRootObject()




