#include "SRSEventBuilder.h"
#include "SRSCluster.h"
#include <sstream>

//============================================================================================
SRSEventBuilder::SRSEventBuilder(int triggerNb, std::string maxClusterSize, std::string minClusterSize, std::string zeroSupCut, std::string runType, bool isClusterPosCorrection ) {
    
    fTriggerCount = triggerNb ;
    
    //  printf("==SRSEventBuilder::SRSEventBuilder() triggerNb = %d \n", triggerNb) ;
    fZeroSupCut = this->atoi(zeroSupCut);
    fMaxClusterSize = 100000 ;
    fMinClusterSize = 1 ;
    fIsClusterPosCorrection = isClusterPosCorrection ;
    if(fZeroSupCut != 0) {
      fMinClusterSize = this->atoi(minClusterSize) ;
      fMaxClusterSize = this->atoi(maxClusterSize);
    }
    
    fIsHitMaxOrTotalADCs = "signalPeak";
    //    fClusterPositionCorrectionRootFile = "" ;
    
    fIsGoodEvent = false ;
    // fIsGoodClusterEvent = false ;
    
    fRunType = runType ;
    
    /**
     fTrapezoidDetLength    = 1000 ;
     fTrapezoidDetInnerRadius = 220 ;
     fTrapezoidDetOuterRadius = 430 ;
     */
    
    //  fEICstereoAngleDegree = (6.067 * PI ) / 180 ;
    //  fUVangleCosineDirection   = TMath::Tan(fTrapezoidDetUVangleDegree) ;
    //  fUVangleCosineDirection = (430 - 220) / (2 * fTrapezoidDetLength) ;
    //  fUVangleCosineDirection = ( fTrapezoidDetOuterRadius - fTrapezoidDetInnerRadius ) / (2 * fTrapezoidDetLength) ;
    //  fUVangle = TMath::ATan(fUVangleCosineDirection) ;
}

//============================================================================================
SRSEventBuilder::~SRSEventBuilder() {
    fTriggerList.clear() ;
    DeleteListOfHits(fListOfHits) ;
    DeleteListOfAPVEvents(fListOfAPVEvents) ;
    DeleteHitsInDetectorPlaneMap(fHitsInDetectorPlaneMap) ;
    //    DeleteClustersInDetectorPlaneMap(fClustersInDetectorPlaneMap) ;
}

//============================================================================================
template <typename M> void ClearVectorMap( M & amap ) {
    for ( typename M::iterator it = amap.begin(); it != amap.end(); ++it ) {
        ((*it).second).clear();
    }
    amap.clear() ;
}

//============================================================================================
void SRSEventBuilder::DeleteHitsInDetectorPlaneMap( std::map<std::string, std::list <SRSHit * > > & stringListMap) {
  std::map < std::string, std::list <SRSHit *> >::const_iterator itr ;
  for (itr = stringListMap.begin(); itr != stringListMap.end(); itr++) {
    std::list <SRSHit *> listOfHits = (*itr).second ;
    std::list <SRSHit *>::const_iterator hit_itr ;
    for (hit_itr = listOfHits.begin(); hit_itr != listOfHits.end(); hit_itr++) {
      delete (* hit_itr) ;
    }
    listOfHits.clear() ;
  }
  stringListMap.clear() ;
}

/*
//============================================================================================
void SRSEventBuilder::DeleteClustersInDetectorPlaneMap( map<std::string, std::list <SRSCluster * > > & stringListMap) {
    map < std::string, list <SRSCluster *> >::const_iterator itr ;
    for (itr = stringListMap.begin(); itr != stringListMap.end(); itr++) {
        list <SRSCluster *> listOfClusters = (*itr).second ;
        list <SRSCluster *>::const_iterator cluster_itr ;
        for (cluster_itr = listOfClusters.begin(); cluster_itr != listOfClusters.end(); cluster_itr++) {
            delete (* cluster_itr) ;
        }
        listOfClusters.clear() ;
    }
    stringListMap.clear() ;
}
*/

//============================================================================================
void SRSEventBuilder::DeleteListOfAPVEvents(std::list<SRSAPVEvent*>&  listOfAPVEvents) {
  std::list<SRSAPVEvent*>::iterator iapve;
  for (iapve=listOfAPVEvents.begin(); iapve!=listOfAPVEvents.end();iapve++){
    delete (*iapve);
  }
  listOfAPVEvents.clear();
}

/*
//============================================================================================
void SRSEventBuilder::DeleteListOfClusters(TList * listOfClusters) {
    listOfClusters->Clear();
    delete listOfClusters;
}
*/


