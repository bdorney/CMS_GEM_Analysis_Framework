//
//  AnalyzeResponseUniformityHits.cpp
//
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#include "DetectorMPGD.h"
#include "AnalyzeResponseUniformityHits.h"

using std::cout;
using std::endl;
using std::make_shared;
using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

using QualityControl::Timing::getString;
using QualityControl::Timing::printROOTFileStatus;
using QualityControl::Timing::HistoSetup;
using QualityControl::Timing::stofSafe;

using namespace QualityControl::Uniformity;

//Default Constructor
AnalyzeResponseUniformityHits::AnalyzeResponseUniformityHits(){
    strAnalysisName = "analysis";
} //End Default Constructor

//Set inputs at construction
AnalyzeResponseUniformityHits::AnalyzeResponseUniformityHits(AnalysisSetupUniformity inputSetup){
    strAnalysisName = "analysis";

    //Store Analysis Parameters
    aSetup = inputSetup;

    //Store Detector
    //detMPGD = inputDet;
} //End Constructor

//Loops over all stored hits in an input DetectorMPGD objectand fills histograms for the full detector
void AnalyzeResponseUniformityHits::fillHistos(DetectorMPGD & inputDet){
    //Variable Declaration
    std::multimap<int, Hit> map_hits;
    vector<int> vec_iEvtList;

    //Determine Hit Multiplicity - Detector Level
    map_hits = inputDet.getHits();
    vec_iEvtList = getVectorOfKeys( map_hits );
    for (auto iterEvt = vec_iEvtList.begin(); iterEvt != vec_iEvtList.end(); ++iterEvt) {
        inputDet.hMulti_Hit->Fill( map_hits.count( (*iterEvt) ) );
    }

    //Loop Over Stored iEta Sectors
    for (auto iterEta = inputDet.map_sectorsEta.begin(); iterEta != inputDet.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors

        //Determine Cluster Multiplicity - iEta Level
        map_hits = inputDet.getHits( (*iterEta).first );
        vec_iEvtList = getVectorOfKeys( map_hits );
        for (auto iterEvt = vec_iEvtList.begin(); iterEvt != vec_iEvtList.end(); ++iterEvt) {
            (*iterEta).second.hitHistos.hMulti->Fill( map_hits.count( (*iterEvt) ) );
        }

        //Loop Over Stored iPhi Sectors
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over iPhi Sectors

            //Determine Hit Multiplicity - iPhi Level
            vec_iEvtList = getVectorOfKeys( (*iterPhi).second.map_hits );
            for (auto iterEvt = vec_iEvtList.begin(); iterEvt != vec_iEvtList.end(); ++iterEvt) {
                (*iterPhi).second.hitHistos.hMulti->Fill( (*iterPhi).second.map_hits.count( (*iterEvt) ) );
            }

            //Loop Over Stored Hits
            for (auto iterHit = (*iterPhi).second.map_hits.begin(); iterHit != (*iterPhi).second.map_hits.end(); ++iterHit) { //Loop Over Stored Hits
                //Fill iEta Histograms
                (*iterEta).second.hitHistos.hADC->Fill( (*iterHit).second.vec_sADC[(*iterHit).second.iTimeBin] );
                (*iterEta).second.hitHistos.hPos->Fill( (*iterHit).second.iStripNum );
                (*iterEta).second.hitHistos.hTime->Fill( (*iterHit).second.iTimeBin );
	        	(*iterEta).second.hitHistos.hADCMax_v_ADCInt->Fill( (*iterHit).second.sADCIntegral, (*iterHit).second.vec_sADC[(*iterHit).second.iTimeBin] );

                //Fill iPhi Histograms
                (*iterPhi).second.hitHistos.hADC->Fill( (*iterHit).second.vec_sADC[(*iterHit).second.iTimeBin] );
                (*iterPhi).second.hitHistos.hTime->Fill( (*iterHit).second.iTimeBin);
		        (*iterPhi).second.hitHistos.hADCMax_v_ADCInt->Fill( (*iterHit).second.sADCIntegral, (*iterHit).second.vec_sADC[(*iterHit).second.iTimeBin] );
            } //End Loop Over Stored Hits
        } //End Loop Over iPhi Sectors
    } //End Loop Over iEta Sectors

    return;
} //End AnalyzeResponseUniformityHits::fillHistos() - Full Detector

