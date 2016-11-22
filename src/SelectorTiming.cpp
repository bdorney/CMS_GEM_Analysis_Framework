//
//  SelectorTiming.cpp
//  
//
//  Created by Brian L Dorney on 18/11/16.
//
//

#include "SelectorTiming.h"

using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::stoi;
using std::string;
using std::vector;

//using QualityControl::Timing::getString;

using QualityControl::Uniformity::getEventRange;
using QualityControl::Uniformity::printClassMethodMsg;

using namespace QualityControl::Timing;

//Default Constructor
SelectorTiming::SelectorTiming(Timing::RunSetup & inputSetup){
    m_daqSetup = inputSetup;
} //End SelectorTiming::SelectorTiming()

//Filters an input vector<Timing::EventReco> object based on the stored Uniformity::AnalysisSetupUniformity attribute
vector<EventReco> SelectorTiming::filterEvents(std::vector<EventReco> vec_inputEvts){
    //Variable Declaration
    vector<EventReco> vec_retEvts;
    
    //Could do this with vector::erase but believe that would be more inefficient (since container is re-organized everytime)
    for (auto iterEvt = vec_inputEvts.begin(); iterEvt != vec_inputEvts.end(); ++iterEvt) { //Loop Over input Events
        if ( eventPassesSelection( (*iterEvt) ) ) { vec_retEvts.push_back( *iterEvt ); }
    } //End Loop Over input Events
    
    return vec_retEvts;
} //End SelectorTiming::filterEvents()

//Cut on # of Trigger detectors in each row (i)
//Require at least one DUT
bool SelectorTiming::eventPassesSelection(Timing::EventReco &inputEvt){
    map<int, int> map_iNTrigInLayer;
    //map<int, double> map_dTrigTimeOfLayer;
    
    //Require at least one DUT
    if( !(inputEvt.m_detMatrix_DUTs.m_map_detectors.size() >= aSetupTiming.selTime.m_iCut_NDUT ) ) return false;
    //cout<<"Passed N_DUT Requirement\n";
    
    //Check to see how many Trigger Detectors are on in each row (i)
    for (auto iterTrigMatrix = inputEvt.m_detMatrix_Trigger.begin(); iterTrigMatrix != inputEvt.m_detMatrix_Trigger.end(); ++iterTrigMatrix) { //Loop Through Trigger Matrix
        
        //map_iNTrigInLayer[(*iterTrigMatrix).first.first]++;
        
        if (map_iNTrigInLayer.count( (*iterTrigMatrix).first.first ) > 0 ) {
            map_iNTrigInLayer[(*iterTrigMatrix).first.first]++;
        } //End Case: Row i is present
        else{ //Case: Row i not yet present, add it
            map_iNTrigInLayer[(*iterTrigMatrix).first.first] = 1;
        } //End Case: Row i not yet present, add it
    } //End Loop Through Trigger Matrix

    //Cut on the number of Trigger detectors in each row (i)
    if ( !( map_iNTrigInLayer.size()  > 1 ) ) { //Case: Not Enough Trigger Detectors
        return false;
    } //End Case: Not Enough Trigger Detectors
    else{ //Case: At least two trigger detectors
        //cout<<"--------------\n";
        //cout<<"Min\tVal\tMax\n";
        for (auto iterTrigMatrix = map_iNTrigInLayer.begin(); iterTrigMatrix != map_iNTrigInLayer.end(); ++iterTrigMatrix) { //Loop Over map_iNTrigInLayer
            
            //cout<<aSetupTiming.selTime.m_iCut_NTrig_Min<<"\t";
            //cout<< (*iterTrigMatrix).second << "\t";
            //cout<< aSetupTiming.selTime.m_iCut_NTrig_Max << endl;
            
            if (  (*iterTrigMatrix).second < aSetupTiming.selTime.m_iCut_NTrig_Min || (*iterTrigMatrix).second > aSetupTiming.selTime.m_iCut_NTrig_Max ) {   //This cuts about 10-11% of events for cut equal to 1
                return false;
            }
        } //End Loop Over map_iNTrigInLayer
        //cout<<"--------------\n";
    } //End Case: At least two trigger detectors
    //cout<<"Passed N_Trig Requirement\n";
    
    //Accept the event if we reached here.
    return true;
} //End SelectorTiming::eventPassesSelection()