//============================================================================================
void SRSEventBuilder::DeleteListOfHits(std::list<SRSHit *>& listOfHits) {
  std::list<SRSHit *>::iterator hit;
  for (hit=listOfHits.begin();hit!=listOfHits.end();hit++){
    delete (*hit);
  }
  listOfHits.clear();
}

//============================================================================================
static bool CompareStripNo( SRSHit *obj1, SRSHit *obj2) {
    bool compare ;
    if ( obj1->GetStripNo() < obj2->GetStripNo() ) compare = true;
    else compare = false;
    return compare ;
}

//============================================================================================
static bool CompareHitADCs( SRSHit *obj1, SRSHit *obj2) {
    bool compare ;
    if (obj1->GetHitADCs() > obj2->GetHitADCs()) compare = true ;
    else compare = false ;
    return compare ;
}

//============================================================================================
static bool CompareClusterADCs( SRSCluster *obj1, SRSCluster *obj2) {
    bool compare ;
    if ( obj1->GetClusterADCs() > obj2->GetClusterADCs()) compare = true ;
    else compare = false ;
    return compare ;
}



//============================================================================================
void SRSEventBuilder::ComputeClustersInDetectorPlane() {
    //  printf("==SRSEventBuilder::ComputeClustersInDetectorPlane() \n") ;
  std::map<std::string, std::list <SRSHit*> >::const_iterator  listOfHits_itr ;
  for (listOfHits_itr = fHitsInDetectorPlaneMap.begin(); listOfHits_itr != fHitsInDetectorPlaneMap.end(); ++listOfHits_itr) {
    std::string detPlane =  listOfHits_itr->first;
    SRSMapping * mapping = SRSMapping::GetInstance();
    std::string detector = mapping->GetDetectorFromPlane(detPlane);
    std::string readoutBoard = mapping->GetReadoutBoardFromDetector(detector);
        
    std::list <SRSHit*> listOfHits = listOfHits_itr->second ;
    listOfHits.sort(CompareStripNo) ;
    int listSize = listOfHits.size() ;
    if (listSize < fMinClusterSize) {
      fIsGoodEvent = false ;
      continue ;
    }
        
    int previousStrip = -2 ;
    int clusterNo = -1 ;
        
    std::map<int, SRSCluster *> clustersMap ;
    std::list <SRSHit *>::const_iterator hit_itr ;
    for (hit_itr = listOfHits.begin(); hit_itr != listOfHits.end(); hit_itr++) {
      SRSHit* hit =  *hit_itr ;            
      int currentStrip = hit->GetStripNo() ;
      int apvIndexOnPlane = hit->GetAPVIndexOnPlane();      
      if(readoutBoard != "PADPLANE") {
	if(currentStrip != (previousStrip + 1)) {
	  clusterNo++ ;
	}
      }
      else {
	clusterNo++ ;
      }
            
      if(!clustersMap[clusterNo]) {
	//	std:: cout <<" >>> Creating Cluster request min size="<<fMinClusterSize<<" max size="<<fMaxClusterSize<<std::endl;
	clustersMap[clusterNo] = new SRSCluster(fMinClusterSize, fMaxClusterSize, fIsClusterMaxOrTotalADCs) ;
	clustersMap[clusterNo]->SetNbAPVsFromPlane(hit->GetNbAPVsFromPlane());
	clustersMap[clusterNo]->SetPlaneSize(hit->GetPlaneSize());
	clustersMap[clusterNo]->SetPlane(hit->GetPlane());
      }
      clustersMap[clusterNo]->AddHit(hit) ;
      previousStrip = currentStrip;
    }
        
    std::map<int, SRSCluster *>::const_iterator  cluster_itr;
    
    for (cluster_itr = clustersMap.begin(); cluster_itr != clustersMap.end(); cluster_itr++) {
      SRSCluster * cluster = cluster_itr->second ;
            
      if (!cluster->IsGoodCluster()) {
	std::cout <<"  OPS Bad cluster!!!!"<<std::endl;
	delete cluster ;
	continue ;
      }
      if (fIsClusterPosCorrection) {
	//	cluster->SetClusterPositionCorrection(fIsClusterPosCorrection) ;
	//	cluster->ComputeClusterPositionWithCorrection(fClusterPositionCorrectionRootFile) ;
      }
      else {
	cluster->SetClusterPositionCorrection(false) ;
	cluster->ComputeClusterPositionWithoutCorrection() ;
      }
      fClustersInDetectorPlaneMap[detPlane].push_back(cluster) ;
    }
    fClustersInDetectorPlaneMap[detPlane].sort(CompareClusterADCs) ;
    
    listOfHits.clear() ;
    clustersMap.clear() ;
  }
  

}



