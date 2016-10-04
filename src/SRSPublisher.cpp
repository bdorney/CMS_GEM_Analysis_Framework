#include "SRSPublisher.h"


SRSPublisher::SRSPublisher() {
  fEvent         = 0;
  fEquipmentSize = 0;
  fStartEventNumber = "0" ;
  fEventFrequencyNumber = "1" ;
  fMinNbOfEquipments = 1 ;
  fMaxNbOfEquipments = 8 ;
  fEventType     = 0;
  fEventRunNb    = -1;
  fRunType       = "RAWDATA";
  fApvNoFromApvIDMap.clear() ;
  fApvNameFromIDMap.clear() ;
  fApvGainFromApvNoMap.clear() ;
  fMapping = SRSMapping::GetInstance() ;
  sigmaCut = 0 ;
}
            

SRSPublisher::~SRSPublisher() {              
  if (fRunType == "ROOTFILE") {
    //    fROOT->WriteRootFile() ;
    // printf("SRSPublisher::EndOfRun()==> Leaving ========= \n");
  }
}


void SRSPublisher::Init(SRSConfiguration* conf) {
  printf("=========================================== SRSPublisher::ParseEvent()==> LOAD CONFIG FILE  \n") ;
                
  fSRSConf = conf;
  //=== Initialize
  fStartEventNumber        = fSRSConf->GetStartEventNumber();
  fEventFrequencyNumber    = fSRSConf->GetEventFrequencyNumber();
                
  if(this->atoi(fStartEventNumber) < 1 ) {
    printf(" SRSPublisher::ParseEvents()==> before fStartEventNumber = %s \n",fStartEventNumber.c_str()) ;
    fStartEventNumber = "0" ;
    printf(" SRSPublisher::ParseEvents()==> after  fStartEventNumber = %s \n",fStartEventNumber.c_str()) ;
  }
                
  if(this->atoi(fEventFrequencyNumber) < 1 ) {;
    printf(" SRSPublisher::ParseEvents()==> before fEventFrequency = %s \n",fEventFrequencyNumber.c_str()) ;
    fEventFrequencyNumber = "1" ;
    printf(" SRSPublisher::ParseEvents()==> after  fEventFrequency = %s \n",fEventFrequencyNumber.c_str()) ;
  }
  
  if( fSRSConf->GetRunType() == std::string("").c_str()) fRunType = "RAWDATA" ;
  else                              fRunType = fSRSConf->GetRunType() ;
                
  std::string zeroSupCut        = fSRSConf->GetZeroSupCut() ;
  std::string maskedChannelCut  = fSRSConf->GetMaskedChannelCut() ;
  std::string minClusterSize    = fSRSConf->GetMinClusterSize() ;
  std::string maxClusterSize    = fSRSConf->GetMaxClusterSize() ;
  std::string maxClusterMult    = fSRSConf->GetMaxClusterMultiplicity() ;
  std::string isCluserPosCorrec = fSRSConf->GetClusterPositionCorrectionFlag() ;
                
  printf("================================================ SRSPublisher::ParseEvents()==> LOAD CONFIG FILE: run type = %s \n\n",fRunType.c_str()) ;
                
  printf("================================================ SRSPublisher::ParseEvents()==> LOAD MAPPING started \n") ;
                //=== Get an instance of the mapping class object and load the mapping
  fMapping->LoadDefaultMapping(fSRSConf->GetMappingFile());
  fMapping->LoadAPVtoPadMapping(fSRSConf->GetPadMappingFile());
  fMapping->PrintMapping() ;
  //fMapping->SaveMapping(fSRSConf->GetSavedMappingFile()) ;
                
  fApvNoFromApvIDMap = fMapping->GetAPVNoFromIDMap();
  fApvNameFromIDMap  = fMapping->GetAPVFromIDMap();
  fMaxNbOfEquipments = fMapping->GetNbOfFECs() ;
  printf("================================================ SRSPublisher::ParseEvents()==> MAPPING LOADED \n\n") ;
                  
  
  fEvent++ ;
  if (fEvent >  this->atoi(fStartEventNumber) ) {
    if ( (fEvent % this->atoi(fEventFrequencyNumber) == 0 )) {
	
      //SRSEventBuilder * eventBuilder = new SRSEventBuilder(fEvent, fSRSConf->GetMaxClusterSize(), fSRSConf->GetMinClusterSize(), fSRSConf->GetZeroSupCut(), fRunType, fIsClusterPosCorrection) ;
      //eventBuilder->SetHitMaxOrTotalADCs(fSRSConf->GetHitMaxOrTotalADCs());
	//eventBuilder->SetClusterMaxOrTotalADCs(fSRSConf->GetClusterMaxOrTotalADCs());
	//eventBuilder->SetMaxClusterMultiplicity(fSRSConf->GetMaxClusterMultiplicity());
      
      // sto inventando
      for (eventLdcIdType * ldcId = firstLdcId; ldcId < lastLdcId; ldcId++) {
	  
	  const eventHeaderStruct * ldcHeader = parser->GetEventByLDCId(*ldcId);
          
	  if (ldcHeader == 0) {
	    std::cerr << "invalid LDC" << std::endl ;
	    continue;
	  }
          
	  const equipmentIdType * firstEqId = parser->FirstEquipment(*ldcId) ;
	  const equipmentIdType * lastEqId  = parser->LastEquipment(*ldcId) ;
                                
	  for (const equipmentIdType * eqId = firstEqId; eqId < lastEqId; eqId++) {
	    const equipmentHeaderStruct * const equipmentHeader = parser->GetEquipmentById(*eqId) ;
	    if (equipmentHeader) {
	      buffer = (UInt_t*)equipmentHeader ;
	      fEquipmentSize = equipmentHeader->equipmentSize ;
              
	      // At the first event check if Pedestal file has to be uploaded
	      // or thanks to APZ no ped file is required
	      if (fEvent == 1) {
		sigmaCut = LoadPedestalRootFileIfNecessary(buffer);
	      }
                                                
	      //SRSFECEventDecoder * fecDecoder = new SRSFECEventDecoder(fEquipmentSize,buffer,eventBuilder,sigmaCut) ;
	      //	      SRSFECEventDecoder * fecDecoder = new SRSFECEventDecoder(fEquipmentSize,buffer) ;
	      //delete fecDecoder ;
	    }
                                            
	  }
	}
      
      
                                
      // ROOT FILE RUN
	if (fRunType == "ROOTFILE") {
	  //	eventBuilder->ComputeClustersInDetectorPlane() ;
	  //eventBuilder->SetTriggerList(fTrack->GetTriggerList()) ;
	  //fROOT->FillRootFile(eventBuilder, fTrack) ;
	}
	
	// Delete the event decoder and event builder object class
	//      delete eventBuilder ;
    }
  }
}

            


