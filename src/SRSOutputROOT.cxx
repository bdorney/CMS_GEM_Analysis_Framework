#include "SRSOutputROOT.h"


//SRSOutputROOT::SRSOutputROOT(const char * cfgname, TString zeroSupCutStr, TString rootDataType) {
SRSOutputROOT::SRSOutputROOT(const std::string& zeroSupCutStr, const std::string& rootDataType) {
    fRunName = "toto";
    fRunType = "PHYSICS" ;    
    fZeroSupCut = this->atoi(zeroSupCutStr);
    fROOTDataType = rootDataType ;
    //    std::cout <<" ++++ Initialized SRSOutputROOT "<<std::endl;
}

//====================================================================================================================
SRSOutputROOT::~SRSOutputROOT() {
  //  std::cout <<"CLOSING fFile "<<std::endl;
  fFile->Close();
  //std::cout <<"CLOSED fFile "<<std::endl;
  if (fZeroSupCut == 0) {
    this->DeleteHitsTree() ;
  }
  
  else {
    if (fROOTDataType == "HITS_ONLY") {
      this->DeleteHitsTree() ;
    }
    else if (fROOTDataType == "CLUSTERS_ONLY") {
      DeleteClustersTree() ;
    }
    else if (fROOTDataType == "TRACKING_ONLY") {
      //            DeleteTrackingTree() ;
    }
    else {
      this->DeleteHitsTree() ;
      this->DeleteClustersTree() ;
    }
  } 
}

//====================================================================================================================
void SRSOutputROOT::DeleteHitsTree() {
    if (m_adc0)  delete[] m_adc0 ;
    if (m_adc1)  delete[] m_adc1 ;
    if (m_adc2)  delete[] m_adc2 ;
    if (m_adc3)  delete[] m_adc3 ;
    if (m_adc4)  delete[] m_adc4 ;
    if (m_adc5)  delete[] m_adc5 ;
    if (m_adc6)  delete[] m_adc6 ;
    if (m_adc7)  delete[] m_adc7 ;
    if (m_adc8)  delete[] m_adc8 ;
    if (m_adc9)  delete[] m_adc9 ;
    if (m_adc10)  delete[] m_adc10 ;
    if (m_adc11)  delete[] m_adc11 ;
    if (m_adc12)  delete[] m_adc12 ;
    if (m_adc13)  delete[] m_adc13 ;
    if (m_adc14)  delete[] m_adc14 ;
    if (m_adc15)  delete[] m_adc15 ;
    if (m_adc16)  delete[] m_adc16 ;
    if (m_adc17)  delete[] m_adc17 ;
    if (m_adc18)  delete[] m_adc18 ;
    if (m_adc19)  delete[] m_adc19 ;
    if (m_adc20)  delete[] m_adc20;
    if (m_adc21)  delete[] m_adc21 ;
    if (m_adc22)  delete[] m_adc22 ;
    if (m_adc23)  delete[] m_adc23 ;
    if (m_adc24)  delete[] m_adc24 ;
    if (m_adc25)  delete[] m_adc25 ;
    if (m_adc26)  delete[] m_adc26 ;
    if (m_adc27)  delete[] m_adc27 ;
    if (m_adc28)  delete[] m_adc28 ;
    if (m_adc29)  delete[] m_adc29 ;

    if (m_hit_timeBin)   delete[] m_hit_timeBin ;
    if (m_strip)         delete[] m_strip ;
    if (m_hit_planeID)   delete[] m_hit_planeID ;
    if (m_hit_detID)     delete[] m_hit_detID ;
    if (m_hit_etaSector) delete[] m_hit_etaSector ;
    //    if (fHitTree)        delete fHitTree ;

}

//====================================================================================================================
void SRSOutputROOT::DeleteClustersTree() {
    if(m_clustTimeBin) delete[] m_clustTimeBin;
    if(m_clustPos)     delete[] m_clustPos ;
    if(m_clustSize)    delete[] m_clustSize ;
    if(m_clustADCs)    delete[] m_clustADCs ;
    if(m_planeID)      delete[] m_planeID ;
    if(m_detID)        delete[] m_detID ;
    if(m_etaSector)    delete[] m_etaSector ;
    //    if(fClusterTree)   delete fClusterTree ;
}