//Loops over all stored hits in an input DetectorMPGD objectand fills histograms for the full detector
void AnalyzeResponseUniformityHits::findDeadStrips(DetectorMPGD & inputDet, string & strOutputTextFileName){
    //Variable Declaration
    int iNbinsX = 0;
    int iReadoutStripNum = -1;  //From [1,128]
    int iReadoutPhi = 1;

    float fBinContent = 0.;

    std::fstream file_DeadStripList;

    //Setup output file
    //------------------------------------------------------
    file_DeadStripList.open( strOutputTextFileName.c_str(), std::fstream::out );

    //Check to see if the config file opened successfully
    if (!file_DeadStripList.is_open()) {
        perror( ("AnalyzeResponseUniformityHits::findDeadStrips() - error while opening file: " + strOutputTextFileName).c_str() );
        Timing::printStreamStatus(file_DeadStripList);

        cout<<"\tCheck for dead strips is being skipped!\n";
        cout<<"\tExitting!!!\n";

        return;
    } //End Case: Input Not Understood

    file_DeadStripList<<"Cut_HitAdc_Min = "<<aSetup.selHit.iCut_ADCNoise<<endl;
    file_DeadStripList<<"iEta\tiPhi\tiStrip\n";

    //Check for dead strips
    //Loop Over Stored iEta Sectors
    //------------------------------------------------------
    for (auto iterEta = inputDet.map_sectorsEta.begin(); iterEta != inputDet.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors

        //Set the number of bins in this histogram (should be the same for all iEta);
        iNbinsX = (*iterEta).second.hitHistos.hPos->GetNbinsX();

        for(int i=1; i <=iNbinsX; ++i ){ //Loop Over Bins of (*iterEta).second.hitHistos.hPos

            fBinContent = (*iterEta).second.hitHistos.hPos->GetBinContent(i);

            //Check for dead strip
            if ( !(fBinContent > 0 ) ) { //Case: Dead Strip
                iReadoutPhi     = getPhiSectorVal(i, 128);
                iReadoutStripNum= getPhiStripNum(i, 128);

                file_DeadStripList<<(*iterEta).first<<"\t"<<iReadoutPhi<<"\t"<<iReadoutStripNum<<endl;
            } //End Case: Dead Strip
        } //End Loop Over Bins of (*iterEta).second.hitHistos.hPos
    } //End Loop Over iEta Sectors

    file_DeadStripList.close();

    return;
} //End AnalyzeResponseUniformityHits::fillHistos() - Full Detector

//Loops over all slices in inputDet and fits Booked histograms for the full detector
//Assumes Histos have been filled already (obviously)
void AnalyzeResponseUniformityHits::fitHistos(DetectorMPGD & inputDet){

    //Placeholder

    return;
} //End AnalyzeResponseUniformityHits::fitHistos()

