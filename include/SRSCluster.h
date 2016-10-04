#ifndef SRSCLUSTER_H
#define SRSCLUSTER_H
/*******************************************************************************
 *  AMORE FOR SRS - SRS                                                         *
 *  SRSCluster                                                                  *
 *  SRS Module Class                                                            *
 *  Author: Kondo GNANVO 18/08/2010                                             *
 *******************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include "SRSHit.h"

class SRSCluster {

 public:
  SRSCluster(int minClusterSize, int maxClusterSize, std::string isMaximumOrTotalCharges) ;
  ~SRSCluster();
    

    bool IsSortable() const {return true;}
    std::vector<SRSHit* > GetArrayOfHits() {return fArrayOfHits;}
    
    SRSHit * GetHit(int i) {
        return fArrayOfHits[i];
    }
    
    void SetMinClusterSize(int min) {fMinClusterSize = min ; }
    void SetMaxClusterSize(int max) {fMaxClusterSize = max ; }
    
    void AddHit(SRSHit * h) ;
    void Timing() ;
    
    int Compare(SRSCluster* obj) const ;
    
    int & GetNbOfHits() { return fNbOfHits ;  }
    
    std::string GetPlane()     {return fPlane;}
    void SetPlane(std::string planename) {fPlane = planename;}
    
    int GetNbAPVsFromPlane() {return fNbAPVsOnPlane;}
    void SetNbAPVsFromPlane(int nb)    {fNbAPVsOnPlane = nb;}
   
    float GetPlaneSize()     {return fPlaneSize;}
    void SetPlaneSize(float planesize) {fPlaneSize = planesize;}
    
    float & GetClusterPosition()     {return fposition;}
    float & GetClusterCentralStrip() {return fclusterCentralStrip;}
    
    int GetClusterTimeBin()  ;
    int GetClusterPeakTimeBin() {return fClusterPeakTimeBin ;}
    
    float GetClusterADCs() ;
    void SetClusterADCs(float adc) {fClusterSumADCs = adc;}
    
    void Dump() ;
    void ClearArrayOfHits();
    bool IsGoodCluster() ;
    
    void ClusterCentralStrip();
    void ClusterPositionHistoMean() ;
    //    void ClusterPositionGausFitMean() ;
    void ClusterPositionPulseHeghtWeight() ;
    void GetClusterPositionCorrectionHisto() ;
    void GetClusterPositionAfterCorrection() ;
    std::vector< float > GetClusterTimeBinADCs () {return fClusterTimeBinADCs; } ;
    
    //    void ComputeClusterPositionWithCorrection() ;
    void ComputeClusterPositionWithoutCorrection() ;
    
    //    void ComputeClusterPositionWithCorrection(const char * filename) ;
    void SetClusterPositionCorrection(bool isCluserPosCorrection) { fIsCluserPosCorrection = isCluserPosCorrection;}
    
private:
    
    int         fNbOfHits;     // numbers of strips with hit
    std::vector<SRSHit*> fArrayOfHits;  // APV hits table
    int fClusterPeakTimeBin, fClusterTimeBin;
    float fClusterPeakADCs, fClusterTimeBinADC, fClusterSumADCs, fposition, fclusterCentralStrip, fstrip, fPlaneSize;
    int fapvID, fStripNo, fAbsoluteStripNo, fapvIndexOnPlane, fNbAPVsOnPlane, fMinClusterSize, fMaxClusterSize;
    std::string fIsClusterMaxOrSumADCs, fPlane;
    bool fIsGoodCluster, fIsCluserPosCorrection ;
    
    std::vector< float > fClusterTimeBinADCs ;
    
}; 
#endif
