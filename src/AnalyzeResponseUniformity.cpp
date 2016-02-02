//
//  AnalyzeResponseUniformity.cpp
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#include "DetectorMPGD.h"
#include "AnalyzeResponseUniformity.h"

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

using Timing::getString;
using Timing::printROOTFileStatus;
using Timing::HistoSetup;

using namespace Uniformity;

//Default Constructor
AnalyzeResponseUniformity::AnalyzeResponseUniformity(){
    analysisName = "analysis";
    
    //Setup Histo Setup Containers
    //Cluster ADC Histos
    hSetupClust_ADC.fHisto_xLower = 0;
    hSetupClust_ADC.fHisto_xUpper = 15000;
    
    hSetupClust_ADC.iHisto_nBins = 300;
    
    hSetupClust_ADC.strHisto_Name = "clustADC";
    hSetupClust_ADC.strHisto_Title_X = "ADC";
    hSetupClust_ADC.strHisto_Title_Y = "N";
    
    //Cluster Multiplicity
    hSetupClust_Multi.fHisto_xLower = 0;
    hSetupClust_Multi.fHisto_xUpper = 20;
    
    hSetupClust_Multi.iHisto_nBins = 20;
    
    hSetupClust_Multi.strHisto_Name = "clustMulti";
    hSetupClust_Multi.strHisto_Title_X = "Cluster Multiplicity";
    hSetupClust_Multi.strHisto_Title_Y = "N";
    
    //Cluster Position
    //hSetupClust_Pos.fHisto_xLower = 0;
    //hSetupClust_Pos.fHisto_xUpper = 15000;
    
    hSetupClust_Pos.iHisto_nBins = 100;
    
    hSetupClust_Pos.strHisto_Name = "clustPos";
    hSetupClust_Pos.strHisto_Title_X = "Position #left(mm#right)";
    hSetupClust_Pos.strHisto_Title_Y = "N";
    
    //Cluster Size
    hSetupClust_Size.fHisto_xLower = 0;
    hSetupClust_Size.fHisto_xUpper = 10;
    
    hSetupClust_Size.iHisto_nBins = 10;
    
    hSetupClust_Size.strHisto_Name = "clustSize";
    hSetupClust_Size.strHisto_Title_X = "Size #left(N_{strips}#right)";
    hSetupClust_Size.strHisto_Title_Y = "N";
} //End Default Constructor

//Set inputs at construction
AnalyzeResponseUniformity::AnalyzeResponseUniformity(AnalysisSetupUniformity inputSetup, DetectorMPGD inputDet){
    analysisName = "analysis";
    
    //Setup Histo Setup Containers
    //Cluster ADC Histos
    hSetupClust_ADC.fHisto_xLower = 0;
    hSetupClust_ADC.fHisto_xUpper = 15000;
    
    hSetupClust_ADC.iHisto_nBins = 300;
    
    hSetupClust_ADC.strHisto_Name = "clustADC";
    hSetupClust_ADC.strHisto_Title_X = "ADC";
    hSetupClust_ADC.strHisto_Title_Y = "N";
    
    //Cluster Multiplicity
    hSetupClust_Multi.fHisto_xLower = 0;
    hSetupClust_Multi.fHisto_xUpper = 20;
    
    hSetupClust_Multi.iHisto_nBins = 20;
    
    hSetupClust_Multi.strHisto_Name = "clustMulti";
    hSetupClust_Multi.strHisto_Title_X = "Cluster Multiplicity";
    hSetupClust_Multi.strHisto_Title_Y = "N";
    
    //Cluster Position
    //hSetupClust_Pos.fHisto_xLower = 0;
    //hSetupClust_Pos.fHisto_xUpper = 15000;
    
    hSetupClust_Pos.iHisto_nBins = 100;
    
    hSetupClust_Pos.strHisto_Name = "clustPos";
    hSetupClust_Pos.strHisto_Title_X = "Position #left(mm#right)";
    hSetupClust_Pos.strHisto_Title_Y = "N";
    
    //Cluster Size
    hSetupClust_Size.fHisto_xLower = 0;
    hSetupClust_Size.fHisto_xUpper = 10;
    
    hSetupClust_Size.iHisto_nBins = 10;
    
    hSetupClust_Size.strHisto_Name = "clustSize";
    hSetupClust_Size.strHisto_Title_X = "Size #left(N_{strips}#right)";
    hSetupClust_Size.strHisto_Title_Y = "N";
    
    //Store Analysis Parameters
    aSetup = inputSetup;
    
    //Store Detector
    detMPGD = inputDet;
} //End Constructor

