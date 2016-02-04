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
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

using Timing::getString;
using Timing::printROOTFileStatus;
using Timing::HistoSetup;

using namespace Uniformity;

//Default Constructor
AnalyzeResponseUniformity::AnalyzeResponseUniformity(){
    strAnalysisName = "analysis";
    
    //Setup Histo Setup Containers
    //Cluster ADC Histos
    /*hSetupClust_ADC.fHisto_xLower = 0;
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
    
    //Cluster Time
    hSetupClust_Time.fHisto_xLower = 0;
    hSetupClust_Time.fHisto_xUpper = 15;
    
    hSetupClust_Time.iHisto_nBins = 15;
    
    hSetupClust_Time.strHisto_Name = "clustTime";
    hSetupClust_Time.strHisto_Title_X = "Time Bin";
    hSetupClust_Time.strHisto_Title_Y = "N";*/
} //End Default Constructor

//Set inputs at construction
AnalyzeResponseUniformity::AnalyzeResponseUniformity(AnalysisSetupUniformity inputSetup, DetectorMPGD inputDet){
    strAnalysisName = "analysis";
    
    //Setup Histo Setup Containers
    //Cluster ADC Histos
    /*hSetupClust_ADC.fHisto_xLower = 0;
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
    
    //Cluster Time
    hSetupClust_Time.fHisto_xLower = 0;
    hSetupClust_Time.fHisto_xUpper = 15;
    
    hSetupClust_Time.iHisto_nBins = 15;
    
    hSetupClust_Time.strHisto_Name = "clustTime";
    hSetupClust_Time.strHisto_Title_X = "Time Bin";
    hSetupClust_Time.strHisto_Title_Y = "N";*/
    
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
        
        //Grab Eta Sector width (for ClustPos Histo)
        aSetup.histoSetup_clustPos.fHisto_xLower = -0.5*(*iterEta).second.fWidth;
        aSetup.histoSetup_clustPos.fHisto_xUpper = 0.5*(*iterEta).second.fWidth;
        
        //Initialize iEta Histograms - 1D
        (*iterEta).second.hEta_ClustADC = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_clustADC ) );
        (*iterEta).second.hEta_ClustMulti = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_clustMulti ) );
        (*iterEta).second.hEta_ClustPos = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_clustPos ) );
        (*iterEta).second.hEta_ClustSize = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_clustSize ) );
        (*iterEta).second.hEta_ClustTime = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_clustTime ) );
        
        //Initialize iEta Histograms - 2D
        (*iterEta).second.hEta_ClustADC_v_ClustPos = make_shared<TH2F>( TH2F( ("hiEta" + getString( (*iterEta).first ) + "_ClustADC_v_ClustPos").c_str(),"Response Uniformity", 3. * aSetup.iUniformityGranularity,-0.5*(*iterEta).second.fWidth,0.5*(*iterEta).second.fWidth,300,0,15000) );
        (*iterEta).second.hEta_ClustADC_v_ClustPos->Sumw2();
        
        //Debugging
        //cout<<"(*iterEta).second.hEta_ClustADC->GetName() = " << (*iterEta).second.hEta_ClustADC->GetName() << endl;
        
        //Loop Over Stored iPhi Sectors
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over iPhi Sectors
            
            //Initialize iPhi Histograms - 1D
            (*iterPhi).second.hPhi_ClustADC = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_clustADC ) );
            (*iterPhi).second.hPhi_ClustMulti = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_clustMulti ) );
            (*iterPhi).second.hPhi_ClustSize = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_clustSize ) );
            (*iterPhi).second.hPhi_ClustTime = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_clustTime ) );
            
            //Initialize iPhi Histograms - 2D
            (*iterPhi).second.hPhi_ClustADC_v_ClustPos = make_shared<TH2F>( TH2F( ("hiEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "_ClustADC_v_ClustPos").c_str(),"Response Uniformity", aSetup.iUniformityGranularity, (*iterPhi).second.fPos_Xlow, (*iterPhi).second.fPos_Xhigh,300,0,15000) );
            (*iterPhi).second.hPhi_ClustADC_v_ClustPos->Sumw2();
            
            //Loop Over Stored Clusters
            for (auto iterClust = (*iterPhi).second.vec_clusters.begin(); iterClust != (*iterPhi).second.vec_clusters.end(); ++iterClust) { //Loop Over Stored Clusters
                //Fill iEta Histograms
                (*iterEta).second.hEta_ClustADC->Fill( (*iterClust).fADC );
                //(*iterEta).second.hEta_ClustMulti->Fill( (*iterClust). );
                (*iterEta).second.hEta_ClustPos->Fill( (*iterClust).fPos_X );
                (*iterEta).second.hEta_ClustSize->Fill( (*iterClust).iSize );
                (*iterEta).second.hEta_ClustTime->Fill( (*iterClust).iTimeBin );
                
                (*iterEta).second.hEta_ClustADC_v_ClustPos->Fill( (*iterClust).fPos_X, (*iterClust).fADC );
                
                //Fill iPhi Histograms
                (*iterPhi).second.hPhi_ClustADC->Fill( (*iterClust).fADC );
                //(*iterPhi).second.hPhi_ClustMulti
                (*iterPhi).second.hPhi_ClustSize->Fill( (*iterClust).iSize);
                (*iterPhi).second.hPhi_ClustTime->Fill( (*iterClust).iTimeBin);
                
                (*iterPhi).second.hPhi_ClustADC_v_ClustPos->Fill( (*iterClust).fPos_X, (*iterClust).fADC );
            } //End Loop Over Stored Clusters
            
            //Slices
            //Now that all clusters have been analyzed we extract the slices
            for (int i=1; i<= aSetup.iUniformityGranularity; ++i) { //Loop Over Slices
                //Create the slice
                SectorSlice slice;
                
                //string strSliceName = "hiEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "Slice" + getString(i) + "_ClustADC";
                
                //Grab ADC spectrum for this slice
                slice.hSlice_ClustADC = make_shared<TH1F>( *( (TH1F*) (*iterPhi).second.hPhi_ClustADC_v_ClustPos->ProjectionY( ("hiEta" + getString( (*iterEta).first ) + "iPhi" + getString( (*iterPhi).first ) + "Slice" + getString(i) + "_ClustADC").c_str(),i,i,"") ) );
                
                //Store position information for this slice
                slice.fPos_Center = (*iterPhi).second.hPhi_ClustADC_v_ClustPos->GetXaxis()->GetBinCenter(i);
                slice.fWidth = (*iterPhi).second.hPhi_ClustADC_v_ClustPos->GetXaxis()->GetBinWidth(i);
                
                //Store the slice
                (*iterPhi).second.map_slices[i] = slice;
            } //End Loop Over Slices
        } //End Loop Over iPhi Sectors
        
        std::cout<<"(*iterEta).second.hEta_ClustADC->Integral() = " << (*iterEta).second.hEta_ClustADC->Integral() << std::endl;
    } //End Loop Over iEta Sectors
    
} //End AnalyzeResponseUniformity::fillHistos() - Full Detector