//Loops through the detector and initalizes all cluster histograms
void AnalyzeResponseUniformityHits::initHistosHits(DetectorMPGD & inputDet){
    //Loop Over Stored iEta Sectors
    HistoSetup hSetup_ADCIntegral = aSetup.histoSetup_hitADC;

    hSetup_ADCIntegral.strHisto_Name    = "hitADC_v_hitADCIntegral";
    hSetup_ADCIntegral.strHisto_Title_X = "Hit ADC Integral";
    hSetup_ADCIntegral.fHisto_xUpper    = 6. * hSetup_ADCIntegral.fHisto_xUpper;
    hSetup_ADCIntegral.iHisto_nBins     = 6. * hSetup_ADCIntegral.iHisto_nBins;

    for (auto iterEta = inputDet.map_sectorsEta.begin(); iterEta != inputDet.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        //Grab Eta Sector width (for ClustPos Histo)
        aSetup.histoSetup_hitPos.fHisto_xLower = 0.;
        aSetup.histoSetup_hitPos.fHisto_xUpper = 128. * (*iterEta).second.map_sectorsPhi.size();
        aSetup.histoSetup_hitPos.iHisto_nBins = 128. * (*iterEta).second.map_sectorsPhi.size();

        //Initialize iEta Histograms - 1D
        (*iterEta).second.hitHistos.hADC = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_hitADC ) );
        (*iterEta).second.hitHistos.hMulti = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_hitMulti ) );
        (*iterEta).second.hitHistos.hPos = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_hitPos ) );
        (*iterEta).second.hitHistos.hTime = make_shared<TH1F>(getHistogram((*iterEta).first, -1, aSetup.histoSetup_hitTime ) );

        //Initialize iEta Histograms - 2D
        (*iterEta).second.hitHistos.hADCMax_v_ADCInt = make_shared<TH2F>( getHistogram2D((*iterEta).first, -1, hSetup_ADCIntegral, aSetup.histoSetup_hitADC) );

        //Loop Over Stored iPhi Sectors
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over iPhi Sectors
            //Initialize iPhi Histograms - 1D
            (*iterPhi).second.hitHistos.hADC = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_hitADC ) );
            (*iterPhi).second.hitHistos.hMulti = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_hitMulti ) );
            (*iterPhi).second.hitHistos.hTime = make_shared<TH1F>(getHistogram( (*iterEta).first, (*iterPhi).first, aSetup.histoSetup_hitTime ) );

            //Initialize iPhi Histograms - 2D
            (*iterPhi).second.hitHistos.hADCMax_v_ADCInt = make_shared<TH2F>(getHistogram2D((*iterEta).first, (*iterPhi).first, hSetup_ADCIntegral, aSetup.histoSetup_hitADC) );
        } //End Loop Over iPhi Sectors
    } //End Loop Over iEta Sectors

    //Initialize histograms over the entire detector
    inputDet.hMulti_Hit = make_shared<TH1F>(getHistogram( -1, -1, aSetup.histoSetup_hitMulti ) );

    return;
} //End AnalyzeResponseUniformityHits::initHistosHits()

//Loads a ROOT file previously created by an instance of AnalyzeResponseUniformityHits
//Loads all TObjects found in the input ROOT file into inputDet;
//Any previously stored information in inputDet is lost.
void AnalyzeResponseUniformityHits::loadHistosFromFile( std::string & strInputMappingFileName, std::string & strInputROOTFileName ){

    //Placeholder

    return;
} //End AnalyzeResponseUniformityHits::loadHistosFromFile()

//Stores booked histograms (for those histograms that are non-null)
//Takes a std::string which stores the physical filename as input
void AnalyzeResponseUniformityHits::storeHistos( string & strOutputROOTFileName, std::string strOption, DetectorMPGD & inputDet){
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);

    //Check if File Failed to Open Correctly
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("AnalyzeResponseUniformityHits","storeHistos","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("AnalyzeResponseUniformityHits","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeResponseUniformityHits","storeHistos", "\tExiting; No Histograms have been stored!\n" );

        return;
    } //End Check if File Failed to Open Correctly

    //Call the store histos sequence
    storeHistos(ptr_fileOutput, inputDet);

    //Close the ROOT file
    ptr_fileOutput->Close();

    return;
} //End storeHistos()

