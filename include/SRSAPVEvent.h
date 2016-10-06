#ifndef __SRSAPVEVENT__
#define __SRSAPVEVENT__
/*******************************************************************************
 *  AMORE FOR SRS - SRS                                                         *
 *  SRSAPVEvent                                                                 *
 *  SRS Module Class                                                            *
 *  Author: Kondo GNANVO 18/08/2010                                             *
 *  Modified by Stefano Colafranceschi on 22/08/2015 to include ZS rawfiles     *
 *******************************************************************************/

#include <map>
#include <list>
#include <vector>
#include <iostream>
/*
#include "TList.h"
#include "TRandom.h"
#include "TObject.h"
#include "TMath.h"
#include "TObjArray.h"
*/
#include "SRSHit.h"
#include "SRSMapping.h"

#define NCH 128


class SRSAPVEvent {
    
public:
    
    SRSAPVEvent(int fec_no, int fec_channel, int apv_id, int sigmaLevel, int packet_size) ;
    ~SRSAPVEvent() ;
    
    void Add32BitsRawData(unsigned int rawdata32bits);
    void ComputeTimeBinCommonMode() ;
    
    void Set32BitsRawData(std::vector<unsigned int> rawdata32bits);
    std::vector<unsigned int> GetRawData32bits() {return fRawData32bits;}
    
    void ComputeRawData16bits();
    void ComputeRawData16bitsZS();
    
    std::vector<unsigned int> GetRawData16bits() {return fRawData16bits ;}
    
    
    std::multimap<int, float> GetTimeBinMap() {return fapvTimeBinDataMap ;}
    
    void ComputeMeanTimeBinPedestalData() ;
    std::vector<float> GetPedestalData() {return fPedestalData ;}
    
    void ComputeMeanTimeBinRawPedestalData() ;
    std::vector<float> GetRawPedestalData() {return fRawPedestalData ;}
    std::list <SRSHit * > ComputeListOfAPVHits() ;
    std::list <SRSHit * > ComputeListOfAPVHitsZS() ;
    
    
    int APVchannelMapping(int chNo) ;
    int StandardMapping(int) ;
    int EICStripMapping(int) ;
    int CMSStripMapping(int) ;
    int CMSStripMapping1(int) ;
    int CMSStripMapping2(int) ;
    int CMSStripMapping3(int) ;
    int CMSStripMapping4(int) ;
    int HMSStripMapping(int) ;
    int NS2StripMapping(int) ;
    //    int ZigZagStripMapping(int) ;
    int MMStripMappingAPV1(int) ;
    int MMStripMappingAPV2(int) ;
    int MMStripMappingAPV3(int) ;
    
    int StripMapping(int chNo) ;
    int APVchannelCorrection(int chNo) ;
    
    void SetZeroSupCut(int sigmalevel)     {fZeroSupCut      = sigmalevel;}
    void SetAPVHeaderLevel(int level)      {fAPVHeaderLevel  = level ;}
    void SetAPVIndexOnPlane(int index)     {fAPVIndexOnPlane = index ;}
    
    void Print() ;
    void Clear() ;
    
    void SetFECNo(int fecNo) {fFECNo = fecNo ;}
    int GetFECNo()           {return fFECNo;}
    
    void SetAPVID(int apvid) {fAPVID = apvid ;}
    int GetAPVID()           {return fAPVID;}
    
    void SetAPVKey(int apvkey) {fAPVKey = apvkey ;}
    int GetAPVKey()            {return fAPVKey;}
    
    void SetAPV(std::string apv) {fAPV = apv;}
    std::string GetAPV()             {return fAPV;}
    
    void SetAPVGain(float apvgain) {fAPVGain = apvgain ;}
    float GetAPVGain()             {return fAPVGain;}
    
    void SetADCChannel(int fecChannel) {fADCChannel = fecChannel ;}
    int GetADCChannel()                {return fADCChannel;}
    
    int GetAPVHeaderLevel()          {return fAPVHeaderLevel;}
    int GetAPVIndexOnPlane()         {return fAPVIndexOnPlane;}
    
    void SetAPVOrientation(int orient) {fAPVOrientation = orient;}
    int GetAPVOrientation()            {return fAPVOrientation;}

    void SetAPVstripmapping(int stripmapping) {fAPVstripmapping = stripmapping;}
    int GetAPVstripmapping()            {return fAPVstripmapping;}

    void SetPacketSize(int size)   {fPacketSize = size;}
    int GetPacketSize()            {return fPacketSize;}
    
    void SetNbOfAPVsFromPlane(int nb) {fNbOfAPVsFromPlane = nb;}
    int GetNbOfAPVsFromPlane()        {return fNbOfAPVsFromPlane;}
    
    void SetPlaneSize(float planesize) {fPlaneSize = planesize;}
    float GetPlaneSize()               {return fPlaneSize;}
    
    void SetPlane(std::string plane) {fPlane = plane;}
    std::string GetPlane()           {return fPlane;}
    
    void SetPedSubFlag(bool pedSub)         {fPedSubFlag = pedSub;}
    void SetCommonModeFlag(bool commonmode) {fCommonModeFlag = commonmode;}
    
    void SetAllFlags(bool pedSub, bool commonmode) {
        SetPedSubFlag(pedSub) ;
        SetCommonModeFlag(commonmode) ;
    }
    
    void SetPedestals(std::vector<float> noises, std::vector<float> offsets, std::vector<float> maskChs) {
        fMaskedChannels = maskChs;
        fPedestalNoises = noises ;
        fPedestalOffsets = offsets ;
    }
    
    void SetRawPedestals(std::vector<float> noises, std::vector<float> offsets) {
        fRawPedestalNoises = noises ;
        fRawPedestalOffsets = offsets ;
    }
    
    float GetMeanAPVnoise() { return fMeanAPVnoise ;}
    
    
    void SetHitMaxOrTotalADCs(std::string isHitMaxOrTotalADCs) {fIsHitMaxOrTotalADCs = isHitMaxOrTotalADCs ;}
    std::string GetHitMaxOrTotalADCs() {return fIsHitMaxOrTotalADCs;}
    
private:
    
    int fFECNo, fADCChannel, fAPVID, fAPVKey, fZeroSupCut, fNbCluster ;
    int fAPVIndexOnPlane, fAPVOrientation, fAPVstripmapping, fNbOfAPVsFromPlane, fAPVHeaderLevel, fPacketSize;
    float fPlaneSize, fEtaSectorPos, fAPVGain, fMeanAPVnoise;
    bool fCommonModeFlag, fPedSubFlag, fIsCosmicRunFlag, fIsPedestalRunFlag, fIsRawPedestalRunFlag, fAPVGainFlag;
    std::string fAPV, fPlane, fDetector, fDetectorType, fReadoutBoard, fIsHitMaxOrTotalADCs ;
    
    float fTrapezoidDetLength, fTrapezoidDetOuterRadius, fTrapezoidDetInnerRadius;
    
    std::vector<unsigned int> fRawData16bits, fRawData32bits;
    std::multimap<int, float> fapvTimeBinDataMap ;
    std::vector<float> fPedestalData, fRawPedestalData, fPedestalOffsets, fRawPedestalOffsets,fPedestalNoises, fRawPedestalNoises, fMaskedChannels;
    
    std::vector<float>  fPadDetectorMap, fCommonModeOffsets, fCommonModeOffsets_odd,   fCommonModeOffsets_even ;
    
    std::map <int, int> fapvChToPadChMap ;
    
};

#endif
