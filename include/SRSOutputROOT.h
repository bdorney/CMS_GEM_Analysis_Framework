#ifndef __SRSOUTPUTROOT__
#define __SRSOUTPUTROOT__
/*******************************************************************************
 *  AMORE FOR SRS - SRS                                                         *
 *  SRSOutputROOT                                                                    *
 *  SRS Module Class                                                            *
 *  Author: Kondo GNANVO, Mike Staib 01/05/2011                                 *
 *******************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include "TFile.h"
#include "TTree.h"

//#include "TObject.h"
//#include "TString.h"

#include "SRSHit.h"
#include "SRSMapping.h"
#include "SRSConfiguration.h"
#include "SRSEventBuilder.h"



class SRSOutputROOT {
    
public:
    SRSOutputROOT();
    SRSOutputROOT(const std::string& zeroSupCut,const std::string& rootdatatype);
    ~SRSOutputROOT();
    
    void InitRootFile() ;
    void WriteRootFile() ;

    void FillRootFile(SRSEventBuilder * eventbuilder);
    void FillHitsTree(SRSEventBuilder * eventbuilder) ;
    void FillClustersTree(SRSEventBuilder * eventbuilder) ;
    
    void SetRunName(std::string runname)     {fRunName = runname;}
    void SetRunType(std::string runtype)     {fRunType = runtype;}
    void SetROOTDataType(const std::string& rootdatatype){ fROOTDataType = rootdatatype ; }
    void SetZeroSupCut(Int_t zeroSupCut) {fZeroSupCut = zeroSupCut;}
    
    
    void DeleteHitsTree() ;
    void DeleteClustersTree() ;
    
private:
    
    int atoi(const std::string& a);
    TFile * fFile ;
    TTree * fHitTree ;
    TTree * fClusterTree ;

    std::string fRunType, fRunName;
    std::string fROOTDataType;
    
    int m_evtID, m_chID, m_nclust, fZeroSupCut;            //
    
    float * m_planeID;      // Plane Number
    int * m_strip;        // Strip Number
    int * m_detID;        // Detector Number
    //short * m_etaSector;  // Detector Number
    
    float * m_hit_planeID;     // Plane Number
    int * m_hit_detID;       // Detector Number
    int * m_hit_timeBin;     // time sample of the strip with maximum ADCs
    //short * m_hit_etaSector; // Detector Number
    
    float * m_clustPos;    // Cluster position
    float * m_clustADCs;   // cluster charge
    
    int   * m_clustSize;   // Cluster position
    int * m_clustTimeBin; // time sample of the strip with maximum ADCs
    
    float * m_REF1;
    float * m_REF2;
    float * m_REF3;
    float * m_REF4;
    float * m_SBS1;
    float * m_UVAEIC;
    
    short * m_adc0;     //ADC value for 1st time sample
    short * m_adc1;     //ADC value for 2nd time sample
    short * m_adc2;     //ADC value for 3rd time sample
    short * m_adc3;     //ADC value for 4th time sample
    short * m_adc4;     //ADC value for 5th time sample
    short * m_adc5;     //ADC value for 6th time sample
    short * m_adc6;     //ADC value for 7th time sample
    short * m_adc7;     //ADC value for 8th time sample
    short * m_adc8;     //ADC value for 9th time sample
    short * m_adc9;     //ADC value for 10th time sample
    short * m_adc10;     //ADC value for 11th time sample
    short * m_adc11;     //ADC value for 12th time sample
    short * m_adc12;     //ADC value for 13th time sample
    short * m_adc13;     //ADC value for 14th time sample
    short * m_adc14;     //ADC value for 15th time sample
    short * m_adc15;     //ADC value for 16th time sample
    short * m_adc16;     //ADC value for 17th time sample
    short * m_adc17;     //ADC value for 18th time sample
    short * m_adc18;     //ADC value for 19th time sample
    short * m_adc19;     //ADC value for 20th time sample
    short * m_adc20;     //ADC value for 21th time sample
    short * m_adc21;     //ADC value for 22th time sample
    short * m_adc22;     //ADC value for 23th time sample
    short * m_adc23;     //ADC value for 24th time sample
    short * m_adc24;     //ADC value for 25th time sample
    short * m_adc25;     //ADC value for 26th time sample
    short * m_adc26;     //ADC value for 27th time sample
    short * m_adc27;     //ADC value for 28th time sample
    short * m_adc28;     //ADC value for 29th time sample
    short * m_adc29;     //ADC value for 30th time sample    
};

#endif

