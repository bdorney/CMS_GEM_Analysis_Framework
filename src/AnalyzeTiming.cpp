//
//  AnalyzeTiming.cpp
//  
//
//  Created by Brian L Dorney on 21/11/16.
//
//

#include "AnalyzeTiming.h"

using std::cin;
using std::cout;
using std::endl;
using std::make_shared;
using std::map;
using std::pair;
using std::string;

using namespace QualityControl::Timing;

using QualityControl::Uniformity::printClassMethodMsg;

using QualityControl::Timing::imap_iter_t;

//Fills Histograms for all events
void AnalyzeTiming::fillHistos(std::vector<Timing::EventReco> vec_inputEvtsReco){
    //Variable Declaration
    int iEvt = 0;
    
    //int iMulti_DUT = 0;     //Multiplicity Counters
    //int iMulti_Trig = 0;
    int iMulti_Chan = 0;
    
    //Debugging
    //cout<<"AnalyzeTiming::fillHistos() - vec_inputEvtsReco.size() = " << vec_inputEvtsReco.size() << endl;
    
    for (auto iterEvt = vec_inputEvtsReco.begin(); iterEvt != vec_inputEvtsReco.end(); ++iterEvt) { //Loop Over Events
        if (iEvt % 1000 == 0) cout<<"Filling Histos; " <<iEvt<<" Events Processed\n";
        
        //Fill Multiplicity
        //m_mtrxHistos.m_hMultiDUT->Fill( (*iterEvt).m_detMatrix_DUTs.m_map_detectors.size() );
        //m_mtrxHistos.m_hMultiTrig->Fill( (*iterEvt).m_detMatrix_Trigger.m_map_detectors.size() );
        
        //Loop Through Superchambers - DUT's
        for (auto iterSC = (*iterEvt).m_detMatrix_DUTs.begin(); iterSC != (*iterEvt).m_detMatrix_DUTs.end(); ++iterSC) { //Loop Over Superchambers
            
            //Loop Over (*iterSC).second's Detectors
            for (auto iterDet = (*iterSC).second.getDetectorPtrBegin(); iterDet != (*iterSC).second.getDetectorPtrEnd(); ++iterDet) {
                
                for (auto iterChan = (*iterDet).second.getChanDataBegin(); iterChan != (*iterDet).second.getChanDataEnd(); ++iterChan) { //Loop Over Detector's Channels
                    
                    //Skip Empty Channels
                    if ( !( (*iterChan).second > 0) ) {
                        continue;
                    }
                    
                    //cout<<(*iterChan).first<<"\t"<<(*iterChan).second<<endl;
                    
                    //Fill Superchamber
                    m_mtrxHistos[(*iterSC).first].m_hAll->Fill( (*iterChan).second );
                    
                    //Fill Detector
                    m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_hAll->Fill( (*iterChan).second );
                    
			//Debugging
			//cout<<"m_mtrxHistos[("<<(*iterSC).first.first<<","<<(*iterSC).first.second<<")].m_map_HistosDet["<<(*iterDet).second.getName().c_str()<<"].m_map_hChan["<<(*iterChan).first<<"] = " << m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_map_hChan[(*iterChan).first] << endl;

                    m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_map_hChan[(*iterChan).first]->Fill( (*iterChan).second );
                }  //End Loop Over Detector's Channels
            } //End Loop Over Detectors
        } //End Loop Over Superchambers - DUT's
        
        //Loop Through Superchambers - Trig
        for (auto iterSC = (*iterEvt).m_detMatrix_Trigger.begin(); iterSC != (*iterEvt).m_detMatrix_Trigger.end(); ++iterSC) { //Loop Over Superchambers
            
            //Loop Over (*iterSC).second's Detectors
            for (auto iterDet = (*iterSC).second.getDetectorPtrBegin(); iterDet != (*iterSC).second.getDetectorPtrEnd(); ++iterDet) {
                
                for (auto iterChan = (*iterDet).second.getChanDataBegin(); iterChan != (*iterDet).second.getChanDataEnd(); ++iterChan) { //Loop Over Detector's Channels
                    
                    //Skip Empty Channels
                    if ( !( (*iterChan).second > 0) ) {
                        continue;
                    }
                    
			//Debugging
                    //cout<<(*iterChan).first<<"\t"<<(*iterChan).second<<endl;
                    
                    //Fill Superchamber
                    m_mtrxHistos[(*iterSC).first].m_hAll->Fill( (*iterChan).second );
                    
                    //Fill Detector
                    m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_hAll->Fill( (*iterChan).second );
                    
                    m_mtrxHistos[(*iterSC).first].m_map_HistosDet[(*iterDet).second.getName()].m_map_hChan[(*iterChan).first]->Fill( (*iterChan).second );
                }  //End Loop Over Detector's Channels
            } //End Loop Over Detectors
        } //End Loop Over Superchambers - Trig

        ++iEvt;
    } //End Loop Over Events
    
    return;
} //End AnalyzeTiming::fillHistos()