//============================================================================================
bool  SRSEventBuilder::IsAGoodEventInDetector(std::string detector) {
    bool IsGoodEventInDetector = false;
    SRSMapping * mapping = SRSMapping::GetInstance() ;
    std::string readoutBoard = mapping->GetReadoutBoardFromDetector(detector) ;
    
    if( readoutBoard == "PADPLANE") {
      std::string padPlane = (mapping->GetDetectorPlaneListFromDetector(detector)).front();
      std::list <SRSHit*> listOfHits = fHitsInDetectorPlaneMap[padPlane];
        int size = listOfHits.size() ;
        if (size > 0) IsGoodEventInDetector = true ;
    }
    
    else if(readoutBoard == "CMSGEM") {
      std::string plane = (mapping->GetDetectorPlaneListFromDetector(detector)).front();
      // Temporary good after clusterization ..
      int clusterMultiplicity = fClustersInDetectorPlaneMap[plane].size() ;
      //int clusterMultiplicity = 1;
      std::cout <<" ==== Cluster multiplicity in the event "<<clusterMultiplicity <<"  compared with max "<<fMaxClusterMultiplicity<<std::endl;
      if ( (clusterMultiplicity == 0) ||  (clusterMultiplicity > fMaxClusterMultiplicity) ) {
	fClustersInDetectorPlaneMap[plane].clear() ;
	IsGoodEventInDetector = false ;
      }
      else {
	IsGoodEventInDetector = true ;
      }
    }
    
    else if( readoutBoard == "1DSTRIPS") {
        std::string plane = (mapping->GetDetectorPlaneListFromDetector(detector)).front();
	//        int clusterMultiplicity = fClustersInDetectorPlaneMap[plane.Data()].size() ;
        //if ( (clusterMultiplicity == 0) || (clusterMultiplicity > fMaxClusterMultiplicity) ) {
	  //            fClustersInDetectorPlaneMap[plane.Data()].clear() ;
	//   IsGoodEventInDetector = false ;
	    //}
        //else {
            IsGoodEventInDetector = true ;
	    // }
    }
    
    else {
      /*
        std::string detPlaneX = (mapping->GetDetectorPlaneListFromDetector(detector)).front() ;
        std::string detPlaneY = (mapping->GetDetectorPlaneListFromDetector(detector)).back() ;
        int clusterMultiplicityX = fClustersInDetectorPlaneMap[detPlaneX.Data()].size() ;
        int clusterMultiplicityY = fClustersInDetectorPlaneMap[detPlaneY.Data()].size() ;
        
        if ( (fTriggerList[detector] == "isTrigger") and ( (clusterMultiplicityX > 1) or (clusterMultiplicityY > 1)) ) {
            IsGoodEventInDetector = false ;
            fClustersInDetectorPlaneMap[detPlaneX.Data()].clear() ;
            fClustersInDetectorPlaneMap[detPlaneY.Data()].clear() ;
        }
        
        if ((clusterMultiplicityX == 0) or (clusterMultiplicityY == 0) or (clusterMultiplicityX > fMaxClusterMultiplicity) or (clusterMultiplicityY > fMaxClusterMultiplicity) ) {
            IsGoodEventInDetector = false ;
            fClustersInDetectorPlaneMap[detPlaneX.Data()].clear() ;
            fClustersInDetectorPlaneMap[detPlaneY.Data()].clear() ;
        }
        else {
            IsGoodEventInDetector = true ;
        }
      */
    }
    std::cout <<" Is in "<<detector<<"  a good event? "<<IsGoodEventInDetector<<std::endl;
    return IsGoodEventInDetector ;
}