//Loops over all stored clusters in detMPGD and Book histograms for the full detector
void AnalyzeResponseUniformity::fillHistos(){
    //Variable Declaration
    //vector<Cluster> vec_clust;
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Initialize iEta Histograms - 1D
        (*iterEta).second.hEta_ClustADC = std::make_shared<TH1F>(getHistogram((*iterEta).first, -1, hSetupClust_ADC ) );
        (*iterEta).second.hEta_ClustMulti = std::make_shared<TH1F>(getHistogram((*iterEta).first, -1, hSetupClust_Multi ) );
        (*iterEta).second.hEta_ClustPos = std::make_shared<TH1F>(getHistogram((*iterEta).first, -1, hSetupClust_Pos ) );
        (*iterEta).second.hEta_ClustSize = std::make_shared<TH1F>(getHistogram((*iterEta).first, -1, hSetupClust_Size ) );
        
        //Initialize iEta Histograms - 2D
        (*iterEta).second.hEta_ClustADC_v_ClustPos = std::make_shared<TH2F>( TH2F( ("hiEta" + getString( (*iterEta).first ) + "_ClustADC_v_ClustPos").c_str(),"Response Uniformity",200,-0.5*(*iterEta).second.fWidth,0.5*(*iterEta).second.fWidth,300,0,15000) );
        
        //Debugging
        //cout<<"(*iterEta).second.hEta_ClustADC->GetName() = " << (*iterEta).second.hEta_ClustADC->GetName() << endl;
        
        //Loop Over Stored iPhi Sectors
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over iPhi Sectors
            
            //Initialize iPhi Histograms - 1D
            (*iterPhi).second.hPhi_ClustADC = std::make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, hSetupClust_ADC ) );
            (*iterPhi).second.hPhi_ClustMulti = std::make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, hSetupClust_Multi ) );
            (*iterPhi).second.hPhi_ClustSize = std::make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, hSetupClust_Size ) );
            
            //Initialize iPhi Histograms - 2D
            (*iterPhi).second.hPhi_ClustADC_v_ClustPos = std::make_shared<TH2F>( TH2F( ("hiEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "_ClustADC_v_ClustPos").c_str(),"Response Uniformity",200,-0.5*(*iterEta).second.fWidth,0.5*(*iterEta).second.fWidth,300,0,15000) );
            
            //Loop Over Stored Clusters
            for (auto iterClust = (*iterPhi).second.vec_clusters.begin(); iterClust != (*iterPhi).second.vec_clusters.end(); ++iterClust) { //Loop Over Stored Clusters
                //Fill iEta Histograms
                (*iterEta).second.hEta_ClustADC->Fill( (*iterClust).fADC );
                //(*iterEta).second.hEta_ClustMulti->Fill( (*iterClust). );
                (*iterEta).second.hEta_ClustPos->Fill( (*iterClust).fPos_X );
                (*iterEta).second.hEta_ClustSize->Fill( (*iterClust).iSize );
                
                (*iterEta).second.hEta_ClustADC_v_ClustPos->Fill( (*iterClust).fPos_X, (*iterClust).fADC );
                
                //Fill iPhi Histograms
                (*iterPhi).second.hPhi_ClustADC->Fill( (*iterClust).fADC );
                //(*iterPhi).second.hPhi_ClustMulti
                (*iterPhi).second.hPhi_ClustSize->Fill( (*iterClust).iSize);
                (*iterPhi).second.hPhi_ClustADC_v_ClustPos->Fill( (*iterClust).fPos_X, (*iterClust).fADC );
                
                //Slices?
                
                //To Be Implemented
                
            } //End Loop Over Stored Clusters
        } //End Loop Over iPhi Sectors
        
        std::cout<<"(*iterEta).second.hEta_ClustADC->Integral() = " << (*iterEta).second.hEta_ClustADC->Integral() << std::endl;
    } //End Loop Over iEta Sectors
    
} //End AnalyzeResponseUniformity::fillHistos() - Full Detector