//Loops over all slices in detMPGD and fits Booked histograms for the full detector
//Assumes Histos have been filled already (obviously)
void AnalyzeResponseUniformity::fitHistos(){
    //Variable Declaration
    TSpectrum specADC(1,2);    //One peak; 2 sigma away from any other peak
    Double_t *dPeakPos;
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Initialize Response uniformity graphs - Fit Mean
        (*iterEta).second.gEta_ClustADCFitRes_Response = make_shared<TGraphErrors>( TGraphErrors( aSetup.iUniformityGranularity * (*iterEta).second.map_sectorsPhi.size() ) );
        
        (*iterEta).second.gEta_ClustADCFitRes_Response->SetName( ( getNameByIndex( (*iterEta).first, -1, -1, "g", "ClustADCFitRes_Response" ) ).c_str() );
        
        //
        (*iterEta).second.gEta_ClustADCFitRes_NormChi2 = make_shared<TGraphErrors>( TGraphErrors( aSetup.iUniformityGranularity * (*iterEta).second.map_sectorsPhi.size() ) );
        
        (*iterEta).second.gEta_ClustADCFitRes_NormChi2->SetName( ( getNameByIndex( (*iterEta).first, -1, -1, "g", "ClustADCFitRes_NormChi2" ) ).c_str() );
        
        //Loop Over Stored iPhi Sectors
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over iPhi Sectors
        
            //Loop Over Stored Slices
            for (auto iterSlice = (*iterPhi).second.map_slices.begin(); iterSlice != (*iterPhi).second.map_slices.end(); ++iterSlice ) { //Loop Over Slices
                
                //Check if Histogram does not exist
                if ( (*iterSlice).second.hSlice_ClustADC == nullptr) continue;
                
                //Initialize Fit
                (*iterSlice).second.fitSlice_ClustADC = make_shared<TF1>( getFit( (*iterEta).first, (*iterPhi).first, (*iterSlice).first, aSetup.histoSetup_clustADC, (*iterSlice).second.hSlice_ClustADC) );
                
                //Find peak & store it's position
                //specADC.Search( hInput.get(), 2, "nobackground", 0.5 );
                //dPeakPos = spec.GetPositionX();
                
                //Perform Fit
                (*iterSlice).second.hSlice_ClustADC->Fit( (*iterSlice).second.fitSlice_ClustADC.get(),aSetup.strFit_Option.c_str(),"");//, dPeakPos[0]-600., dPeakPos[0]+600. );
                
                //Determine which point in the TGraphs this is
                int iPoint = std::distance( (*iterPhi).second.map_slices.begin(), iterSlice) + aSetup.iUniformityGranularity * std::distance((*iterEta).second.map_sectorsPhi.begin(), iterPhi);
                
                cout<<"iPoint = " << iPoint << endl;
                
                //Store Fit parameters - Peak Position
                (*iterEta).second.gEta_ClustADCFitRes_Response->SetPoint(iPoint, (*iterSlice).second.fPos_Center, getPeakPos( (*iterSlice).second.fitSlice_ClustADC, aSetup.histoSetup_clustADC ) );
                //(*iterEta).second.gEta_ClustADCFitRes_Response->SetPointError(iPoint, (*iterSlice).second.fWidth, (*iterSlice).second.fitSlice_ClustADC->GetParError( ???? ) );
                
                //Store Fit parameters - NormChi2
                (*iterEta).second.gEta_ClustADCFitRes_NormChi2->SetPoint(iPoint, (*iterSlice).second.fPos_Center, (*iterSlice).second.fitSlice_ClustADC->GetChisquare() / (*iterSlice).second.fitSlice_ClustADC->GetNDF() );
                (*iterEta).second.gEta_ClustADCFitRes_NormChi2->SetPointError(iPoint, 0.5 * (*iterSlice).second.fWidth, 0. );
            } //End Loop Over Slices
        } //End Loop Over iPhi Sectors
    } //End Loop Over iEta Sectors
    
    return;
} //End AnalyzeResponseUniformity::fitHistos()

