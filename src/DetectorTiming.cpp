//
//  DetectorTiming.cpp
//  
//
//  Created by Brian L Dorney on 26/10/16.
//
//

#include "DetectorTiming.h"


using std::cout;
using std::endl;
using std::map;
using std::make_pair;
using std::pair;
using std::string;

using namespace QualityControl::Timing;

//Default Constructor
DetectorTiming::DetectorTiming(){
    m_bHasData = false;
    m_iNumNonZeroChan = 0;
    m_strName = "";
    m_strBaseAddress = "";
} //End Default Constructor

//Get const iterators for the detector's channel map
//pair::first is m_map_VMEChan2DetChan.cbegin()
//pair::second is m_map_VMEChan2DetChan.cend()
pair<imap_iter_t, imap_iter_t> DetectorTiming::getChannelMapVME2Det(){
    return make_pair(m_map_VMEChan2DetChan.cbegin(), m_map_VMEChan2DetChan.cend() );
} //End DetectorTiming::getChannelMapVME2Det()

void DetectorTiming::printChannelData(){
    
    cout<<"Channel Data for: " <<m_strName<<endl;
    cout<<"Ch\tVal\n";
    cout<<"---------------\n";
    for (auto iterChan = m_map_DetChanData.begin(); iterChan != m_map_DetChanData.end(); ++iterChan) {
        cout<<(*iterChan).first<<"\t"<<(*iterChan).second<<endl;
    } //End Loop Over Channels
    cout<<"---------------\n";
    
    return;
} //End DetectorTiming::printChannelData()

void DetectorTiming::setChanData(int iVMEChan, double dData){
    
    if (m_map_VMEChan2DetChan.count(iVMEChan) > 0 ) { //Case: Mapping for this VME Channel exists in detector
        m_map_DetChanData[m_map_VMEChan2DetChan[iVMEChan]]=dData;
        
        if (dData > 0.) {
            //Debugging
            //cout<<m_strName<<" iVMEChan = " << iVMEChan << " iDetChan = " << m_map_VMEChan2DetChan[iVMEChan] << " data = " << dData << endl;
            
            m_bHasData = true;
            m_iNumNonZeroChan++;
        }
        
        //cout<<"DetectorTiming::setChanData() - bHasData = " << bHasData << endl;
    } //End Case: Mapping for this VME Channel exists in detector
    else{ //Case: Mapping for this VME Channel Does NOT exist
        cout<<"DetectorTiming::setChanData() - VME Channel " << iVMEChan << " not present in detector " << m_strName.c_str() << "channel map" << endl;
    } //End Case: Mapping for this VME Channel Does NOT exist
    
    return;
} //End DetectorTiming::setChanData()

//Wipe all stored objects
void DetectorTiming::reset(){
    m_bHasData = false;
    
    m_iNumNonZeroChan = 0;
    
    m_strName.clear();
    m_strBaseAddress.clear();
    
    m_map_VMEChan2DetChan.clear();
    m_map_DetChanData.clear();
} //End DetectorTiming::reset()

//Reset Channel Data
void DetectorTiming::resetChannels(){
    m_bHasData = false;
    m_iNumNonZeroChan = 0;
    m_map_DetChanData.clear();
}