//Loops over all stored clusters in a specific iEta sector of detMPGD and Book Histograms
//void AnalyzeResponseUniformity::fillHistos(int iEta){
void AnalyzeResponseUniformity::fillHistos(SectorEta &inputEta){
    
    //Not Implemented Yet
    
} //AnalyzeResponseUniformity::fillHistos() - Specific iEta Sector

//void AnalyzeResponseUniformity::fillHistos(int iEta, int iPhi){
void AnalyzeResponseUniformity::fillHistos(SectorPhi &inputPhi){
    
    //Not Implemented Yet
    
} //AnalysisSetupUniformity::fillHistos() - Specific iPhi

//Stores booked histograms (for those histograms that are non-null)
void AnalyzeResponseUniformity::storeHistos( string strOutputROOTFileName, std::string strOption ){
    //Variable Declaration
    //std::shared_ptr<TFile> ptr_fileOutput;
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Assign the TFile to the ptr_fileOutput
    //ptr_fileOutput = std::make_shared<TFile>(TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1) );
    
    //Check if File Failed to Open Correctly
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("AnalyzeResponseUniformity","storeHistos","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("AnalyzeResponseUniformity","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeResponseUniformity","storeHistos", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Loop over ieta's
        //Create/Load file structure
        //Store ieta level histograms
        //Loop over iphi's within ieta's
            //Create/Load file structure
            //Store iphi level histograms
                //Loop over slices
                    //Create/Load file structure
                    //store slice level histograms
    //Close File
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Get Directory
        TDirectory *dir_SectorEta = ptr_fileOutput->GetDirectory( ( "SectorEta" + getString( (*iterEta).first ) ).c_str(), false, "GetDirectory" );
        
        cout<<"dir_SectorEta->GetName() = " << dir_SectorEta->GetName()<<endl;
        
    } //End Loop Over Stored iEta Sectors
    
    return;
} //End storeHistos()

//Returns a directory of a TFile
//Checks to see if the dir exists, if it does not it is created.
/*TDirectory AnalyzeResponseUniformity::getDirectory(std::shared_ptr<TFile> inputFile, std::string strDirName){
    
}*/ //End getDirectory()

//Returns a histogram whose parmeters match those defined in hte input HistoSetup object
TH1F AnalyzeResponseUniformity::getHistogram(int iEta, int iPhi,HistoSetup &setupHisto){
    //Variable Declaration
    string strName = setupHisto.strHisto_Name;
    
    //Alter the name to reflect the location within the detector
    if (iPhi > -1){ //Case: Specific (iEta,iPhi) sector
        strName = "hiEta" + getString(iEta) + "iPhi" + getString(iPhi) + "_" + strName;
    } //End Case: Specific (iEta,iPhi) sector
    else{ //Case: iEta Sector, sum over sector's iPhi
        strName = "hiEta" + getString(iEta) + "_" + strName;
    } //End Case: iEta Sector, sum over sector's iPhi
    
    //Histo Declaration
    TH1F ret_Histo(strName.c_str(), "", setupHisto.iHisto_nBins, setupHisto.fHisto_xLower, setupHisto.fHisto_xUpper );

    //Set Histo Parameters
    ret_Histo.SetXTitle( setupHisto.strHisto_Title_X.c_str() );
    ret_Histo.SetYTitle( setupHisto.strHisto_Title_Y.c_str() );
    
    ret_Histo.Sumw2();
    
    //Return Histogram
    return ret_Histo;
} //End AnalyzeResponseUniformity::getHistogram()