//Loops over all stored clusters in a specific iEta sector of detMPGD and Book Histograms
//void AnalyzeResponseUniformity::fillHistos(int iEta){
/*void AnalyzeResponseUniformity::fillHistos(SectorEta &inputEta){
    
    //Not Implemented Yet
    
}*/ //AnalyzeResponseUniformity::fillHistos() - Specific iEta Sector

//void AnalyzeResponseUniformity::fillHistos(int iEta, int iPhi){
/*void AnalyzeResponseUniformity::fillHistos(SectorPhi &inputPhi){
    
    //Not Implemented Yet
    
}*/ //AnalysisSetupUniformity::fillHistos() - Specific iPhi

//Stores booked histograms (for those histograms that are non-null)
void AnalyzeResponseUniformity::storeHistos( string strOutputROOTFileName, std::string strOption ){
    //Variable Declaration
    //std::shared_ptr<TFile> ptr_fileOutput;
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Assign the TFile to the ptr_fileOutput
    //ptr_fileOutput = make_shared<TFile>(TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1) );
    
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
        //-------------------------------------
        //Check to see if the directory exists already
        TDirectory *dir_SectorEta = ptr_fileOutput->GetDirectory( ( "SectorEta" + getString( (*iterEta).first ) ).c_str(), false, "GetDirectory" );
        
        //If the above pointer is null the directory does NOT exist, create it
        if (dir_SectorEta == nullptr) { //Case: Directory did not exist in file, CREATE
            dir_SectorEta = ptr_fileOutput->mkdir( ( "SectorEta" + getString( (*iterEta).first ) ).c_str() );
        } //End Case: Directory did not exist in file, CREATE
        
        //Debugging
        cout<<"dir_SectorEta->GetName() = " << dir_SectorEta->GetName()<<endl;
        
        //Store Histograms - SectorEta Level
        //-------------------------------------
        dir_SectorEta->cd();
        (*iterEta).second.hEta_ClustADC->Write();
        (*iterEta).second.hEta_ClustPos->Write();
        (*iterEta).second.hEta_ClustSize->Write();
        (*iterEta).second.hEta_ClustADC_v_ClustPos->Write();
        
        //Loop Over Stored iPhi Sectors within this iEta Sector
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Stored iPhi Sectors
            //Get Directory
            //-------------------------------------
            //Check to see if the directory exists already
            TDirectory *dir_SectorPhi = dir_SectorEta->GetDirectory( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str(), false, "GetDirectory"  );
            
            //If the above pointer is null the directory does NOT exist, create it
            if (dir_SectorPhi == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_SectorPhi = dir_SectorEta->mkdir( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str() );
            } //End Case: Directory did not exist in file, CREATE
            
            //Debugging
            cout<<"dir_SectorPhi->GetName() = " << dir_SectorPhi->GetName()<<endl;
            
            //Store Histograms - SectorPhi Level
            //-------------------------------------
            dir_SectorPhi->cd();
            (*iterPhi).second.hPhi_ClustADC->Write();
            (*iterPhi).second.hPhi_ClustSize->Write();
            (*iterPhi).second.hPhi_ClustADC_v_ClustPos->Write();
            
            //Loop through Slices
                //To be implemented
            
            //Slices
            //Now that all clusters have been analyzed we extract the slices
            for (auto iterSlice = (*iterPhi).second.map_slices.begin(); iterSlice != (*iterPhi).second.map_slices.end(); ++iterSlice ) { //Loop Over Slices
                
                int iSliceCount = std::distance( (*iterPhi).second.map_slices.begin(), iterSlice );
                
                //Get Directory
                //-------------------------------------
                //Check to see if the directory exists already
                TDirectory *dir_Slice = dir_SectorPhi->GetDirectory( ( "Slice" + getString( iSliceCount ) ).c_str(), false, "GetDirectory"  );
                
                //If the above pointer is null the directory does NOT exist, create it
                if (dir_Slice == nullptr) { //Case: Directory did not exist in file, CREATE
                    dir_Slice = dir_SectorPhi->mkdir( ( "Slice" + getString( iSliceCount ) ).c_str() );
                } //End Case: Directory did not exist in file, CREATE
                
                //Store Histograms - Slice Level
                //-------------------------------------
                dir_Slice->cd();
                (*iterSlice).second.hSlice_ClustADC->Write();
            } //End Loop Over Slices
        } //End Loop Over Stored iPhi Sectors
    } //End Loop Over Stored iEta Sectors
    
    //Close the ROOT file
    ptr_fileOutput->Close();
    
    return;
} //End storeHistos()