/*
//====================================================================================================================
void SRSOutputROOT::DeleteTrackingTree() {
    if(m_REF1)        delete[] m_REF1 ;
    if(m_REF2)        delete[] m_REF2 ;
    if(m_REF3)        delete[] m_REF3 ;
    if(m_REF4)        delete[] m_REF4 ;
    if(m_SBS1)        delete[] m_SBS1 ;
    if(m_UVAEIC)      delete[] m_UVAEIC ;
    if(fTrackingTree) delete fTrackingTree ;
}
*/
//====================================================================================================================
void SRSOutputROOT::FillRootFile(SRSEventBuilder * eventbuilder) {
  m_evtID++ ;
  //  std::cout <<"  ==== is a good Event?"<<std::endl;
  if (eventbuilder->IsAGoodEvent()) {
    //    std::cout <<" YES ! "<<std::endl;
    
    //    std::cout << " Zero Sup Cut "<<fZeroSupCut<<std::endl;
    if (fZeroSupCut == 0) {
      this->FillHitsTree(eventbuilder) ;
    }
    else {
      //     std::cout <<" fROOTDataType = "<<fROOTDataType<<std::endl;
      if (fROOTDataType == "HITS_ONLY") {
	this->FillHitsTree(eventbuilder) ;
      }
      else if (fROOTDataType == "CLUSTERS_ONLY") {
	this->FillClustersTree(eventbuilder) ;
      }
      else if (fROOTDataType == "TRACKING_ONLY") {
	//                FillTrackingTree(eventbuilder, track) ;
      }
      else {
	this->FillClustersTree(eventbuilder) ;
	this->FillHitsTree(eventbuilder) ;
      }
    }
  } else {
    //    std::cout <<" NO ! "<<std::endl;

  }
}

//============================================================================================
static bool CompareStripNo( SRSHit *obj1, SRSHit *obj2) {
    bool compare ;
    if ( obj1->GetStripNo() < obj2->GetStripNo() ) compare = true ;
    else compare = false ;
    return compare;
}

//====================================================================================================================
void SRSOutputROOT::FillHitsTree(SRSEventBuilder * eventbuilder) {
    //  printf("  SRSOutputROOT::FillHitsTree() ==> Enter \n") ;
    m_chID = 0 ;
    //    std::cout <<" In Fill Hits Tree "<<std::endl;
    
    SRSMapping * mapping = SRSMapping::GetInstance();
    
    Int_t NbADCTimeFrames = 40 ;
    std::vector<float > timebinCharges ;
    timebinCharges.resize(NbADCTimeFrames,0) ;
    
    
    std::map < std::string, std::list <SRSHit * > >  hitsInDetectorPlaneMap = eventbuilder->GetHitsInDetectorPlane() ;
    std::map < std::string, std::list <SRSHit * > >::const_iterator detPlane_itr ;
    //    std::cout <<"Loop on the dtectors "<<hitsInDetectorPlaneMap.size()<<std::endl;
    for (detPlane_itr = hitsInDetectorPlaneMap.begin(); detPlane_itr != hitsInDetectorPlaneMap.end(); detPlane_itr++) {
      std::string detPlaneName = (* detPlane_itr).first ;
      //      std::cout <<" Det plane name "<<detPlaneName<<std::endl;
        // Assign Detector ID
        Int_t detID = 0 ;
        Int_t  planeID = 0;
        Float_t etaSector = 0.0;
        
	std::string detName = mapping->GetDetectorFromPlane(detPlaneName) ;
	//	std::cout <<" DETNAME "<<detName<<std::endl;
        detID = mapping->GetDetectorIDFromDetector(detName) ;
	//std::cout <<" DETID "<<detID<<std::endl;
        planeID   = (Int_t) (mapping->GetPlaneIDorEtaSector(detPlaneName)) ;
	//std::cout << " Plane ID "<<planeID<<std::endl;
        etaSector = mapping->GetPlaneIDorEtaSector(detPlaneName) ;
	//std::cout << " detName "<<detName<<" planeID "<<planeID<<" etaSector " <<etaSector<<std::endl;

	std::list <SRSHit * > listOfHits = hitsInDetectorPlaneMap[detPlaneName] ;
        listOfHits.sort(CompareStripNo) ;
        
	std::list <SRSHit * >::const_iterator hit_itr ;
        
        for(hit_itr = listOfHits.begin(); hit_itr != listOfHits.end(); ++hit_itr ) {;
            SRSHit * hit = * hit_itr ;
            
            m_strip[m_chID]       = hit->GetStripNo() ;
            m_hit_timeBin[m_chID] = hit->GetSignalPeakBinNumber() ;
            
            m_hit_detID[m_chID]     = detID ;
            m_hit_planeID[m_chID]   = planeID ;
            m_hit_etaSector[m_chID] = (Short_t) etaSector ;
            
            Int_t nbOfTimeBin = hit->GetTimeBinADCs().size() ;
            if (nbOfTimeBin <= NbADCTimeFrames) {
	      //	      std::cout <<" Time profile for hit "<<m_strip[m_chID]<<" --- ";
	      for(Int_t t = 0; t < nbOfTimeBin; t++) {
		timebinCharges[t] = hit->GetTimeBinADCs()[t] ;
		//		std::cout <<" "<<timebinCharges[t];
	      }
	      //	      std::cout <<" --- "<<std::endl;
            }
            else {
                for(Int_t t = 0; t < NbADCTimeFrames; t++) {
                    timebinCharges[t] = hit->GetTimeBinADCs()[t] ;
                }
            }
            
            m_adc0[m_chID] = (short) timebinCharges[0];
            m_adc1[m_chID] = (short) timebinCharges[1];
            m_adc2[m_chID] = (short) timebinCharges[2];
            m_adc3[m_chID] = (short) timebinCharges[3];
            m_adc4[m_chID] = (short) timebinCharges[4];
            m_adc5[m_chID] = (short) timebinCharges[5];
            m_adc6[m_chID] = (short) timebinCharges[6];
            m_adc7[m_chID] = (short) timebinCharges[7];
            m_adc8[m_chID] = (short) timebinCharges[8];
            m_adc9[m_chID] = (short) timebinCharges[9];
            m_adc10[m_chID] = (short) timebinCharges[10];
            m_adc11[m_chID] = (short) timebinCharges[11];
            m_adc12[m_chID] = (short) timebinCharges[12];
            m_adc13[m_chID] = (short) timebinCharges[13];
            m_adc14[m_chID] = (short) timebinCharges[14];
            m_adc15[m_chID] = (short) timebinCharges[15];
            m_adc16[m_chID] = (short) timebinCharges[16];
            m_adc17[m_chID] = (short) timebinCharges[17];
            m_adc18[m_chID] = (short) timebinCharges[18];
            m_adc19[m_chID] = (short) timebinCharges[19];
            m_adc20[m_chID] = (short) timebinCharges[20];
            m_adc21[m_chID] = (short) timebinCharges[21];
            m_adc22[m_chID] = (short) timebinCharges[22];
            m_adc23[m_chID] = (short) timebinCharges[23];
            m_adc24[m_chID] = (short) timebinCharges[24];
            m_adc25[m_chID] = (short) timebinCharges[25];
            m_adc26[m_chID] = (short) timebinCharges[26];
            m_adc27[m_chID] = (short) timebinCharges[27];
            m_adc28[m_chID] = (short) timebinCharges[28];
            m_adc29[m_chID] = (short) timebinCharges[29];
            m_chID++;
            timebinCharges.clear() ;
            
        }
        listOfHits.clear() ;
    }
    hitsInDetectorPlaneMap.clear() ;
    fHitTree->Fill() ;
}


