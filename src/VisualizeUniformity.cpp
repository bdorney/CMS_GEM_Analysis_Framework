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

//This method is longer than I'd like it to be
//But it seems that TCanvas doesn't perpetuate its drawn TObject's
//So passing it to another method by reference keeps the TCanvas alive, but ends up being blank with nothing drawn on it =/
//Draws the distribution pointed to by inputObjPtr on a pad of inputCanvas
//inputCanvas is split into two columns;
//The Pad is created when this method is called; iEta and iNumEta define the pad position automatically
//Odd (even) values of iEta are on the left (right)
//The SectorEta is used to determine the location of the SectorPhi's
void VisualizeUniformity::storeCanvas(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption){
    //Variable Declaration
    //bool bEvenEtaNum = (bool) ( iNumEta % 2);
    
    int iNumEta = detMPGD.getNumEtaSectors();
    
    float fXPad_Low;
    float fXPad_High;
    
    float fYPad_Low;
    float fYPad_High;
    
    shared_ptr<TH1F> hObs; //Observable to be drawn
    
    SectorEta etaSector;
    
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Make the Canvas
    //------------------------------------------------------
    TCanvas canv_DetSum( ("canv_" + strObsName + "_AllEta" ).c_str(), ( strObsName + " for All Eta" ).c_str(), 1000, 2500);

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
    
    //Partition the Canvas
    //------------------------------------------------------
    /*if (bEvenEtaNum) {
        inputCanvas.Divide(2, iNumEta / 2);
        cout<<"Even Number Of Pads"<<endl;
    }
    else{
        inputCanvas.Divide(2, std::ceil(iNumEta / 2.) );
        cout<<"Odd Number of Pads"<<endl;
    }*/
    
    //Loop Over the detector's Eta Sectors
    //------------------------------------------------------
    for (int iEta=1; iEta <= iNumEta; ++iEta) {
        //Determine the Pad X-Coordinates
        if (iEta % 2 != 0){ //Case: iEta is Odd
            fXPad_Low   = 0.02;
            fXPad_High  = 0.48;
        } //End Case: iEta is Odd
        else{ //Case: iEta is Even
            fXPad_Low   = 0.52;
            fXPad_High  = 0.98;
        } //End Case: iEta is Even
        
        //Determine the Pad Y-Coordinates
        fYPad_Low   = (1. / iNumEta) * (iEta - 1);
        fYPad_High  = (1. / iNumEta) * (iEta);
        
        //Initialize the Pad
        TPad *pad_SectorObs = new TPad( ( getNameByIndex(iEta, -1, -1, "pad", "Obs" ) ).c_str() ,"",fXPad_Low,fYPad_Low,fXPad_High,fYPad_High,kWhite);
        canv_DetSum.cd();
        pad_SectorObs->Draw();
        
        //Get the histogram & draw it
        etaSector = detMPGD.getEtaSector(iEta);
        hObs = getObsHisto(strObsName, etaSector);
        hObs->Draw( strDrawOption.c_str() );
        
        //Setup the TLatex for this iEta sector
        TLatex latex_EtaSector;
        latex_EtaSector.SetTextSize(0.03);
        latex_EtaSector.DrawLatexNDC(0.1, 0.9, ( "i#eta = " + getString(iEta) ).c_str() );
        
        //Setup the iPhi designation
        for(auto iterPhi = etaSector.map_sectorsPhi.begin(); iterPhi != etaSector.map_sectorsPhi.end(); ++iterPhi){
            //Ensure the pad is the active pad (it should be already but who knows...)
            pad_SectorObs->cd();
            
            //Declare the TLatex
            TLatex latex_PhiSector;
            
            //Determine the iPhi index
            int iPhiPos = std::distance( etaSector.map_sectorsPhi.begin(), iterPhi);
            
            //Draw the TLatex
            latex_PhiSector.SetTextSize(0.03);
            latex_PhiSector.DrawLatexNDC(0.1+(iPhiPos)*(iPhiPos / (float)etaSector.map_sectorsPhi.size() ), 0.8, ( "i#phi = " + getString(iPhiPos) ).c_str() );
            
            //Segment the Plot with lines
            if (iPhiPos < (etaSector.map_sectorsPhi.size() - 1) ) { //Case: Not the Last Phi Segment Yet
                TLine line_PhiSeg;
                
                line_PhiSeg.SetLineStyle(2);
                line_PhiSeg.SetLineWidth(2);
                
                line_PhiSeg.DrawLineNDC( ( (iPhiPos+1) / (float)etaSector.map_sectorsPhi.size() ), 0., ( (iPhiPos+1) / (float)etaSector.map_sectorsPhi.size() ), 1. );
            } //End Case: Not the Last Phi Segment Yet
        } //End Loop Over Sector Phi
    } //End Loop Over Detector's Eta Secto
    
    //Write the Canvas to the File
    //------------------------------------------------------
    dir_Summary->cd();
    canv_DetSum.Write();
    
    //Close the File
    //------------------------------------------------------
    ptr_fileOutput->Close();
    
    return;
} //End VisualizeUniformity::storeHistos()

std::shared_ptr<TH1F> VisualizeUniformity::getObsHisto(std::string &strObsName, Uniformity::SectorEta &inputEta){
    //Variable Declaration
    std::shared_ptr<TH1F> ret_histo;
    
    std::cout<<"Calling VisualizeUniformity::getRootObject()\n";
    
    //=======================Cluster Parameters=======================
    if (0 == strObsName.compare("CLUSTADC") ) { //Case: Cluster ADC's
        ret_histo = inputEta.clustHistos.hADC;
	//ret_histo = std::make_shared<TObject>( inputEta.clustHistos.hADC.get() );
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
	//std::cout<<"ret_histo = " << ret_histo << std::endl;
	//std::cout<<"inputEta.hitHistos.hPos = " << inputEta.hitHistos.hPos << std::endl;
	//std::cout<<"inputEta.hitHistos.hPos.get() = " << inputEta.hitHistos.hPos.get() << std::endl;
	//std::cout<<"inputEta.map_sectorsPhi.size() = " << inputEta.map_sectorsPhi.size() << std::endl;

        ret_histo = inputEta.hitHistos.hPos;
	
	//std::cout<<"ret_histo = " << ret_histo << std::endl;
    } //End Case: Hit Position
    else if (0 == strObsName.compare("HITTIME") ) { //Case: Hit Time
        ret_histo = inputEta.hitHistos.hTime;
    } //End Case: Hit Time
    //=======================Results Parameters=======================
    else if (0 == strObsName.compare("RESPONSEFITCHI2") ) { //Case: Fit Norm Chi2
	inputEta.gEta_ClustADC_Fit_NormChi2->Draw(); //Hack
        ret_histo = std::make_shared<TH1F>( *inputEta.gEta_ClustADC_Fit_NormChi2->GetHistogram() );
    } //End Case: Fit Norm Chi2
    else if (0 == strObsName.compare("RESPONSEFITPKPOS") ) { //Case: Fit Pk Pos
	inputEta.gEta_ClustADC_Fit_PkPos->Draw(); //Hack
        ret_histo = std::make_shared<TH1F>( *inputEta.gEta_ClustADC_Fit_PkPos->GetHistogram() );
    } //End Case: Fit Pk Pos
    
    return ret_histo;
} //End VisualizeUniformity::getObsHisto()