void AnalyzeResponseUniformity::storeFits( string strOutputROOTFileName, std::string strOption ){
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("AnalyzeResponseUniformity","storeFits","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("AnalyzeResponseUniformity","storeFits", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeResponseUniformity","storeFits", "\tExiting; No Fits have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Loop Over Stored iEta Sectors
    for (auto iterEta = detMPGD.map_sectorsEta.begin(); iterEta != detMPGD.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        
        //Get Directory
        //-------------------------------------
        //Check to see if the directory exists already
        TDirectory *dir_SectorEta = ptr_fileOutput->GetDirectory( ( "SectorEta" + getString( (*iterEta).first ) ).c_str(), false, "GetDirectory" );
        
        //If the above pointer is null the directory does NOT exist, create it
        if (dir_SectorEta == nullptr) { //Case: Directory did not exist in file, CREATE
            dir_SectorEta = ptr_fileOutput->mkdir( ( "SectorEta" + getString( (*iterEta).first ) ).c_str() );
        } //End Case: Directory did not exist in file, CREATE
        
        //Debugging
        //cout<<"dir_SectorEta->GetName() = " << dir_SectorEta->GetName()<<endl;
        
        //Store Fits - SectorEta Level
        //-------------------------------------
        dir_SectorEta->cd();
        (*iterEta).second.gEta_ClustADCFitRes_Response->Write();
        (*iterEta).second.gEta_ClustADCFitRes_NormChi2->Write();
        
        //Loop Over Stored iPhi Sectors within this iEta Sector
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Stored iPhi Sectors
            //Get Directory
            //-------------------------------------
            //Check to see if the directory exists already
            TDirectory *dir_SectorPhi = dir_SectorEta->GetDirectory( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str(), false, "GetDirectory"  );
            
            //If the above pointer is null the directory does NOT exist, create it
            if (dir_SectorPhi == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_SectorPhi = dir_SectorEta->mkdir( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str() );
            } //End Case: Directory did not exist in file, CREATE
            
            //Debugging
            //cout<<"dir_SectorPhi->GetName() = " << dir_SectorPhi->GetName()<<endl;
            
            //Store Fits - SectorPhi Level
            //-------------------------------------
            dir_SectorPhi->cd();
            
            //No Fits defined at this level - yet

            //Slices
            //Now that all clusters have been analyzed we extract the slices
            for (auto iterSlice = (*iterPhi).second.map_slices.begin(); iterSlice != (*iterPhi).second.map_slices.end(); ++iterSlice ) { //Loop Over Slices
                
                int iSliceCount = std::distance( (*iterPhi).second.map_slices.begin(), iterSlice );
                
                //Get Directory
                //-------------------------------------
                //Check to see if the directory exists already
                TDirectory *dir_Slice = dir_SectorPhi->GetDirectory( ( "Slice" + getString( iSliceCount ) ).c_str(), false, "GetDirectory"  );
                
                //If the above pointer is null the directory does NOT exist, create it
                if (dir_Slice == nullptr) { //Case: Directory did not exist in file, CREATE
                    dir_Slice = dir_SectorPhi->mkdir( ( "Slice" + getString( iSliceCount ) ).c_str() );
                } //End Case: Directory did not exist in file, CREATE
                
                //Store Fits - Slice Level
                //-------------------------------------
                dir_Slice->cd();
                (*iterSlice).second.fitSlice_ClustADC->Write();
            } //End Loop Over Slices
        } //End Loop Over Stored iPhi Sectors
    } //End Loop Over Stored iEta Sectors
    
    //Close the ROOT file
    ptr_fileOutput->Close();
    
    return;
} //End storeHistos()

