//
//  TestStandVME.cpp
//  
//
//  Created by Brian L Dorney on 26/10/16.
//
//

#include "TestStandVME.h"

using std::cout;
using std::endl;
using std::make_pair;
using std::map;
using std::pair;
using std::shared_ptr;
using std::vector;

using namespace QualityControl::Timing;

//Wipes all stored information
void TestStandVME::reset(){
    m_detMatrix.clear();
} //End TestStandVME::reset

//Get a single event
EventReco TestStandVME::getEventReco(Timing::EventDigi & inputEvtDIGI){
    
    EventReco evtReco(inputEvtDIGI.m_uiEvtCount);
    
    //cout<<"Begin TestStandVME::getEventReco()\n";
    
    //Debugging
    /*for (auto iterVMEBoard = inputEvtDIGI.m_map_TDCData.begin(); iterVMEBoard != inputEvtDIGI.m_map_TDCData.end(); ++iterVMEBoard) {
        for (auto iterChan = (*iterVMEBoard).second.m_map_fTime.begin(); iterChan != (*iterVMEBoard).second.m_map_fTime.end(); ++iterChan) {
            cout<<(*iterVMEBoard).first.c_str()<<"\t"<<(*iterChan).first<<"\t"<<(*iterChan).second<<endl;
        }
    }*/
    
    for (auto iterSuperChamber = m_detMatrix.m_map_detectors.begin(); iterSuperChamber != m_detMatrix.m_map_detectors.end(); ++iterSuperChamber) { //Loop through stand
        for (auto iterDet = (*iterSuperChamber).second.getDetectorPtrBegin(); iterDet != (*iterSuperChamber).second.getDetectorPtrEnd(); ++iterDet) { //Loop throguh detectors in this superchamber
            
            //Debugging
            //cout<<"DETECTOR = " << (*iterDet).second->getName() << endl;
            //cout<<"inputEvtDIGI.m_map_TDCData.count(" << (*iterDet).second->getBaseAddress() <<") = " << inputEvtDIGI.m_map_TDCData.count( (*iterDet).second->getBaseAddress() ) << endl;
            
            if (inputEvtDIGI.m_map_TDCData.count( (*iterDet).second.getBaseAddress() ) > 0 ) {
                
                pair<imap_iter_t, imap_iter_t> pair_iterChMap = (*iterDet).second.getChannelMapVME2Det();
                
                //Debugging
                //cout<<"pair_iterChMap.first.first = " << (pair_iterChMap.first)->first << endl;
                //cout<<"pair_iterChMap.first.second = " << (pair_iterChMap.first)->second << endl;
                //cout<<"pair_iterChMap.second.first = " << (pair_iterChMap.second)->first << endl;
                //cout<<"pair_iterChMap.second.second = " << (pair_iterChMap.second)->second << endl;
                
                //Debugging
                //cout<<"CHANNEL LIST\n";
                
                for (auto iterChMap = pair_iterChMap.first; iterChMap != pair_iterChMap.second; ++iterChMap) { //Loop over mapped channels for this detector
                    
                    //Debugging
                    //cout<<(*iterDet).second->getBaseAddress()<<"\t";
                    //cout<<(*iterChMap).first<<"\t";
                    //cout<<(inputEvtDIGI.m_map_TDCData[(*iterDet).second->getBaseAddress()]).m_map_fTime[(*iterChMap).first]<<endl;
                    
                    (*iterDet).second.setChanData( (*iterChMap).first, (inputEvtDIGI.m_map_TDCData[(*iterDet).second.getBaseAddress()]).m_map_fTime[(*iterChMap).first] );
                } //End Loop over mapped channels for this detector
            } //End Case: This event has this detector's base address
            
            //cout<<"Detector has data = " << (*iterDet).second->hasData() << endl;
        } //End Loop throguh detectors in this superchamber
        
        //cout<<"Superchamber has data = " << (*iterSuperChamber).second->hasData() << endl;
        
        //Store in event
        if ( (*iterSuperChamber).second.getType() == kDUT && (*iterSuperChamber).second.hasData() ) { //Case: Superchamer is DUT
            evtReco.m_detMatrix_DUTs.m_map_detectors[(*iterSuperChamber).first]=(*iterSuperChamber).second;
            (*iterSuperChamber).second.resetChannels();
        } //End Case: Superchamer is DUT
        else if ( (*iterSuperChamber).second.getType() == kTrigger && (*iterSuperChamber).second.hasData() ) { //Case: Superchamer is Trigger
            evtReco.m_detMatrix_Trigger.m_map_detectors[(*iterSuperChamber).first]=(*iterSuperChamber).second;
            (*iterSuperChamber).second.resetChannels();
        } //End Case: Superchamer is Trigger
    } //End Loop through stand
    
    //cout<<"evtReco.m_detMatrix_DUTs.m_map_detectors.size() = " << evtReco.m_detMatrix_DUTs.m_map_detectors.size() << endl;
    //cout<<"evtReco.m_detMatrix_Trigger.m_map_detectors.size() = " << evtReco.m_detMatrix_Trigger.m_map_detectors.size() << endl;
    
    //cout<<"End TestStandVME::getEventReco()\n";
    
    return evtReco;
} //End TestStandVME::getEventReco()