//====================================================================================================================
void SRSOutputROOT::FillClustersTree(SRSEventBuilder * eventbuilder) {
    //  printf("  SRSOutputROOT::FillClustersTree() ==> \n") ;
    m_nclust = 0 ;
    
    SRSMapping * mapping = SRSMapping::GetInstance();
    
    std::map < std::string, std::list <SRSCluster * > >  clustersInDetectorPlaneMap = eventbuilder->GetClustersInDetectorPlane() ;
    std::map < std::string, std::list <SRSCluster * > >::const_iterator detPlane_itr ;
    for (detPlane_itr = clustersInDetectorPlaneMap.begin(); detPlane_itr != clustersInDetectorPlaneMap.end(); detPlane_itr++) {
      std::string detPlaneName = detPlane_itr->first ;
        
        //Assign Detector ID
        Int_t detID = 0 ;
        Int_t  planeID = 0 ;
        Float_t etaSector = 0.0;
        
	std::string detName = mapping->GetDetectorFromPlane(detPlaneName) ;
        detID = mapping->GetDetectorIDFromDetector(detName) ;
        
        planeID   = (int) (mapping->GetPlaneIDorEtaSector(detPlaneName)) ;
        etaSector = mapping->GetPlaneIDorEtaSector(detPlaneName) ;
        
	std::list <SRSCluster * > listOfClusters = detPlane_itr->second  ;
	std::list <SRSCluster * >::const_iterator cluster_itr ;
        
        for(cluster_itr = listOfClusters.begin(); cluster_itr != listOfClusters.end(); ++cluster_itr ) {
            SRSCluster * cluster = * cluster_itr ;
            
            m_clustTimeBin[m_nclust] = cluster->GetClusterPeakTimeBin() ;
            m_clustSize[m_nclust]    = cluster->GetNbOfHits() ;
            m_clustPos[m_nclust]     = cluster->GetClusterPosition() ;
            m_clustADCs[m_nclust]    = cluster->GetClusterADCs() ;
            
            m_planeID[m_nclust]   = planeID ;
            m_detID[m_nclust]     = detID ;
            m_etaSector[m_nclust] = (short) etaSector ;
            m_nclust++ ;
        }
        listOfClusters.clear() ;
    }
    clustersInDetectorPlaneMap.clear() ;
    fClusterTree->Fill() ;
}