TF1 AnalyzeResponseUniformity::getFit(int iEta, int iPhi, int iSlice, HistoSetup & setupHisto, shared_ptr<TH1F> hInput ){
    //Variable Declaration
    TF1 ret_Func( getNameByIndex(iEta, iPhi, iSlice, "fit", setupHisto.strHisto_Name).c_str(), setupHisto.strFit_Formula.c_str(), setupHisto.fHisto_xLower, setupHisto.fHisto_xUpper );
    
	//cout<<"setupHisto.strFit_Formula.c_str() = " << setupHisto.strFit_Formula.c_str() << endl;

    //Set Fit Parameters
    //------------------------------------------------------
    //Check to see if user has correctly linking the meaning of the fit parameters and their initial guess
    if ( setupHisto.vec_strFit_ParamMeaning.size() == setupHisto.vec_strFit_ParamIGuess.size() ) {
        
        //AMPLITUDE, MEAN, PEAK, SIGMA
        //We assume the user has correctly matched the indices of setupHisto.vec_strFit_ParamMeaning and setupHisto.vec_strFit_ParamIGuess to the formula given in setupHisto.strFit_Formula
        for (int i=0; i<setupHisto.vec_strFit_ParamMeaning.size(); ++i) {
            //Try to automatically assign a value
            if (0 == setupHisto.vec_strFit_ParamIGuess[i].compare("AMPLITUDE") ) { //Case: Histo Amplitude
                ret_Func.SetParameter(i, hInput->GetBinContent( hInput->GetMaximumBin() ) );
            } //End Case: Histo Amplitude
            else if (0 == setupHisto.vec_strFit_ParamIGuess[i].compare("MEAN") ) { //Case: Histo Mean
                ret_Func.SetParameter(i, hInput->GetMean() );
            } //End Case: Histo Mean
            else if ( 0 == setupHisto.vec_strFit_ParamIGuess[i].compare("PEAK") ){ //Case: Histo Peak
                TSpectrum spec;    //One peak; 2 sigma away from any other peak
                
                Double_t *dPeakPos;
                
                //Find peak & store it's position
                spec.Search( hInput.get(), 2, "nobackground", 0.5 );
                dPeakPos = spec.GetPositionX();
                
                //Set initial guess for fit
                ret_Func.SetParameter(i, dPeakPos[0] );
            } //End Case: Histo Peak
            else if (0 == setupHisto.vec_strFit_ParamIGuess[i].compare("SIGMA") ) { //Case: Histo RMS
                ret_Func.SetParameter(i, hInput->GetRMS() );
            } //End Case: Histo RMS
            else{ //Case: manual assignment
                ret_Func.SetParameter(i, Timing::stofSafe("Fit_Param_IGuess", setupHisto.vec_strFit_ParamIGuess[i] ) );
            } //End Case: manual assignment
        } //End Loop over parameters
    } //End Case: equal number of inputs, containers (should be) linked!!!
    else{
        cout<<"Uniformity::AnalyzeResponseUniformity::getFit() - Trying to set initial parameters for:\n";
        cout<<"\tHisto Name = " << hInput->GetName() << endl;
        cout<<"\tFunction Name = " << ret_Func.GetName() << endl;
        
        cout<<"\tFit_Param_Map (Parameter Meaning) Given:\n\t\t";
        for (int i = 0; i < setupHisto.vec_strFit_ParamMeaning.size(); ++i) {
            cout<<setupHisto.vec_strFit_ParamMeaning[i]<<"\t";
        }
        cout<<endl;
        
        cout<<"tFit_Param_IGuess (Parameter Initial Guess) Given:\n\t\t";
        for (int i = 0; i < setupHisto.vec_strFit_ParamIGuess.size(); ++i) {
            cout<<setupHisto.vec_strFit_ParamIGuess[i]<<"\t";
        }
        cout<<endl;
        cout<<"\tParameters not linked!!! Please cross check input analysis config file!!!\n";
    } //End Case: vec_strFit_ParamIGuess not linked with vec_strFit_ParamMeaning; skip
    
    //Set Fit Data Members
    ret_Func.SetLineColor(kRed);
    ret_Func.SetLineWidth(3);
    
    //Return fit
    return ret_Func;
} //End AnalyzeResponseUniformity::getFit()