//============================================================================================
bool  SRSEventBuilder::IsAGoodEvent() {
  //  std::cout <<" ==== IS it a good event? "<<std::endl;
  fIsGoodEvent = false ;
  int nbOfTriggeredDetectors = 0 ;
  
  SRSMapping * mapping = SRSMapping::GetInstance() ;
  int nbOfDetectorsToBeTriggered = fTriggerList.size() ;
  //  std::cout <<" ====++  Number of detector to be triggered "<<nbOfDetectorsToBeTriggered<<std::endl;
  if (nbOfDetectorsToBeTriggered == 0 ) {
    fIsGoodEvent = true ;
  }
    
  else {
    std::map<std::string, std::string>::const_iterator trigger_itr ;
    for (trigger_itr = fTriggerList.begin(); trigger_itr != fTriggerList.end(); trigger_itr++) {
      std::string detector = (*trigger_itr).first ;
      if (!IsAGoodEventInDetector(detector) ) {
	continue ;
      }
      else {
	nbOfTriggeredDetectors++ ;
      }
    }
    if (nbOfTriggeredDetectors == nbOfDetectorsToBeTriggered) {
      fIsGoodEvent = true ;
    }
  }
  //  std::cout <<" Is it a good event? "<<fIsGoodEvent<<std::endl;
  return fIsGoodEvent ;
}
/*
//============================================================================================
bool  SRSEventBuilder::IsAGoodClusterEvent(std::string detPlane) {
    fIsGoodClusterEvent = false ;
    
    SRSMapping * mapping = SRSMapping::GetInstance() ;
    std::string detector = mapping->GetDetectorFromPlane(detPlane) ;
    std::string readoutBoard = mapping->GetReadoutBoardFromDetector(detector) ;
    
    if(readoutBoard == "PADPLANE") {
        list <SRSHit*> listOfHits = fHitsInDetectorPlaneMap[detPlane] ;
        int size = listOfHits.size() ;
        if (size > 0) fIsGoodClusterEvent = true ;
        listOfHits.clear() ;
    }
    
    else {
        int clusterMult = fClustersInDetectorPlaneMap[detPlane].size()  ;
        if ((clusterMult == 0) || (clusterMult > fMaxClusterMultiplicity) ) {
	fClustersInDetectorPlaneMap[detPlane].clear() ;
	fIsGoodClusterEvent = false ;
        }
        else {
            fIsGoodClusterEvent = true ;
        }
    }
    return fIsGoodClusterEvent ;
}
//============================================================================================
*/
/**
 std::list <SRSCluster * >  SRSEventBuilder::CrossTalkCorrection( list <SRSCluster * >  listOfClusters) {
 
 int clusterMultiplicity = listOfClusters.size() ;
 
 if(clusterMultiplicity == 2) {
 SRSCluster * cluster1 = listOfClusters.front() ;
 SRSCluster * cluster2 = listOfClusters.back() ;
 float diffStripNb    = (fabs(cluster1->GetClusterCentralStrip() - cluster2->GetClusterCentralStrip())) ;
 float ratioADCs = cluster1->GetClusterADCs() /  cluster2->GetClusterADCs() ;
 
 float criteria32 = fabs(diffStripNb - 32) ;
 float criteria88 = fabs(diffStripNb - 88) ;
 
 if ((( criteria32 <= 1)  || (criteria88 <=1)) && (ratioADCs > 1) ) {
 cluster1->SetClusterADCs(cluster1->GetClusterADCs() +  cluster2->GetClusterADCs() ) ;
 listOfClusters.pop_back() ;
 clusterMultiplicity = listOfClusters.size() ;
 }
 }
 return listOfClusters ;
 }
 */
