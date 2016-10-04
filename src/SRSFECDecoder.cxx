#include <vector>
#include <iostream>
#include "SRSFECDecoder.h"
#include "SRSMapping.h"
#include "SRSAPVEvent.h"
#include "SRSEventBuilder.h"
SRSFECDecoder::SRSFECDecoder(SRSEventBuilder* eventBuilder): feventBuilder(eventBuilder){
  SRSMapping * mapping = SRSMapping::GetInstance();
  std::map <int, int> apvNoFromApvIDMap = mapping->GetAPVNoFromIDMap();
  fActiveFecChannelsMap.clear() ;
  std::map <int, int>::const_iterator adcChannel_itr ;
  for(adcChannel_itr = apvNoFromApvIDMap.begin(); adcChannel_itr != apvNoFromApvIDMap.end(); ++adcChannel_itr) {
    int apvid = (* adcChannel_itr).first ;
    int activeChannel = apvid & 0xF;
    int fecId = (apvid >> 4 ) & 0xF;
    fActiveFecChannelsMap[fecId].push_back(activeChannel) ;
  }

}

void
SRSFECDecoder::decodeFEC(unsigned int nw, unsigned long* buffer){
  //  std::cout <<" START DECODING"<<std::endl;
  int ll=0;
  int ln=0;
  std::vector<unsigned long int> data32BitsVector;
  unsigned int current_offset = 0;
  data32BitsVector.clear();
  current_offset += 2;
  unsigned int eqHeaderWord = buffer[current_offset];
  unsigned int fecID = eqHeaderWord & 0xff;
  unsigned int apvID=0;
  unsigned int adcChannel =0;
  current_offset +=5;
  bool kFalse = false;
  bool fIsNewPacket=false;
  fPacketSize = 0;
  unsigned int currentAPVPacketHdr = 0;
  while(current_offset<nw){
    unsigned long rawdata=buffer[current_offset];
    if (rawdata == 0xfafafafa) {
      //===================================================================================================//                                               
      // last word of the previous packet added for Filippo in DATE to count the eventNb x 16 UDP packets  //                                               
      // We dont need it here, will just skip it We remove it from the vector of data                      //                                               
      //===================================================================================================//                                               
      if(!data32BitsVector.empty()) {
        apvID = (fecID << 4) | adcChannel ;
        //      BuildHits(data32BitsVector, fecID, adcChannel, ped, eventBuilder, zeroSupCut);
	this->BuildHits(data32BitsVector, fecID, adcChannel,feventBuilder);
        unsigned int  datasize = data32BitsVector.size() ;
      }

      adcChannel = 0 ;
      data32BitsVector.clear() ;
      current_offset++ ;
      break ;
    }
    //==========================================================================================//                                                          
    // Word with the event number (trigger count) and the packet size information               //                                                          
    //                                     size of APV packet                                    //                                                         
    //==========================================================================================//                                                          
    if (fIsNewPacket) {
      fPacketSize = (rawdata & 0xffff) ;
      data32BitsVector.clear() ;
      fIsNewPacket = false;
      current_offset++ ;
      continue ;
    }

    //=========================================================================================================//                                           
    //         New packet (or frame) FEC channel data in the equipment                                         //                                           
    //=========================================================================================================//                                           

    if (((rawdata >> 8) & 0xffffff) == 0x41505a) {
      data32BitsVector.pop_back() ;
      if(!data32BitsVector.empty()) {
        apvID = (fecID << 4) | adcChannel ;
        //      BuildHits(data32BitsVector, fecID, adcChannel, ped, eventBuilder, zeroSupCut) ;                                                             
	this->BuildHits(data32BitsVector, fecID, adcChannel,feventBuilder);
        unsigned int  datasize = data32BitsVector.size() ;
      }

      unsigned int currentAPVPacketHdr = rawdata  ;
      adcChannel = currentAPVPacketHdr & 0xff ;
      //=== REINITIALISE EVERYTHING                                                                                                                         
      if(adcChannel > 15) {
        printf("  SRSFECEventDecoder => ERROR #### fecID=%d, ADC Channel=%d, apvID=%d, \n",fecID, adcChannel, apvID) ;
        break ;
      }
      
      data32BitsVector.clear() ;
      fIsNewPacket = true;      
      current_offset++ ;
      continue ;
    }
    //=========================================================================================================//                                           
    //         apv data in the packet (frame)                                                                 //                                            
    //========================================================================================================//                                            
    if (!fIsNewPacket) {
      data32BitsVector.push_back(rawdata) ;
      current_offset++ ;
      continue ;
    }
  }
  //  std::cout <<" END OF EVENT"<<std::endl;
}


