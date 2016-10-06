#ifndef SRSHIT_H
#define SRSHIT_H
/*******************************************************************************
 *  AMORE FOR SRS - SRS                                                         *
 *  SRSHit                                                                      *
 *  SRS Module Class                                                            *
 *  Author: Kondo GNANVO 18/08/2010                                             *
 *******************************************************************************/

//#include "SRSAPVSignalFit.h"

//#include <map>
//#include <stdlib.h>
#include <vector>
#include <iostream>
//#include "TList.h"
//#include "TObject.h"
//#include "TMath.h"
//#include "TGraph.h"
//#include "TVector.h"
//#include <stdlib.h>

#define NCH 128
class SRSHit {
    
public:
    
    ~SRSHit() ;
    SRSHit() ;
    
    bool IsSortable() const { return true; }
    
    void IsHitFlag(bool hitOrNoise) { fIsHit = hitOrNoise ;}
    bool IsHit() {return fIsHit ;}
    
    void Timing() ;
    
    //=== Sort hit according to the strip number
    int Compare(const SRSHit *obj) const { return (fStripNo > obj->GetStripNo() ? 1 : -1); }
    
    void ComputePosition() ;
    void AddTimeBinADCs(float charges) {fTimeBinADCs.push_back(charges);}
    
    void SetTimeBinADCs(std::vector<float> timebinCharges) {
        fTimeBinADCs.clear() ;
        fTimeBinADCs = timebinCharges ;
    }
    
    void SetPadDetectorMap(std::vector<float> padDetectorMap) {
        fPadDetectorMap.clear() ;
        fPadDetectorMap = padDetectorMap ;
        fPadDetectorMap.resize(5) ;
    }
    
    void ClearTimeBinADCs() {fTimeBinADCs.clear() ;}
    
    std::vector<float> GetTimeBinADCs() { return fTimeBinADCs ;}
    
    int GetAPVID()            {return fapvID;}
    void SetAPVID(int apvID) {fapvID = apvID;}
    void SetHitADCs(int sigmaLevel, float charges, std::string isHitMaxOrTotalADCs) ;
    float GetHitADCs()   {return fHitADCs;}
    
    int GetSignalPeakBinNumber()   {
      this->Timing() ;
      return fSignalPeakBinNumber;
    }
    
    std::string GetHitMaxOrTotalADCs() { return fIsHitMaxOrTotalADCs ;}
    
    int GetAPVIndexOnPlane()            {return fapvIndexOnPlane;}
    void  SetAPVIndexOnPlane(int index) {fapvIndexOnPlane = index;}
    
    int GetNbAPVsFromPlane()         {return fNbAPVsOnPlane;}
    void  SetNbAPVsFromPlane(int nb) {fNbAPVsOnPlane = nb;}
    
    int GetAPVOrientation()         {return fAPVOrientation;}
    void  SetAPVOrientation(int nb) {fAPVOrientation = nb;}

    int GetAPVstripmapping()         {return fAPVstripmapping;}
    void  SetAPVstripmapping(int nb) {fAPVstripmapping = nb;}

    float GetPlaneSize() {return fPlaneSize;}
    void SetPlaneSize(float planesize) {fPlaneSize = planesize;}
    
    float GetTrapezoidDetInnerRadius() {return fTrapezoidDetInnerRadius;}
    float GetTrapezoidDetOuterRadius() {return fTrapezoidDetOuterRadius;}
    
    void SetTrapezoidDetRadius(float innerRadius, float outerRadius) {
        fTrapezoidDetInnerRadius = innerRadius ;
        fTrapezoidDetOuterRadius = outerRadius ;
    }
    
    
    std::string GetPlane() {return fPlane;}
    void SetPlane(std::string plane) {fPlane = plane;}
    
    std::string GetDetector() {return fDetector;}
    void SetDetector(std::string detector) {fDetector = detector;}
    
    std::string GetReadoutBoard() {return fReadoutBoard;}
    void SetReadoutBoard(std::string readoutBoard) {fReadoutBoard = readoutBoard;}
    
    std::string GetDetectorType() {return fDetectorType;}
    void SetDetectorType(std::string detectorType) {fDetectorType = detectorType;}
    
    void  SetStripNo(int stripNo) ;
    int GetStripNo() const        {return fStripNo;}
    int GetAbsoluteStripNo() {return fAbsoluteStripNo;}
    
    void  SetPadNo(int padno)  {fPadNo = padno;}
    int GetPadNo() {return fPadNo;}
    
    float GetStripPosition() {
      this->ComputePosition() ;
      return fStripPosition;
    }
    
    std::vector<float> GetPadPosition() {
        ComputePosition() ;
        return fPadPosition ;
    }
    
private:
    
    bool fIsHit ;
    int fapvID, fStripNo, fPadNo, fAbsoluteStripNo, fapvIndexOnPlane, fNbAPVsOnPlane, fAPVOrientation, fAPVstripmapping, fSignalPeakBinNumber;
    float fHitADCs, fStripPosition, fPlaneSize, fTrapezoidDetInnerRadius, fTrapezoidDetOuterRadius;
    std::string fPlane, fReadoutBoard, fDetectorType, fDetector, fIsHitMaxOrTotalADCs;
    
    std::vector<float> fTimeBinADCs, fPadDetectorMap, fPadPosition  ;
};

#endif