//Inserst a detector into the stand
//void TestStandVME::setDetector(int iRow, int iCol, std::shared_ptr<Timing::DetectorTiming> inputDet, Timing::DetType detType){
void TestStandVME::setDetector(int iRow, int iCol, Timing::DetectorTiming inputDet, Timing::DetType detType){
    pair<int,int> pair_iPos = std::make_pair(iRow, iCol);
    
    //Check if the position exists
    if (m_detMatrix.m_map_detectors.count(pair_iPos) > 0 ) { //Case: Position Exists, Add to it!
        int iNDet = m_detMatrix.m_map_detectors[pair_iPos].getNDetectors();
        
        /*try {
            if (detType != m_detMatrix.m_map_detectors[pair_iPos]->getType() ) {
                throw 1;
            }
        } catch (int iErrorCode) {
            
        }*/
        
        //Check to make sure they type is the same
        if (detType == m_detMatrix.m_map_detectors[pair_iPos].getType() ) {
            m_detMatrix.m_map_detectors[pair_iPos].setDetector(iNDet, inputDet);
        }
        else{
            cout<<"TestStandVME::setDetector() - Type Mismatch\n";
            cout<<"\tInput Type = " << detType << endl;
            cout<<"\tDetectorSuperchamber at (" << iRow << "," << iCol << ") has type " << m_detMatrix.m_map_detectors[pair_iPos].getType() << endl;
            cout<<"\tInput Detector is not set\n";
        }
    } //End Case: Position Exists, Add to it!
    else{ //Case: Position does not exist, CREATE IT!
        //shared_ptr<DetectorSuperchamber> detectorSC(new DetectorSuperchamber);
        DetectorSuperchamber detectorSC;
        
        detectorSC.setType(detType);
        detectorSC.setDetector(0, inputDet);
        
        m_detMatrix.m_map_detectors[pair_iPos]=detectorSC;
    } //End Case: Position does not exist, CREATE IT!
    
    return;
} //End TestStandVME::setDetector

//Inserst a superchamber into the stand
//Erases any superchamber previously at that position
//void TestStandVME::setSuperchamber(int iRow, int iCol, std::shared_ptr<Timing::DetectorSuperchamber> inputSuperchamber){
void TestStandVME::setSuperchamber(int iRow, int iCol, Timing::DetectorSuperchamber inputSuperchamber){
    pair<int,int> pair_iPos = std::make_pair(iRow, iCol);
    
    m_detMatrix.m_map_detectors[pair_iPos]=inputSuperchamber;
    
    return;
} //End TestStandVME::setDetector