//Stores booked histograms (for those histograms that are non-null)
//Takes a TFile * which the histograms are written to as input
void AnalyzeResponseUniformityHits::storeHistos(TFile * file_InputRootFile, DetectorMPGD & inputDet){
    //Variable Declaration

    //Check if File Failed to Open Correctly
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("AnalyzeResponseUniformityHits","storeHistos","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("AnalyzeResponseUniformityHits","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeResponseUniformityHits","storeHistos", "\tExiting; No Histograms have been stored!\n" );

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

    //Setup the summary histograms
    TH1F hHitADC_All( getHistogram(-1, -1, aSetup.histoSetup_hitADC) );
	TH1F hHitPos_All( getHistogram(-1, -1, aSetup.histoSetup_hitPos) );
	TH1F hHitTime_All( getHistogram(-1, -1, aSetup.histoSetup_hitTime) );

    //Get/Make the Summary Directory
    //Check to see if the directory exists already
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );

    //If the above pointer is null the directory does NOT exist, create it
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE

    //Loop Over Stored iEta Sectors
    for (auto iterEta = inputDet.map_sectorsEta.begin(); iterEta != inputDet.map_sectorsEta.end(); ++iterEta) { //Loop Over iEta Sectors
        //Get Directory
        //-------------------------------------
        //Check to see if dir_SectorEta directory exists already, if not create it
        TDirectory *dir_SectorEta = file_InputRootFile->GetDirectory( ( "SectorEta" + getString( (*iterEta).first ) ).c_str(), false, "GetDirectory" );
        if (dir_SectorEta == nullptr) { //Case: Directory did not exist in file, CREATE
            dir_SectorEta = file_InputRootFile->mkdir( ( "SectorEta" + getString( (*iterEta).first ) ).c_str() );
        } //End Case: Directory did not exist in file, CREATE

        //Add this sector to the summary histogram
        hHitADC_All.Add((*iterEta).second.hitHistos.hADC.get() );
        hHitPos_All.Add((*iterEta).second.hitHistos.hPos.get() );
        hHitTime_All.Add((*iterEta).second.hitHistos.hTime.get() );

        //Store Histograms - ReadoutSectorEta Level
        //-------------------------------------
        dir_SectorEta->cd();
        (*iterEta).second.hitHistos.hADC->Write();
        (*iterEta).second.hitHistos.hMulti->Write();
        (*iterEta).second.hitHistos.hPos->Write();
        (*iterEta).second.hitHistos.hTime->Write();
        (*iterEta).second.hitHistos.hADCMax_v_ADCInt->Write();

        //Loop Over Stored iPhi Sectors within this iEta Sector
        for (auto iterPhi = (*iterEta).second.map_sectorsPhi.begin(); iterPhi != (*iterEta).second.map_sectorsPhi.end(); ++iterPhi) { //Loop Over Stored iPhi Sectors
            //Get Directory
            //-------------------------------------
            //Check to see if dir_SectorPhi directory exists already, if not create it
            TDirectory *dir_SectorPhi = dir_SectorEta->GetDirectory( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str(), false, "GetDirectory"  );
            if (dir_SectorPhi == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_SectorPhi = dir_SectorEta->mkdir( ( "SectorPhi" + getString( (*iterPhi).first ) ).c_str() );
            } //End Case: Directory did not exist in file, CREATE

            //Store Histograms - ReadoutSectorPhi Level
            //-------------------------------------
            dir_SectorPhi->cd();
            (*iterPhi).second.hitHistos.hADC->Write();
            (*iterPhi).second.hitHistos.hMulti->Write();
            (*iterPhi).second.hitHistos.hTime->Write();
            (*iterPhi).second.hitHistos.hADCMax_v_ADCInt->Write();
        } //End Loop Over Stored iPhi Sectors
    } //End Loop Over Stored iEta Sectors

    //Store the Summary Histograms
    dir_Summary->cd();
    hHitADC_All.Write();
    inputDet.hMulti_Hit->Write();
	hHitPos_All.Write();
	hHitTime_All.Write();

    //Do not close the file it is used elsewhere

    return;
} //End storeHistos()

void AnalyzeResponseUniformityHits::storeFits( string & strOutputROOTFileName, std::string strOption, DetectorMPGD & inputDet){

    //Placeholder

} //End storeHistos()