TGraphErrors AnalyzeResponseUniformity::getGraph(int iEta, int iPhi, HistoSetup & setupHisto){
    //Variable Declaration
    string strPrefix = "g";
    string strName = getNameByIndex(iEta, iPhi, -1, strPrefix, setupHisto.strHisto_Name);
    
    //Graph Declaration
    TGraphErrors ret_Graph;
    
    //Set Histo Data Members
    ret_Graph.GetXaxis()->SetTitle( setupHisto.strHisto_Title_X.c_str() );
    
    return ret_Graph;
} //End AnalyzeResponseUniformity::getGraph()

//Returns a histogram whose parmeters match those defined in hte input HistoSetup object
TH1F AnalyzeResponseUniformity::getHistogram(int iEta, int iPhi, HistoSetup &setupHisto){
    //Variable Declaration
    string strPrefix = "h";
    string strName = getNameByIndex(iEta, iPhi, -1, strPrefix, setupHisto.strHisto_Name);
    
    //Alter the name to reflect the location within the detector
    /*if (iPhi > -1){ //Case: Specific (iEta,iPhi) sector
        strName = "hiEta" + getString(iEta) + "iPhi" + getString(iPhi) + "_" + strName;
    } //End Case: Specific (iEta,iPhi) sector
    else{ //Case: iEta Sector, sum over sector's iPhi
        strName = "hiEta" + getString(iEta) + "_" + strName;
    }*/ //End Case: iEta Sector, sum over sector's iPhi
    
    //Histo Declaration
    TH1F ret_Histo(strName.c_str(), "", setupHisto.iHisto_nBins, setupHisto.fHisto_xLower, setupHisto.fHisto_xUpper );

    //Set Histo Data Members
    ret_Histo.SetXTitle( setupHisto.strHisto_Title_X.c_str() );
    ret_Histo.SetYTitle( setupHisto.strHisto_Title_Y.c_str() );
    
    ret_Histo.Sumw2();
    
    //Return Histogram
    return ret_Histo;
} //End AnalyzeResponseUniformity::getHistogram()