int SRSPublisher::LoadPedestalRootFileIfNecessary(unsigned int * buffer) {
                
  SRSMapping * mapping = SRSMapping::GetInstance();
  std::map <int, int> apvNoFromApvIDMap = mapping->GetAPVNoFromIDMap();
                
  int size = apvNoFromApvIDMap.size() ;
  int sigmaCut, executeonce = 0 ;

  bool fIsNewPacket = false;
  int fPacketSize = 4000 ;
                
  std::vector <int>  fActiveFecChannels ;
  std::map<int, std::vector<int> > fActiveFecChannelsMap ;
                
                
  fActiveFecChannelsMap.clear() ;
  std::map <int, int >::const_iterator adcChannel_itr ;
  for(adcChannel_itr = apvNoFromApvIDMap.begin(); adcChannel_itr != apvNoFromApvIDMap.end(); ++adcChannel_itr) {
    int apvid = (* adcChannel_itr).first ;
    int activeChannel = apvid & 0xF;
    int fecId = (apvid >> 4 ) & 0xF;
    fActiveFecChannelsMap[fecId].push_back(activeChannel) ;
    printf("  Publisher => List of  fecNo=%d, activeChannel = %d\n", fecId, activeChannel) ;
  }
                
  //==========================================================================//
  // Needed as the key to link apvID (or adcChannel) to the apvEvent in the TList  //
  // Should be < to 15 (max 16 APV channel in the FEC)                        //
  //==========================================================================//
                
  int current_offset = 0, fecID = 0 ;
  int adcChannel = 0, apvID = 0;
  unsigned int currentAPVPacketHdr;
  int previousAPVPacketSize = 0 ;
                
  std::vector<unsigned int> data32BitsVector ;
  data32BitsVector.clear() ;
                
  //===============================================================================//
  // Dealing with the 7 Equipment header words. We just skip the first 2 words     //
  // and go straight to the 3rd word  where we extract the FEC no (Equip Id)       //
  //===============================================================================//
  current_offset += 2 ;
  unsigned int_t eqHeaderWord = buffer[current_offset] ;
  fecID = eqHeaderWord & 0xff ;
  fActiveFecChannels.clear();
  fActiveFecChannels = fActiveFecChannelsMap[fecID] ;
  
  //=== The next 4 words are Equip word, we dont care
  current_offset += 5 ;
  
  //================================================================================//
  // Start looking at the APV data word from here                                   //
  //================================================================================//
  while (current_offset < fEquipmentSize) {
    
    unsigned int rawdata = buffer[current_offset] ;
    
    if (rawdata == 0xfafafafa) {
      //===================================================================================================//
      // last word of the previous packet added for Filippo in DATE to count the eventNb x 16 UDP packets  //
      // We dont need it here, will just skip it We remove it from the vector of data                      //
      //===================================================================================================//
      if(!data32BitsVector.empty()) {
	apvID = (fecID << 4) | adcChannel ;
	int datasize = data32BitsVector.size() ;
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
    if ( ( ( (rawdata >> 8) & 0xffffff) == 0x41505a) && (executeonce == 0) ) {
      executeonce = 1;
      std::cout << std::endl << " The rawfile being processed is Zero suppressed! " << std::endl << std::endl;
      
      data32BitsVector.pop_back() ;
      if(!data32BitsVector.empty()) {
	apvID = (fecID << 4) | adcChannel ;
      }
                        
      currentAPVPacketHdr = rawdata  ;
      adcChannel = currentAPVPacketHdr & 0xff ;
                        
      //=== REINITIALISE EVERYTHING
      if(adcChannel > 15) {
	printf("  Publisher => ERROR #### fecID=%d, ADC Channel=%d, apvID=%d, \n",fecID, adcChannel, apvID) ;
	break ;
      }
      
      data32BitsVector.clear() ;
      fIsNewPacket = true;
      current_offset++ ;
      sigmaCut=-999;
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
  return sigmaCut;
}


int
SRSPublisher atoi(const std::string& s){
  int f;
  std::stringstream os;
  os<<s;
  os>>f;
  return f;
}
float 
SRSPublisher::atof(const std::string& s){
  float f;
  std::stringstream os;
  os<<s;
  os>>f;
  return f;
}