//Fits histograms found in m_mtrxHistos
void AnalyzeTiming::fitHistos(){
    
} //End AnalyzeTiming::fitHistos()

//Initializes all TH1F objects for all detectors in a test stand
//Wipes any existing histograms
void AnalyzeTiming::initHistos(Timing::DetectorMatrix detMatrix){
//void AnalyzeTiming::initHistos(Timing::TestStandVME testStand){

    //cout<<"=======AnalyzeTiming::initHistos()=======\n";
      
    m_mtrxHistos.m_map_HistosSC_All.clear();

    //Determine the largest full scale range
    float fVME_LSB = -1;    //Least Sensitive Bit
    int iBitADC = 0;        //Bits in the ADC
    for (auto iterVMEBoard = m_daqSetup.m_map_vmeBoards.begin(); iterVMEBoard != m_daqSetup.m_map_vmeBoards.end(); ++iterVMEBoard) { //Loop Over Boards
        
        //Store the *worst* least sentive bit from all boards
        if ( (*iterVMEBoard).second.getLSB() >  fVME_LSB) { //Case: LSB is larger
            fVME_LSB = (*iterVMEBoard).second.getLSB();
            iBitADC = (*iterVMEBoard).second.m_iBitADC;
        } //End Case: LSB is larger
    } //End Loop Over Boards
    
    //Create HistoSetup Container
    HistoSetup hSetup;
    
    hSetup.fHisto_BinWidth = fVME_LSB;
    hSetup.fHisto_xLower = 0;
    hSetup.fHisto_xUpper = fVME_LSB * pow(2, iBitADC);
    hSetup.iHisto_nBins = pow(2, iBitADC);
    hSetup.strHisto_Title_X = "Time #left(ns#right)";
    hSetup.strHisto_Title_Y = "N #left(" + getString(std::ceil( fVME_LSB*1.e3) / 1.e3 ) + " ns#right)"; //Should round to thousandths place
    
    //Initialize Histograms
    for (auto iterSC = detMatrix.begin(); iterSC != detMatrix.end(); ++iterSC) { //Loop Over Superchambers
        //Declare a new HistoSC Container & create TH1F objects for this superchamber
        HistoSC superChamberHistos;
        superChamberHistos.m_strName = (*iterSC).second.getName();

        hSetup.strHisto_Name = (*iterSC).second.getName(); //Set this superchambers name
        superChamberHistos.m_hAll = make_shared<TH1F>( getHistogram(-1, "",  hSetup) );
        //superChamberHistos.m_hAND = make_shared<TH1F>( getHistogram(-1, "AND",  hSetup) );
        //superChamberHistos.m_hOR = make_shared<TH1F>( getHistogram(-1, "OR",  hSetup) );
        
        //Loop Over all Detectors in this Superchamber
        for (auto iterDet = (*iterSC).second.getDetectorPtrBegin(); iterDet != (*iterSC).second.getDetectorPtrEnd(); ++iterDet) {
		//Get the channel map for this detector
		pair<imap_iter_t, imap_iter_t> pair_chanMapIter = (*iterDet).second.getChannelMapVME2Det();

            //Declare a new HistoDet container & create TH1F objects for this detector
            HistoDet detectorHistos;
            
            hSetup.strHisto_Name = (*iterDet).second.getName(); //Set this superchambers name
            detectorHistos.m_hAll = make_shared<TH1F>( getHistogram(-1, "",  hSetup) );
            
		//cout<<(*iterSC).first.first<<"\t"<<(*iterSC).first.second<<"\t"<<(*iterSC).second.getName()<<"\t"<<(*iterDet).second.getName()<<"\t"<<(*iterDet).second.getNChan()<<endl;

            //Make Histograms for each channel
            for (auto iterChan = pair_chanMapIter.first; iterChan != pair_chanMapIter.second; ++iterChan) { //Loop Over Detector's Channels
                detectorHistos.m_map_hChan[(*iterChan).second] = make_shared<TH1F>( getHistogram((*iterChan).second, "",  hSetup) );

		//cout<<(*iterSC).first.first<<"\t"<<(*iterSC).first.second<<"\t"<<(*iterSC).second.getName()<<"\t"<<(*iterDet).second.getName()<<"\t"<<(*iterChan).first<<endl;
            } //End Loop Over Detector's Channels
            
            superChamberHistos.m_map_HistosDet[(*iterDet).second.getName()]=detectorHistos;
        } //End Loop Over Detector
        
        m_mtrxHistos.m_map_HistosSC_All[(*iterSC).first]=superChamberHistos;
    } //End Loop Over Superchambers

    return;
} //End AnalyzeTiming::initHistos()