/*
//====================================================================================================================
void SRSOutputROOT::FillTrackingTree(SRSEventBuilder * eventbuilder, SRSTrack * track) {
    //  printf("  SRSOutputROOT::FillResidualHistos() ==> ENTER IN \n") ;
    
    for(Int_t i = 0; i < 3; i++) {
        m_REF1[i]   = -919191 ;
        m_REF2[i]   = -919191 ;
        m_REF3[i]   = -919191 ;
        m_REF4[i]   = -919191 ;
        m_SBS1[i]   = -919191 ;
        m_UVAEIC[i] = -919191 ;
    }
    
    if(track->IsAGoodTrack(eventbuilder)) {
        map <TString, TString >         detectorList = track->GetDetectorList() ;
        map <TString, vector<Float_t> > rawData = track->GetRawDataSpacePoints() ;
        map <TString, TString>::const_iterator detector_itr ;
        for(detector_itr =  detectorList.begin(); detector_itr !=  detectorList.end(); ++detector_itr) {
            
            TString detector = (*detector_itr).first ;
            vector <Float_t> rawPoint =  rawData[detector] ;
            
            Int_t size = rawPoint.size() ;
            if(size == 0) continue ;
            
            if(detector == "TrkGEM0")  {
                for(Int_t i = 0; i < 3; i++) m_REF1[i] = rawPoint[i] ;
            }
            if(detector == "TrkGEM1")  {;
                for(Int_t i = 0; i < 3; i++) m_REF2[i] = rawPoint[i] ;
            }
            if(detector == "TrkGEM2")  {
                for(Int_t i = 0; i < 3; i++) m_REF4[i] = rawPoint[i] ;
            }
            if(detector == "SBSGEM2")  {
                for(Int_t i = 0; i < 3; i++) m_REF3[i] = rawPoint[i] ;
            }
            if(detector == "SBSGEM1")  {
                for(Int_t i = 0; i < 3; i++) m_SBS1[i] = rawPoint[i] ;
            }
            if(detector == "EIC1")  {
                for(Int_t i = 0; i < 3; i++) m_UVAEIC[i] = rawPoint[i] ;
            }
        }
        fTrackingTree->Fill() ;
        
        detectorList.clear() ;
        map <TString, vector<Float_t> >::const_iterator raw_itr ;
        for(raw_itr =  rawData.begin(); raw_itr !=  rawData.end(); ++raw_itr) {
            vector<Float_t> rawdata = (*raw_itr).second ;
            rawdata.clear() ;
        }
        rawData.clear() ;
    }
}
*/
//====================================================================================================================
//void SRSOutputROOT::InitRootFile(const char * cfgname) {
void SRSOutputROOT::InitRootFile() {
    
  std::string fileName  = fRunName + std::string("_dataTree.root");
  
  printf("  SRSOutputROOT::InitRootFile() ==> Init Root file %s \n",fileName.c_str());
    
  m_evtID = 0 ;
  m_chID = 0 ;
  m_nclust = 0 ;
  
  fFile = TFile::Open(fileName.c_str(),"RECREATE");
    
  if (fZeroSupCut == 0) {
    printf("  SRSOutputROOT::InitRootFile() ==> Creating the Hit Tree: fHitTree \n") ;
    fHitTree = new TTree("THit","GEM Hit Data Rootfile") ;
    fHitTree->SetDirectory(fFile) ;
    
    m_strip         = new int[10000] ;
    m_hit_detID     = new int[10000] ;
    m_hit_planeID   = new int[10000] ;
    m_hit_timeBin   = new int[10000] ;
    m_hit_etaSector = new short[10000] ;
    
    m_adc0  = new short[10000] ;
    m_adc1  = new short[10000] ;
    m_adc2  = new short[10000] ;
    m_adc3  = new short[10000] ;
    m_adc4  = new short[10000] ;
    m_adc5  = new short[10000] ;
    m_adc6  = new short[10000] ;
    m_adc7  = new short[10000] ;
    m_adc8  = new short[10000] ;
    m_adc9  = new short[10000] ;
    m_adc10 = new short[10000] ;
    m_adc11 = new short[10000] ;
    m_adc12 = new short[10000] ;
    m_adc13 = new short[10000] ;
    m_adc14 = new short[10000] ;
    m_adc15 = new short[10000] ;
    m_adc16 = new short[10000] ;
    m_adc17 = new short[10000] ;
    m_adc18 = new short[10000] ;
    m_adc19 = new short[10000] ;
    m_adc20 = new short[10000] ;
    m_adc21 = new short[10000] ;
    m_adc22 = new short[10000] ;
    m_adc23 = new short[10000] ;
    m_adc24 = new short[10000] ;
    m_adc25 = new short[10000] ;
    m_adc26 = new short[10000] ;
    m_adc27 = new short[10000] ;
    m_adc28 = new short[10000] ;
    m_adc29 = new short[10000] ;
        
    printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fHitTree \n") ;
    fHitTree->Branch("evtID",&m_evtID,"evtID/I");
    fHitTree->Branch("nch",&m_chID,"nch/I");
    fHitTree->Branch("strip", m_strip, "strip[nch]/I");
    fHitTree->Branch("hitTimebin", m_hit_timeBin, "hitTimeBin[nch]/I");
        
    fHitTree->Branch("adc0", m_adc0, "adc0[nch]/S");
    fHitTree->Branch("adc1", m_adc1, "adc1[nch]/S");
    fHitTree->Branch("adc2", m_adc2, "adc2[nch]/S");
    fHitTree->Branch("adc3", m_adc3, "adc3[nch]/S");
    fHitTree->Branch("adc4", m_adc4, "adc4[nch]/S");
    fHitTree->Branch("adc5", m_adc5, "adc5[nch]/S");
    fHitTree->Branch("adc6", m_adc6, "adc6[nch]/S");
    fHitTree->Branch("adc7", m_adc7, "adc7[nch]/S");
    fHitTree->Branch("adc8", m_adc8, "adc8[nch]/S");
    fHitTree->Branch("adc9", m_adc9, "adc9[nch]/S");
    fHitTree->Branch("adc10", m_adc10, "adc10[nch]/S");
    fHitTree->Branch("adc11", m_adc11, "adc11[nch]/S");
    fHitTree->Branch("adc12", m_adc12, "adc12[nch]/S");
    fHitTree->Branch("adc13", m_adc13, "adc13[nch]/S");
    fHitTree->Branch("adc14", m_adc14, "adc14[nch]/S");
    fHitTree->Branch("adc15", m_adc15, "adc15[nch]/S");
    fHitTree->Branch("adc16", m_adc16, "adc16[nch]/S");
    fHitTree->Branch("adc17", m_adc17, "adc17[nch]/S");
    fHitTree->Branch("adc18", m_adc18, "adc18[nch]/S");
    fHitTree->Branch("adc19", m_adc19, "adc19[nch]/S");
    fHitTree->Branch("adc20", m_adc20, "adc20[nch]/S");
    fHitTree->Branch("adc21", m_adc21, "adc21[nch]/S");
    fHitTree->Branch("adc22", m_adc22, "adc22[nch]/S");
    fHitTree->Branch("adc23", m_adc23, "adc23[nch]/S");
    fHitTree->Branch("adc24", m_adc24, "adc24[nch]/S");
    fHitTree->Branch("adc25", m_adc25, "adc25[nch]/S");
    fHitTree->Branch("adc26", m_adc26, "adc26[nch]/S");
    fHitTree->Branch("adc27", m_adc27, "adc27[nch]/S");
    fHitTree->Branch("adc28", m_adc28, "adc28[nch]/S");
    fHitTree->Branch("adc29", m_adc29, "adc29[nch]/S");
    
    fHitTree->Branch("detID", m_hit_detID, "detID[nch]/I");
    fHitTree->Branch("planeID", m_hit_planeID, "planeID[nch]/I");
    fHitTree->Branch("etaSector", m_hit_etaSector, "etaSector[nch]/S");
  }
    
  else {
    if (fROOTDataType == "HITS_ONLY") {
      printf("  SRSOutputROOT::InitRootFile() ==> Creating the Hit Tree: fHitTree \n") ;
      fHitTree = new TTree("THit","GEM Hit Data Rootfile") ;
      fHitTree->SetDirectory(fFile) ;
      
      m_hit_detID     = new int[2000] ;
      m_hit_planeID   = new int[2000] ;
      m_hit_timeBin   = new int[2000] ;
      m_hit_etaSector = new short[2000] ;
      
      m_adc0  = new short[2000] ;
      m_adc1  = new short[2000] ;
      m_adc2  = new short[2000] ;
      m_adc3  = new short[2000] ;
      m_adc4  = new short[2000] ;
      m_adc5  = new short[2000] ;
      m_adc6  = new short[2000] ;
      m_adc7  = new short[2000] ;
      m_adc8  = new short[2000] ;
      m_adc9  = new short[2000] ;
      m_adc10 = new short[2000] ;
      m_adc11 = new short[2000] ;
      m_adc12 = new short[2000] ;
      m_adc13 = new short[2000] ;
      m_adc14 = new short[2000] ;
      m_adc15 = new short[2000] ;
      m_adc16 = new short[2000] ;
      m_adc17 = new short[2000] ;
      m_adc18 = new short[2000] ;
      m_adc19 = new short[2000] ;
      m_adc20 = new short[2000] ;
      m_adc21 = new short[2000] ;
      m_adc22 = new short[2000] ;
      m_adc23 = new short[2000] ;
      m_adc24 = new short[2000] ;
      m_adc25 = new short[2000] ;
      m_adc26 = new short[2000] ;
      m_adc27 = new short[2000] ;
      m_adc28 = new short[2000] ;
      m_adc29 = new short[2000] ;
      m_strip   = new int[2000] ;
      
      printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fHitTree \n") ;
      fHitTree->Branch("evtID",&m_evtID,"evtID/I") ;
      fHitTree->Branch("nch",&m_chID,"nch/I");
      fHitTree->Branch("strip", m_strip, "strip[nch]/I");
      fHitTree->Branch("hitTimebin", m_hit_timeBin, "hitTimeBin[nch]/I");
            
      fHitTree->Branch("adc0", m_adc0, "adc0[nch]/S");
      fHitTree->Branch("adc1", m_adc1, "adc1[nch]/S");
      fHitTree->Branch("adc2", m_adc2, "adc2[nch]/S");
      fHitTree->Branch("adc3", m_adc3, "adc3[nch]/S");
      fHitTree->Branch("adc4", m_adc4, "adc4[nch]/S");
      fHitTree->Branch("adc5", m_adc5, "adc5[nch]/S");
      fHitTree->Branch("adc6", m_adc6, "adc6[nch]/S");
      fHitTree->Branch("adc7", m_adc7, "adc7[nch]/S");
      fHitTree->Branch("adc8", m_adc8, "adc8[nch]/S");
      fHitTree->Branch("adc9", m_adc9, "adc9[nch]/S");
      fHitTree->Branch("adc10", m_adc10, "adc10[nch]/S");
      fHitTree->Branch("adc11", m_adc11, "adc11[nch]/S");
      fHitTree->Branch("adc12", m_adc12, "adc12[nch]/S");
      fHitTree->Branch("adc13", m_adc13, "adc13[nch]/S");
      fHitTree->Branch("adc14", m_adc14, "adc14[nch]/S");
      fHitTree->Branch("adc15", m_adc15, "adc15[nch]/S");
      fHitTree->Branch("adc16", m_adc16, "adc16[nch]/S");
      fHitTree->Branch("adc17", m_adc17, "adc17[nch]/S");
      fHitTree->Branch("adc18", m_adc18, "adc18[nch]/S");
      fHitTree->Branch("adc19", m_adc19, "adc19[nch]/S");
      fHitTree->Branch("adc20", m_adc20, "adc20[nch]/S");
      fHitTree->Branch("adc21", m_adc21, "adc21[nch]/S");
      fHitTree->Branch("adc22", m_adc22, "adc22[nch]/S");
      fHitTree->Branch("adc23", m_adc23, "adc23[nch]/S");
      fHitTree->Branch("adc24", m_adc24, "adc24[nch]/S");
      fHitTree->Branch("adc25", m_adc25, "adc25[nch]/S");
      fHitTree->Branch("adc26", m_adc26, "adc26[nch]/S");
      fHitTree->Branch("adc27", m_adc27, "adc27[nch]/S");
      fHitTree->Branch("adc28", m_adc28, "adc28[nch]/S");
      fHitTree->Branch("adc29", m_adc29, "adc29[nch]/S");
      
      fHitTree->Branch("detID", m_hit_detID, "detID[nch]/I");
      fHitTree->Branch("planeID", m_hit_planeID, "planeID[nch]/I");
      fHitTree->Branch("etaSector", m_hit_etaSector, "etaSector[nch]/S");
    }
    
    else if (fROOTDataType == "CLUSTERS_ONLY") {
      
      printf("  SRSOutputROOT::InitRootFile() ==> Creating the Cluster tree: fClusterTree \n") ;
      fClusterTree = new TTree("TCluster","GEM Cluster Data Rootfile") ;
      fClusterTree->SetDirectory(fFile) ;
      
      m_clustSize    = new int[200] ;
      m_clustTimeBin = new int[200] ;
      m_clustPos     = new float[200] ;
      m_clustADCs    = new float[200] ;
      m_detID        = new int[200] ;
      m_planeID      = new int[200] ;
      m_etaSector    = new short[200] ;
      
      printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fClusterTree \n") ;
      fClusterTree->Branch("evtID",&m_evtID,"evtID/I");
      fClusterTree->Branch("nclust",&m_nclust,"nclust/I");
      fClusterTree->Branch("clustPos", m_clustPos, "clustPos[nclust]/F");
      fClusterTree->Branch("clustSize", m_clustSize, "clustSize[nclust]/I");
      fClusterTree->Branch("clustADCs", m_clustADCs, "clustADCs[nclust]/F");
      fClusterTree->Branch("clustTimebin", m_clustTimeBin, "clustTimebin[nclust]/I");
      
      fClusterTree->Branch("detID", m_detID, "detID[nclust]/I");
      fClusterTree->Branch("planeID", m_planeID, "planeID[nclust]/I");
      fClusterTree->Branch("etaSector", m_etaSector, "etaSector[nclust]/S");
      
    }
        
    else if (fROOTDataType == "TRACKING_ONLY") {
      /*
      printf("  SRSOutputROOT::InitRootFile() ==> Creating the Tracking tree: fTrackingTree\n") ;
      
      fTrackingTree = new TTree("TTracking","Data from FLYSUB Test Beam @ FNAL: ") ;
      fTrackingTree ->SetDirectory(fFile) ;
      
      m_REF1   = new float[3] ;
      m_REF2   = new float[3] ;
      m_REF3   = new float[3] ;
      m_REF4   = new float[3] ;
      m_SBS1   = new float[3] ;
      m_UVAEIC = new float[3] ;
            
      printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fTrackingTree\n") ;
      fTrackingTree->Branch("evtID", &m_evtID, "evtID/I");
      fTrackingTree->Branch("REF1", m_REF1, "REF1[3]/F");
      fTrackingTree->Branch("REF2", m_REF2, "REF2[3]/F");
      fTrackingTree->Branch("REF3", m_REF3, "REF3[3]/F");
      fTrackingTree->Branch("REF4", m_REF4, "REF4[3]/F");
      fTrackingTree->Branch("SBS1", m_SBS1, "SBS1[3]/F");
      fTrackingTree->Branch("UVAEIC", m_UVAEIC, "UVAEIC[3]/F");
      */
    }
    
    else {
      printf("  SRSOutputROOT::InitRootFile() ==> Creating the Hit Tree: fHitTree \n") ;
      fHitTree = new TTree("THit","GEM Hit Data Rootfile") ;
      fHitTree->SetDirectory(fFile) ;
      
      m_hit_detID     = new int[2000] ;
      m_hit_planeID   = new int[2000] ;
      m_hit_etaSector = new short[2000] ;
      m_hit_timeBin   = new int[2000] ;
      
      m_adc0  = new short[2000] ;
      m_adc1  = new short[2000] ;
      m_adc2  = new short[2000] ;
      m_adc3  = new short[2000] ;
      m_adc4  = new short[2000] ;
      m_adc5  = new short[2000] ;
      m_adc6  = new short[2000] ;
      m_adc7  = new short[2000] ;
      m_adc8  = new short[2000] ;
      m_adc9  = new short[2000] ;
      m_adc10 = new short[2000] ;
      m_adc11 = new short[2000] ;
      m_adc12 = new short[2000] ;
      m_adc13 = new short[2000] ;
      m_adc14 = new short[2000] ;
      m_adc15 = new short[2000] ;
      m_adc16 = new short[2000] ;
      m_adc17 = new short[2000] ;
      m_adc18 = new short[2000] ;
      m_adc19 = new short[2000] ;
      m_adc20 = new short[2000] ;
      m_adc21 = new short[2000] ;
      m_adc22 = new short[2000] ;
      m_adc23 = new short[2000] ;
      m_adc24 = new short[2000] ;
      m_adc25 = new short[2000] ;
      m_adc26 = new short[2000] ;
      m_adc27 = new short[2000] ;
      m_adc28 = new short[2000] ;
      m_adc29 = new short[2000] ;
      m_strip   = new int[2000] ;
      
      printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fHitTree \n") ;
      fHitTree->Branch("evtID",&m_evtID,"evtID/I") ;
      fHitTree->Branch("nch",&m_chID,"nch/I");
      fHitTree->Branch("strip", m_strip, "strip[nch]/I");
      fHitTree->Branch("hitTimebin", m_hit_timeBin, "hitTimeBin[nch]/I");
      
      fHitTree->Branch("adc0", m_adc0, "adc0[nch]/S");
      fHitTree->Branch("adc1", m_adc1, "adc1[nch]/S");
      fHitTree->Branch("adc2", m_adc2, "adc2[nch]/S");
      fHitTree->Branch("adc3", m_adc3, "adc3[nch]/S");
      fHitTree->Branch("adc4", m_adc4, "adc4[nch]/S");
      fHitTree->Branch("adc5", m_adc5, "adc5[nch]/S");
      fHitTree->Branch("adc6", m_adc6, "adc6[nch]/S");
      fHitTree->Branch("adc7", m_adc7, "adc7[nch]/S");
      fHitTree->Branch("adc8", m_adc8, "adc8[nch]/S");
      fHitTree->Branch("adc9", m_adc9, "adc9[nch]/S");
      fHitTree->Branch("adc10", m_adc10, "adc10[nch]/S");
      fHitTree->Branch("adc11", m_adc11, "adc11[nch]/S");
      fHitTree->Branch("adc12", m_adc12, "adc12[nch]/S");
      fHitTree->Branch("adc13", m_adc13, "adc13[nch]/S");
      fHitTree->Branch("adc14", m_adc14, "adc14[nch]/S");
      fHitTree->Branch("adc15", m_adc15, "adc15[nch]/S");
      fHitTree->Branch("adc16", m_adc16, "adc16[nch]/S");
      fHitTree->Branch("adc17", m_adc17, "adc17[nch]/S");
      fHitTree->Branch("adc18", m_adc18, "adc18[nch]/S");
      fHitTree->Branch("adc19", m_adc19, "adc19[nch]/S");
      fHitTree->Branch("adc20", m_adc20, "adc20[nch]/S");
      fHitTree->Branch("adc21", m_adc21, "adc21[nch]/S");
      fHitTree->Branch("adc22", m_adc22, "adc22[nch]/S");
      fHitTree->Branch("adc23", m_adc23, "adc23[nch]/S");
      fHitTree->Branch("adc24", m_adc24, "adc24[nch]/S");
      fHitTree->Branch("adc25", m_adc25, "adc25[nch]/S");
      fHitTree->Branch("adc26", m_adc26, "adc26[nch]/S");
      fHitTree->Branch("adc27", m_adc27, "adc27[nch]/S");
      fHitTree->Branch("adc28", m_adc28, "adc28[nch]/S");
      fHitTree->Branch("adc29", m_adc29, "adc29[nch]/S");
      
      fHitTree->Branch("detID", m_hit_detID, "detID[nch]/I");
      fHitTree->Branch("planeID", m_hit_planeID, "planeID[nch]/I");
      fHitTree->Branch("etaSector", m_hit_etaSector, "etaSector[nch]/S");
     
      printf("  SRSOutputROOT::InitRootFile() ==> Creating the Cluster Tree: fClusterTree \n") ;
      fClusterTree = new TTree("TCluster","GEM Cluster Data Rootfile") ;
      fClusterTree->SetDirectory(fFile) ;
            
      m_clustSize    = new int[200] ;
      m_clustTimeBin = new int[200] ;
      m_clustPos     = new float[200] ;
      m_clustADCs    = new float[200] ;
      m_detID        = new int[200] ;
      m_planeID      = new int[200] ;
      m_etaSector    = new short[200] ;
      
      printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fClusterTree \n") ;
      fClusterTree->Branch("evtID",&m_evtID,"evtID/I");
      fClusterTree->Branch("nclust",&m_nclust,"nclust/I");
      fClusterTree->Branch("clustPos", m_clustPos, "clustPos[nclust]/F");
      fClusterTree->Branch("clustSize", m_clustSize, "clustSize[nclust]/I");
      fClusterTree->Branch("clustADCs", m_clustADCs, "clustADCs[nclust]/F");
      fClusterTree->Branch("clustTimebin", m_clustTimeBin, "clustTimeBin[nclust]/I");
      
      fClusterTree->Branch("detID", m_detID, "detID[nclust]/I");
      fClusterTree->Branch("planeID", m_planeID, "planeID[nclust]/I");
      fClusterTree->Branch("etaSector", m_etaSector, "etaSector[nclust]/S");
      
    }
  }
}

//====================================================================================================================
void SRSOutputROOT::WriteRootFile() {
    
    if (fZeroSupCut == 0) {
        fHitTree->Write() ;
    }
    
    else {
        if (fROOTDataType == "HITS_ONLY") {
            // A bug to be fixed? Weird with ZS no problem, without ROOT is complaining.. mistery...
            fHitTree->Write() ;

        }
        else if (fROOTDataType == "CLUSTERS_ONLY") {
	  fClusterTree->Write(); 
        } 
        else if (fROOTDataType == "TRACKING_ONLY") {
	  //            fTrackingTree->Write(); 
        } 
        else {
            fHitTree->Write() ;
	    fClusterTree->Write();
        }
    }
    fFile->Write();
    //fFile->Close();
}

int
SRSOutputROOT::atoi(const std::string& a){
  std::stringstream s;
  s<<a;
  int c;
  s>>c;
  return c;
}