void 
SRSFECDecoder::BuildHits(std::vector<unsigned long int> data32bits, int fec_id, int adc_channel, SRSEventBuilder* eventBuilder){
  SRSMapping * mapping = SRSMapping::GetInstance();
  fActiveFecChannels.clear();
  //  if (fdebug) 
  //  std::cout <<" +++ SRSFECDecoder: BuildHIts FECid="<<fec_id<<std::endl;
  fActiveFecChannels = fActiveFecChannelsMap[fec_id] ;

  int apvID = (fec_id << 4) | adc_channel ;
  if (find(fActiveFecChannels.begin(), fActiveFecChannels.end(), adc_channel) != fActiveFecChannels.end() ) {
    int zeroSupCut = -999;
    SRSAPVEvent * apvEvent = new SRSAPVEvent(fec_id, adc_channel, apvID, zeroSupCut, fPacketSize) ;
    apvEvent->SetHitMaxOrTotalADCs(eventBuilder->GetHitMaxOrTotalADCs()) ;

    std::vector<unsigned long int >::const_iterator data_itr ;
    //    if (fdebug)
    // std::cout <<" +++ SRSFECDecoder: data32 bits size "<<data32bits.size()<<std::endl;
    for(data_itr = data32bits.begin(); data_itr != data32bits.end(); ++data_itr) {
      unsigned int data = (* data_itr) ;
      // if (fdebug)
	//	std::cout <<" +++ SRSFECDecoder: data 0x"<<std::hex<<data<<std::dec<<std::endl;
      apvEvent->Add32BitsRawData(data) ;
    }
    
    apvEvent->SetAllFlags(true, true) ;
    //    if (zeroSupCut!=-999) apvEvent->SetPedestals(ped->GetAPVNoises(apvEvent->GetAPVID()), ped->GetAPVOffsets(apvEvent->GetAPVID()), ped->GetAPVMaskedChannels(apvEvent->GetAPVID())) ;

    //    list <SRSHit*> listOfHits = apvEvent->ComputeListOfAPVHits(eventBuilder->GetHitMaxOrTotalADCs()) ;                                                                                           
    std::list<SRSHit*> listOfHits = apvEvent->ComputeListOfAPVHitsZS() ;
    std::list <SRSHit*>::const_iterator  hit_itr;
    //    std::cout <<"Number of hit found "<<listOfHits.size()<<std::endl;
    //    if (fdebug)
    // std::cout << " +++ SRSFECDecoder: Number of Hits "<<listOfHits.size()<<std::endl;
    for (hit_itr = listOfHits.begin(); hit_itr != listOfHits.end(); ++hit_itr) {
      SRSHit * hit = * hit_itr ;
      // if(fdebug)  
      //	std::cout <<" +++ SRSFECDecoder: HIT plane "<<hit->GetPlane()<<" ADC "<<hit->GetHitADCs()<<" strip "<<hit->GetStripNo()<<" bin peak "<<hit->GetSignalPeakBinNumber()<<std::endl;
      eventBuilder->AddHitInDetectorPlane(hit)  ;
      eventBuilder->AddHit(hit)  ;
    }

    //    eventBuilder->AddMeanDetectorPlaneNoise(apvEvent->GetPlane(), apvEvent->GetMeanAPVnoise()) ;

    listOfHits.clear() ;
    delete apvEvent ;
  }
}