//Stores booked histograms (for those histograms that are non-null)
//Takes a std::string which stores the physical filename as input
void AnalyzeTiming::storeHistos( string & strOutputROOTFileName, std::string strOption){
    //Variable Declaration
    TFile * ptr_fileOutput = new TFile(strOutputROOTFileName.c_str(), strOption.c_str(),"",1);
    
    //Check if File Failed to Open Correctly
    if ( !ptr_fileOutput->IsOpen() || ptr_fileOutput->IsZombie()  ) {
        printClassMethodMsg("AnalyzeTiming","storeHistos","Error: File I/O");
        printROOTFileStatus(ptr_fileOutput);
        printClassMethodMsg("AnalyzeTiming","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeTiming","storeHistos", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Call the store histos sequence
    storeHistos(ptr_fileOutput);
    
    //Close the ROOT file
    ptr_fileOutput->Close();
    
    return;
} //End storeHistos()

//Stores booked histograms (for those histograms that are non-null)
//Takes a TFile * which the histograms are written to as input
void AnalyzeTiming::storeHistos(TFile * file_InputRootFile){
    
    //Check if File Failed to Open Correctly
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie()  ) {
        printClassMethodMsg("AnalyzeTiming","storeHistos","Error: File I/O");
        printROOTFileStatus(file_InputRootFile);
        printClassMethodMsg("AnalyzeTiming","storeHistos", "\tPlease cross check input file name, option, and the execution directory\n" );
        printClassMethodMsg("AnalyzeTiming","storeHistos", "\tExiting; No Histograms have been stored!\n" );
        
        return;
    } //End Check if File Failed to Open Correctly
    
    //Get Base Directories
    //-------------------------------------
    //Check to see if dir_Summary directory exists already, if not create it
    TDirectory *dir_Summary = file_InputRootFile->GetDirectory("Summary", false, "GetDirectory" );
    if (dir_Summary == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_Summary = file_InputRootFile->mkdir("Summary");
    } //End Case: Directory did not exist in file, CREATE
    
    //Check to see if dir_TimeAbs directory exists already, if not create it
    TDirectory *dir_TimeAbs = file_InputRootFile->GetDirectory("TimeAbsolute", false, "GetDirectory" );
    if (dir_TimeAbs == nullptr) { //Case: Directory did not exist in file, CREATE
        dir_TimeAbs = file_InputRootFile->mkdir("TimeAbsolute");
    } //End Case: Directory did not exist in file, CREATE
    
    //Loop Over the Histogram Matrix and store All histos
    //-------------------------------------
    //Level - Superchamber
    for (auto iterSC = m_mtrxHistos.m_map_HistosSC_All.begin(); iterSC != m_mtrxHistos.m_map_HistosSC_All.end(); ++iterSC) { //Loop Over Superchamber Histos
        //Get Directory
        //-------------------------------------
        //Check to see if dir_Superchamber directory exists already, if not create it
        TDirectory *dir_Superchamber = dir_TimeAbs->GetDirectory( ( (*iterSC).second.m_strName ).c_str(), false, "GetDirectory" );
        if (dir_Superchamber == nullptr) { //Case: Directory did not exist in file, CREATE
            dir_Superchamber = dir_TimeAbs->mkdir(  ( (*iterSC).second.m_strName ).c_str() );
        } //End Case: Directory did not exist in file, CREATE
        
        //Store Histograms - Superchamber Level
        //-------------------------------------
        dir_Superchamber->cd();
        (*iterSC).second.m_hAll->Write();
        
        cout<<"(*iterSC).second.m_hAll->GetName() = " << (*iterSC).second.m_hAll->GetName() << endl;
        cout<<"(*iterSC).second.m_hAll->GetEntries() = " << (*iterSC).second.m_hAll->GetEntries() << endl;
        
        //Level - Detector
        map<string, TDirectory *> map_detDir;
        for (auto iterDet = (*iterSC).second.m_map_HistosDet.begin(); iterDet != (*iterSC).second.m_map_HistosDet.end(); ++iterDet) { //Loop Over this Superchambers' Detector Histos
            //Get Directory
            //-------------------------------------
            //Check to see if dir_Detector directory exists already, if not create it
            TDirectory *dir_Detector = dir_Superchamber->GetDirectory( ( (*iterDet).first ).c_str(), false, "GetDirectory" );
            if (dir_Detector == nullptr) { //Case: Directory did not exist in file, CREATE
                dir_Detector = dir_Superchamber->mkdir(  ( (*iterDet).first ).c_str() );
            } //End Case: Directory did not exist in file, CREATE
            map_detDir[(*iterDet).first]=dir_Detector;
            
            //Store Histograms - Detector Level
            //-------------------------------------
            map_detDir[(*iterDet).first]->cd();
            (*iterDet).second.m_hAll->Write();
            
            //Level - Channel
            for (auto iterChan = (*iterDet).second.m_map_hChan.begin(); iterChan != (*iterDet).second.m_map_hChan.end(); ++iterChan) { //Loop Over this Detector's Channel Histos
                //Get Directory
                //-------------------------------------
                //Check to see if dir_Chan directory exists already, if not create it
                TDirectory *dir_Chan = map_detDir[(*iterDet).first]->GetDirectory( ( "Channel" + getString( (*iterChan).first ) ).c_str(), false, "GetDirectory" );
                if (dir_Chan == nullptr) { //Case: Directory did not exist in file, CREATE
                    dir_Chan = map_detDir[(*iterDet).first]->mkdir( ( "Channel" + getString( (*iterChan).first ) ).c_str() );
                } //End Case: Directory did not exist in file, CREATE
                
                //Store Histograms - Detector Level
                //-------------------------------------
                dir_Chan->cd();
                (*iterChan).second->Write();
            } //End Loop Over this Detector's Channel Histos
            
            map_detDir.clear();
        } //End Loop Over this Superchambers' Detector Histos
    } //End Loop Over Superchamber Histos
    
    //Do not close file_InputRootFile it is used elsewhere
    
    return;
} //End AnalyzeTiming::storeHistos()

//Resets attributes of this class
void AnalyzeTiming::reset(){
    m_mtrxHistos.clear();
}

TH1F AnalyzeTiming::getHistogram(int iChan, std::string strLogic, Timing::HistoSetup setupHisto){
    //Variable Declaration
    string strPrefix = "h";
    string strName = getNameByChanNum(iChan, strPrefix, setupHisto.strHisto_Name);
    
    strName+=strLogic;
    
    //Histo Declaration
    TH1F ret_Histo(strName.c_str(), "", setupHisto.iHisto_nBins, setupHisto.fHisto_xLower, setupHisto.fHisto_xUpper );
    
    //Set Histo Data Members
    ret_Histo.SetXTitle( setupHisto.strHisto_Title_X.c_str() );
    ret_Histo.SetYTitle( setupHisto.strHisto_Title_Y.c_str() );
    
    ret_Histo.Sumw2();
    
    //Set Directory to the global directory
    ret_Histo.SetDirectory(gROOT);
    
    //Return Histogram
    return ret_Histo;
} //End AnalyzeTiming::getHistogram()

std::string AnalyzeTiming::getNameByChanNum(int iChan, std::string strPrefix, std::string strName){
    //Variable Declaration
    string ret_Name = "";
    
    if (iChan > -1) { //Case: Specific Channel
        ret_Name = strPrefix + strName + "_Ch" + getString(iChan);
    } //End Case: Specific Channel
    else{ //Case: All Channels
        ret_Name = strPrefix + strName + "_ChAll";
    } //End Case: All Channels
    
    return ret_Name;
} //End AnalyzeTiming::getNameByChanNum()