//Given an output ROOT file from VME System with events
//Returns a vector<Timing::EventReco> object of those EventReco objects passing event selection
//Input is a std::string storing the physical filename
std::vector<EventReco> SelectorTiming::getEventsReco(std::string &strInputRootFileName, Timing::TestStandVME &testStand){
    //Variable Declaration
    TFile *file_ROOT = NULL;
    
    vector<EventReco> vec_retEvts;
    
    //Open this run's root file
    //------------------------------------------------------
    file_ROOT = new TFile(strInputRootFileName.c_str(),"READ","",1);
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !file_ROOT->IsOpen() || file_ROOT->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Timing::SelectorTiming::getEventsReco() - error while opening file: " + strInputRootFileName ).c_str() );
        Timing::printROOTFileStatus(file_ROOT);
        std::cout << "Exiting!!!\n";
        
        return vec_retEvts;
    } //End Case: failed to load ROOT file
    
    //Calling setClusters
    //------------------------------------------------------
    vec_retEvts = getEventsReco(file_ROOT, testStand);
    
    //Close the Input ROOT File
    //------------------------------------------------------
    file_ROOT->Close();
    
    return vec_retEvts;
} //End SelectorTiming::getEventsReco()

//Given an output ROOT file from VME System with events
//Returns a vector<Timing::EventReco> object of those EventReco objects passing event selection
//Input is a TFile *
std::vector<EventReco> SelectorTiming::getEventsReco(TFile * file_InputRootFile, Timing::TestStandVME & testStand){
    //Variable Declaration
    TTree *tree_eventsDigi = NULL;
    
    vector<EventReco> vec_retEvts;
    
    //Check to see if data file opened successfully, if so load the tree
    //------------------------------------------------------
    if ( !file_InputRootFile->IsOpen() || file_InputRootFile->IsZombie() ) { //Case: failed to load ROOT file
        perror( ("Timing::SelectorTiming::getEventsReco() - error while opening file: " + (string) file_InputRootFile->GetName() ).c_str() );
        Timing::printROOTFileStatus(file_InputRootFile);
        std::cout << "Exiting!!!\n";
        
        return vec_retEvts;
    } //End Case: failed to load ROOT file
    
    tree_eventsDigi = (TTree*) file_InputRootFile->Get("Timing");
    
    if ( nullptr == tree_eventsDigi ) { //Case: failed to load TTree
        printClassMethodMsg("SelectorTiming","getEventsReco",("error while fetching: " + (string) file_InputRootFile->GetName() ).c_str() );
        printClassMethodMsg("SelectorTiming","getEventsReco","\tTree returns nullptr; Exiting!!!");
        
        return vec_retEvts;
    } //End Case: failed to load TTree
    
    //Initialize Tree Branch Address to retrieve the cluster information
    //------------------------------------------------------
    //Initialize an empty container
    map<string, double> map_vmeEvtData;
    
    string strTempBaseAddr;
    string strChanName;
    
    for (auto iterVMEBoard = m_daqSetup.m_map_vmeBoards.begin(); iterVMEBoard != m_daqSetup.m_map_vmeBoards.end(); ++iterVMEBoard) { //Loop Over VME Boards Used in DAQ
        //Format Base Address
        strTempBaseAddr = (*iterVMEBoard).first;
        strTempBaseAddr.erase(6,4); //Remove trailing 0's
        strTempBaseAddr.erase(0,2); //Remove "0x"
        
        for (int iChan=0; iChan<(*iterVMEBoard).second.m_iNumChan; ++iChan) {
            strChanName = ( "TDC" + strTempBaseAddr + "_Ch" + getString(iChan) );
            map_vmeEvtData[strChanName]=-1;
        }
    } //End Loop Over VME Boards Used in DAQ
    
    //cout<<"map_vmeEvtData.size() = " << map_vmeEvtData.size() << endl;
    
    //Create the branch references for the tree
    for (auto iterVMEBoard = map_vmeEvtData.begin(); iterVMEBoard != map_vmeEvtData.end(); ++iterVMEBoard) { //Loop Over Channels
        //cout<< (*iterVMEBoard).first.c_str() <<endl;
        tree_eventsDigi->SetBranchAddress( (*iterVMEBoard).first.c_str(), &(map_vmeEvtData[(*iterVMEBoard).first]) );
    } //End Loop Over Channels
    
    //Determine Event Range
    //------------------------------------------------------
    pair<int,int> pair_iEvtRange;
    
    pair_iEvtRange = getEventRange( aSetupTiming.iEvt_First, aSetupTiming.iEvt_Total, tree_eventsDigi->GetEntries() );
    
    //Get data event-by-event
    //------------------------------------------------------
    EventDigi evtDigi; //Event - Digi Level
    EventReco evtReco;
    
    string strTempChNum;
    
    for (int iEvt=pair_iEvtRange.first; iEvt < pair_iEvtRange.second; ++iEvt) {
        //Get this event's data
        tree_eventsDigi->GetEntry(iEvt);
        
        //Output to the user some message that we are still running
        if (iEvt % 1000 == 0) cout<<"Event Selection; " <<iEvt<<" Events Processed\n";
        //cout<<"--------------------New Event--------------------\n";
        
        //---------------Load Digi Level Event---------------
        evtDigi.m_uiEvtCount = iEvt;
        for (auto iterVMEBoard = map_vmeEvtData.begin(); iterVMEBoard != map_vmeEvtData.end(); ++iterVMEBoard) {
            //Get the base address without "TDC" or "_ChX"
            strTempBaseAddr = (*iterVMEBoard).first;
            strTempBaseAddr.erase(0,3);
            
            strTempChNum = strTempBaseAddr.substr(strTempBaseAddr.find("_"), std::string::npos);
            strTempBaseAddr.erase(strTempBaseAddr.find("_"), std::string::npos);
            
            //Debugging
            //cout<<"strTempBaseAddr = " << strTempBaseAddr << endl;
            
            //Set the digi parameters for this board
            
            if ( ((*iterVMEBoard).second) > 0. ) { //Check: Non-zero channel data!
                
                //int iChan = stoi(strTempChNum, nullptr, 10);

                //Strip non-numeric characters
                for (auto iterStr = strTempChNum.begin(); iterStr != strTempChNum.end(); ) {
                    if ( !std::isdigit(*iterStr) ) { iterStr = strTempChNum.erase(iterStr); }
                    else{ ++iterStr; }
                }
                
                //cout<<"strTempChNum = " << strTempChNum.c_str() << endl;
                int iChan = stoi(strTempChNum);
                //cout<<"iChan = " << iChan << endl;
                
                if (evtDigi.m_map_TDCData.count(strTempBaseAddr) > 0 ) { //Case: Board Exists, add to it!
                    evtDigi.m_map_TDCData[strTempBaseAddr].m_map_fTime[iChan] = (*iterVMEBoard).second;
                    
                    //Debugging
                    //cout<<evtDigi.m_map_TDCData[strTempBaseAddr].m_uiEvtCount<<"\t";
                    //cout<<evtDigi.m_map_TDCData[strTempBaseAddr].m_strBaseAddress<<"\t";
                    //cout<<iChan<<"\t";
                    //cout<<evtDigi.m_map_TDCData[strTempBaseAddr].m_map_fTime[iChan]<<endl;
                } //End Case: Board Exists, add to it!
                else{ //Case: This Board Does Not Exist in Event Yet; CREATE IT!
                    TDCDataDigi digi;
                    digi.m_uiEvtCount = iEvt;
                    digi.m_strBaseAddress = strTempBaseAddr;
                    digi.m_map_fTime[iChan] = (*iterVMEBoard).second;
                    
                    //Debugging
                    //cout<<digi.m_uiEvtCount<<"\t";
                    //cout<<digi.m_strBaseAddress<<"\t";
                    //cout<<iChan<<"\t";
                    //cout<<digi.m_map_fTime[iChan]<<endl;
                    
                    evtDigi.m_map_TDCData[strTempBaseAddr]=digi;
                } //End Case: This Board Does Not Exist in Event Yet; CREATE IT!
            } //End Check: Non-zero channel data!
        } //End Loop Over Boards
        
        //---------------Event Reconstruction---------------
        //Check to see if event has data
        if( !(evtDigi.m_map_TDCData.size() > 0) ){
            evtDigi.clear();
            continue;
        }
        
        //Reco evt
        evtReco = testStand.getEventReco(evtDigi);
        
        //Wipe the digi level event
        evtDigi.clear();
        
        //---------------Event Selection---------------
        if ( !eventPassesSelection(evtReco) ) continue;
        
        //cout<<"Event Accepted!\n";
        
        vec_retEvts.push_back(evtReco);
    } //End Loop Over "Events"
    
    //Do not close the input TFile, it is used elsewhere
    
    return vec_retEvts;
} //End SelectorTiming::getEventsReco()

int SelectorTiming::getTransformedChanNum(int iChInputNum, int iMaxChanNum){
    
    if (iChInputNum > iMaxChanNum) {
        return getTransformedChanNum(iChInputNum - iMaxChanNum, iMaxChanNum);
    }
    else{
        return iChInputNum;
    }
} //End SelectorTiming::getTransformedChanNum()