//Formats a given input string such that it follows the iEta, iPhi, iSlice naming convention
string AnalyzeResponseUniformity::getNameByIndex(int iEta, int iPhi, int iSlice, std::string & strInputPrefix, std::string & strInputName){
    //Variable Declaration
    string ret_Name = "";
    
    if (iSlice > -1) {
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "iPhi" + getString(iPhi) + "Slice" + getString(iSlice) + "_" + strInputName;
    }
    else if (iPhi > -1){ //Case: Specific (iEta,iPhi) sector
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "iPhi" + getString(iPhi) + "_" + strInputName;
    } //End Case: Specific (iEta,iPhi) sector
    else{ //Case: iEta Sector, sum over sector's iPhi
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "_" + strInputName;
    } //End Case: iEta Sector, sum over sector's iPhi
    
    return ret_Name;
} //End AnalyzeResponseUniformity::getNameByIndex()

//Formats a given input string such that it follows the iEta, iPhi, iSlice naming convention
//Will take text written in quotes without requiring a variable
string AnalyzeResponseUniformity::getNameByIndex(int iEta, int iPhi, int iSlice, const std::string & strInputPrefix, const std::string & strInputName){
    //Variable Declaration
    string ret_Name = "";
    
    if (iSlice > -1) {
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "iPhi" + getString(iPhi) + "Slice" + getString(iSlice) + "_" + strInputName;
    }
    else if (iPhi > -1){ //Case: Specific (iEta,iPhi) sector
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "iPhi" + getString(iPhi) + "_" + strInputName;
    } //End Case: Specific (iEta,iPhi) sector
    else{ //Case: iEta Sector, sum over sector's iPhi
        ret_Name = strInputPrefix + "_iEta" + getString(iEta) + "_" + strInputName;
    } //End Case: iEta Sector, sum over sector's iPhi

    return ret_Name;
} //End AnalyzeResponseUniformity::getNameByIndex()

float AnalyzeResponseUniformity::getPeakPos( shared_ptr<TF1> fitInput, HistoSetup & setupHisto ){
    
    //Search the peak parameter meaning vector for "PEAK"
    //If found return this parameter
    
    //If not found, return -1;
    //warn the user
    
    //Variable Declaration
    int iParamPos = -1;
    
    float ret_Val = -1;
    
    vector<string>::iterator iterParamMeaning = std::find(setupHisto.vec_strFit_ParamMeaning.begin(), setupHisto.vec_strFit_ParamMeaning.end(), "PEAK");
    
    if ( iterParamMeaning != setupHisto.vec_strFit_ParamMeaning.end() ) { //Case: Parameter Found!!!
        
        iParamPos = std::distance(setupHisto.vec_strFit_ParamMeaning.begin(), iterParamMeaning);
        
        ret_Val = fitInput->GetParameter(iParamPos);
    } //End Case: Parameter Found!!!
    else{ //Case: Parameter NOT Found
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","Error! - I Do not know which parameter in your fit function represents the peak!\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\tPlease Cross-check input analysis config file.\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\tEnsure the field 'Fit_Param_Map' has a value 'PEAK' and the posi 'PEAK' matches\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\tThe position of 'PEAK' in the list must match the numeric index of the parameter\n");
        printClassMethodMsg("AnalyzeResponseUniformity","getPeakPos","\te.g. if Parameter [2] represents the spectrum peak than 'PEAK' should be the third member in the list given to 'Fit_Param_Map'\n");
    } //End Case: Parameter NOT Foun
    
    return ret_Val;
} //End AnalyzeResponseUniformity::getPeakPos