/*
//============================================================================================
std::map < int, std::vector <float > > SRSEventBuilder::GetDetectorCluster(std::string detector) {
    
    SRSMapping * mapping = SRSMapping::GetInstance() ;
    
    std::string detPlaneX = (mapping->GetDetectorPlaneListFromDetector(detector)).front() ;
    std::string detPlaneY = (mapping->GetDetectorPlaneListFromDetector(detector)).back() ;
    std::string readoutBoard = mapping->GetReadoutBoardFromDetector(detector) ;
    
    int planeOrientationX = mapping->GetPlaneOrientation(detPlaneX) ;
    int planeOrientationY = mapping->GetPlaneOrientation(detPlaneY) ;
    
    
    // planeOrientationX = 1 ;
    // planeOrientationY = 1;
     
    
    
    map < int, vector <float > > detectorClusterMap ;
    list <SRSCluster * > listOfClustersX = fClustersInDetectorPlaneMap[detPlaneX.Data()] ;
    list <SRSCluster * > listOfClustersY = fClustersInDetectorPlaneMap[detPlaneY.Data()] ;
    
    int clusterMultiplicityX = listOfClustersX.size() ;
    int clusterMultiplicityY = listOfClustersY.size() ;
    
    int clusterMult = clusterMultiplicityX ;
    if ( clusterMultiplicityY < clusterMult )    clusterMult = clusterMultiplicityY  ;
    if ( fMaxClusterMultiplicity < clusterMult ) clusterMult = fMaxClusterMultiplicity ;
    
    TList  * clusterListX, * clusterListY;
    clusterListX  = new TList ;
    clusterListY  = new TList ;
    
    list <SRSCluster * >::const_iterator clusterX_itr ;
    for(clusterX_itr = listOfClustersX.begin(); clusterX_itr != listOfClustersX.end(); ++clusterX_itr ) {
        clusterListX->Add(* clusterX_itr) ;
    }
    list <SRSCluster * >::const_iterator clusterY_itr ;
    for(clusterY_itr = listOfClustersY.begin(); clusterY_itr != listOfClustersY.end(); ++clusterY_itr ) {
        clusterListY->Add(* clusterY_itr) ;
    }
    
    for (int k = 0 ; k < clusterMult ; k++) {
        float clusterPos1   = ((SRSCluster *) clusterListX->At(k))->GetClusterPosition() ;
        float clusterPos2   = ((SRSCluster *) clusterListY->At(k))->GetClusterPosition() ;
        
        float x_coord   = ((SRSCluster *) clusterListX->At(k))->GetClusterPosition() ;
        float y_coord   = ((SRSCluster *) clusterListY->At(k))->GetClusterPosition() ;
        
        clusterPos1 = planeOrientationX * clusterPos1 ;
        clusterPos2 = planeOrientationY * clusterPos2 ;
        
        float xpos =  clusterPos1;
        float ypos =  clusterPos2;
        
        float adcCount1 = ((SRSCluster *) clusterListX->At(k))->GetClusterADCs() ;
        float adcCount2 = ((SRSCluster *) clusterListY->At(k))->GetClusterADCs() ;
        
        float timing1 = ((SRSCluster *) clusterListX->At(k))->GetClusterTimeBin() ;
        float timing2 = ((SRSCluster *) clusterListY->At(k))->GetClusterTimeBin() ;
        
        if (readoutBoard == "UV_ANGLE") {
            float trapezoidDetLength = mapping->GetUVangleReadoutMap(detector) [0] ;
            float trapezoidDetInnerRadius =  mapping->GetUVangleReadoutMap(detector) [1] ;
            float trapezoidDetOuterRadius =  mapping->GetUVangleReadoutMap(detector) [2] ;
            float uvAngleCosineDirection = ( trapezoidDetOuterRadius - trapezoidDetInnerRadius ) / (2 * trapezoidDetLength) ;
            
            //      printf("==SRSEventBuilder::GetDetectorCluster(): trapezoidDetLength=%f, trapezoidDetInnerRadius=%f trapezoidDetOuterRadius=%f, uvAngleCosineDirection=%f\n", trapezoidDetLength, trapezoidDetInnerRadius, trapezoidDetOuterRadius, uvAngleCosineDirection) ;
            
            xpos = 0.5 * (trapezoidDetLength + ( (clusterPos1 - clusterPos2) / uvAngleCosineDirection ) );
            ypos = 0.5 * (clusterPos1 + clusterPos2) ;
        }
        
        if ( readoutBoard == "PADPLANE")  {
            SRSHit * hit = ((SRSCluster *) clusterListX->At(k))->GetHit(0) ;
            vector<float> padPosition = hit->GetPadPosition() ;
            xpos = hit->GetPadPosition() [0];
            ypos = hit->GetPadPosition() [1];
            adcCount1 =  hit->GetHitADCs() ;
            adcCount2 =  hit->GetHitADCs() ;
        }
        
        if ( (adcCount1 == 0) || (adcCount2 == 0) ) {
            fClustersInDetectorPlaneMap[detPlaneX.Data()].clear() ;
            fClustersInDetectorPlaneMap[detPlaneY.Data()].clear() ;
            continue ;
        }
        detectorClusterMap[k].clear() ;
        detectorClusterMap[k].push_back(xpos) ;
        detectorClusterMap[k].push_back(ypos) ;
        detectorClusterMap[k].push_back(adcCount1) ;
        detectorClusterMap[k].push_back(adcCount2) ;
        detectorClusterMap[k].push_back(timing1) ;
        detectorClusterMap[k].push_back(timing2) ;
        detectorClusterMap[k].push_back(clusterPos1) ;
        detectorClusterMap[k].push_back(clusterPos2) ;
    }
    
    DeleteListOfClusters(clusterListX) ;
    DeleteListOfClusters(clusterListY) ;
    listOfClustersX.clear() ;
    listOfClustersY.clear() ;
    
    return detectorClusterMap ;
}

*/

int
SRSEventBuilder::atoi(const std::string& s){
  int a;
  std::stringstream os;
  os<<s;
  os>>a;
  return a;
}
