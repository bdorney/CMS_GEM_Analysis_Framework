#ifndef __SRSEVENTBUILDER__
#define __SRSEVENTBUILDER__

#include <map>
#include <list>
#include <vector>
#include <iostream>

//#include "SRSMapping.h"
//#include "SRSPedestal.h"
#include "SRSCluster.h"
#include "SRSHit.h"
#include "SRSAPVEvent.h"

#define PI 3.14159265359

class SRSEventBuilder{
    
public:
    
  SRSEventBuilder(int triggerNb, std::string maxClusterSize, std::string minClusterSize, std::string nbofsigma, std::string runType, bool isCluserPosCorrection ) ;
    
    
    ~SRSEventBuilder() ;
    
    template <typename M> void ClearVectorMap( M & amap ) ;
    
    void AddHitInDetectorPlane(SRSHit* hit) {fHitsInDetectorPlaneMap[hit->GetPlane()].push_back(hit) ;}
    
    void AddHit(SRSHit* hit ) {fListOfHits.push_back(hit) ;}
    
    void AddMeanDetectorPlaneNoise(std::string detPlaneName, float meanAPVnoise) { fDetectorPlaneNoise[detPlaneName].push_back(meanAPVnoise);}
    
    void ComputeClustersInDetectorPlane() ;
    void AddAPVEvent(SRSAPVEvent* apvEvent) {fListOfAPVEvents.push_back(apvEvent); }
    std::string GetRunType() {return fRunType;}
    
    std::list<SRSHit*>  GetListOfHits() {return fListOfHits;}
    
    std::map < std::string, std::list <SRSHit * > > GetHitsInDetectorPlane() {return fHitsInDetectorPlaneMap;}
    std::map < std::string, std::list <SRSCluster * > >  GetClustersInDetectorPlane() {return fClustersInDetectorPlaneMap ;}
    
    void SetTriggerList(std::map<std::string, std::string> triggerList)  {fTriggerList = triggerList ;}
    int GetTriggerCount() {return fTriggerCount;}
    
    bool IsAGoodEvent()  ;
    //    bool IsAGoodClusterEvent(std::string detPlaneName)  ;
    bool IsAGoodEventInDetector(std::string detName) ;
    
    //    bool IsCluserPosCorrection() {return fIsClusterPosCorrection ;}
    
    //  list <SRSCluster * > CrossTalkCorrection( list <SRSCluster * >  listOfClusters) ;
    
    //    SRSAPVEvent * GetAPVEventFromAPVKey(int apvKey) {return (SRSAPVEvent*) (fListOfAPVEvents->At(apvKey));}
    
    void DeleteListOfAPVEvents(std::list<SRSAPVEvent*>&  listOfAPVEvents) ;
    void DeleteListOfHits(std::list<SRSHit *>& listOfHits) ;
    //void DeleteListOfClusters(TList * listOfClusters) ;
    
    void DeleteHitsInDetectorPlaneMap( std::map <std::string, std::list <SRSHit * > >  & stringListMap) ;
    //    void DeleteClustersInDetectorPlaneMap( std::map < std::string, std::list <SRSCluster * > > & stringListMap) ;
    //    float GetDetectorPlaneNoise(std::string planeName) { return TMath::Mean(fDetectorPlaneNoise[planeName].begin(), fDetectorPlaneNoise[planeName].end());}
    
    //    std::map < int, std::vector <float > > GetDetectorCluster(std::string detector) ;
    
    //    void SetClusterPositionCorrection(bool isClusterPosCorrection) {fIsClusterPosCorrection = isClusterPosCorrection;}
    // void SetClusterPositionCorrectionRootFile(const char * filename) {fClusterPositionCorrectionRootFile = filename;}
    
    void SetHitMaxOrTotalADCs(std::string isHitMaxOrTotalADCs) {fIsHitMaxOrTotalADCs = isHitMaxOrTotalADCs ;}
    std::string GetHitMaxOrTotalADCs() {return fIsHitMaxOrTotalADCs;}
    
    //    void SetMaxClusterMultiplicity(std::string maxClusterMult) {fMaxClusterMultiplicity = maxClusterMult.Atoi() ;}
    //    std::string GetMaxClusterMultiplicity() {return fMaxClusterMultiplicity;}
    
    //    void SetClusterMaxOrTotalADCs(std::string isClusterMaxOrTotalADCs) { fIsClusterMaxOrTotalADCs = isClusterMaxOrTotalADCs ;}
    // std::string GetClusterMaxOrTotalADCs() {return fIsClusterMaxOrTotalADCs;}
    
private:
    int atoi(const std::string& s);
    int fTriggerCount, fMinClusterSize, fMaxClusterSize, fMaxClusterMultiplicity, fZeroSupCut ;
    
    std::list<SRSAPVEvent *>  fListOfAPVEvents; 
    std::list<SRSHit *>       fListOfHits;
    std::string fIsClusterMaxOrTotalADCs, fIsHitMaxOrTotalADCs, fRunType ;
    bool fIsGoodEvent, fIsGoodClusterEvent, fIsClusterPosCorrection ;
    
    std::map < std::string, std::vector <float > > fDetectorPlaneNoise ;
    std::map<std::string, std::string> fTriggerList ;
    std::map < std::string, std::list <SRSHit * > >  fHitsInDetectorPlaneMap ;
    std::map < std::string, std::list <SRSCluster * > >  fClustersInDetectorPlaneMap ;
    
    const char * fClusterPositionCorrectionRootFile ;
    
};
#endif
