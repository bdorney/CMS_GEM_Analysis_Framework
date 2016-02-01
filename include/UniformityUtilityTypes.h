//
//  GainUniformityTypes.h
//  
//
//  Created by Brian L Dorney on 20/01/16.
//
//

#ifndef _UniformityUtilityTypes_h
#define _UniformityUtilityTypes_h

//C++ Includes
#include <map>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//My Includes

//ROOT Includes
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TTree.h"

using namespace ROOT;

namespace Uniformity {
    //Storage container for cluster selection parameters
    struct SelParamClusters{
        //Cut on cluster adc value
        //Cluster::iADC greater than this number
        int iCut_ADCNoise;
        
        //Cut on cluster multiplicity per event
        int iCut_MultiMin;
        int iCut_MultiMax;
        
        //Cut on cluster size
        //first -> Min Size; Cluster::iSize greater than this number
        //second -> Max Size; Cluster::iSize less than this number
        //std::pair<int,int> pair_iCut_Size;
        int iCut_SizeMin;
        int iCut_SizeMax;
        
        //Cut on cluster time bin
        //first -> min time; Cluster::iTimeBin greater than this number
        //second -> max time; Cluster::iTimeBin less than this number
        //std::pair<int,int> pair_iCut_Time;
        int iCut_TimeMin;
        int iCut_TimeMax;
        
        //Default Values
        SelParamClusters(){ //SelParamClusters Inital Values
            iCut_ADCNoise = -1;
            
            iCut_MultiMin = 0;
            
            iCut_SizeMin = iCut_TimeMin = -1;
            iCut_SizeMax = iCut_TimeMax = iCut_MultiMax = 3072;
            
            //pair_iCut_Size = std::make_pair(1,20);
            //pair_iCut_Time = std::make_pair(1,14);
        } //End SelParamClusters Initial Values
    }; //End SelParamClusters
    
    //Analysis Setup
    struct AnalysisSetupUniformity{
        int iUniformityGranularity; //Each iEta sector is partitioned into this many slices
        
        SelParamClusters selClust; //Selection Criteria for Clusters
        
        std::string strFit_Eqn;     //Fit equation, e.g. "[0]*x+[1]"
        std::string strFit_Option;  //Fit Option, e.g. "R"
    }; //End AnalysisSetupUniformity
    
    //Strip Cluster
    struct Cluster{
        //Int_t *fPos_Y;  //distance from base of trapezoid (in mm); e.g. planeID from AMORE
        int iPos_Y;  //distance from base of trapezoid (in mm); e.g. planeID from AMORE
        //Float_t *fPos_X;  //position within eta sector (in mm); e.g. clustPos from AMORE
        float fPos_X;  //position within eta sector (in mm); e.g. clustPos from AMORE
        
        //Float_t *fADC;       //ADC value of cluster; e.g. clustADCs from AMORE
        float fADC;       //ADC value of cluster; e.g. clustADCs from AMORE
        
        //Int_t *fSize;      //Number of strips in cluster; e.g. clustSize from AMORE
        int iSize;      //Number of strips in cluster; e.g. clustSize from AMORE
        
        //Int_t *fTimeBin;   //Time bin of cluster (not sure what that physically means...); e.g. clustTimeBin from AMORE
        int iTimeBin;   //Time bin of cluster (not sure what that physically means...); e.g. clustTimeBin from AMORE
        
        //Set Initial Values
        Cluster(){
            //fPos_Y = fPos_X = -1;
            
            //fADC = fSize = fTimeBin = -1;
            
            iPos_Y = iSize = iTimeBin = -1;
            
            fPos_X = fADC = -1.;
        } //End initialization
    }; //End Cluster
    
    //Defines a slice of a phi sector within the detector
    struct SectorSlice{
        //One dimensional histograms
        std::shared_ptr<TH1F> hSlice_ClustADC;
    };
    
    //Defines the phi sector within the detector
    struct SectorPhi{
        float fPos_Xlow;    //Lower Bound X Position
        float fPos_Xhigh;   //Upper Bound X Position
        
        std::map<int, SectorSlice> map_slices;  //Slices of this sector
        
        std::vector<Cluster> vec_clusters;
        
        //One dimensional histograms
        std::shared_ptr<TH1F> hPhi_ClustADC;
        std::shared_ptr<TH1F> hPhi_ClustMulti;
        std::shared_ptr<TH1F> hPhi_ClustSize;
        
        //Two dimensional histograms
        std::shared_ptr<TH2F> hPhi_ClustADC_v_ClustPos; //ADC vs Position for all clusters in this eta sector
        
        //initialization
        SectorPhi(){
            fPos_Xlow = fPos_Xhigh = -1;
        } //End initialization
    };
    
    //Defines the pseudorapidity sector within the detector (note this is considered 3 phi sectors)
    struct SectorEta{
        float fPos_Y;       //Roughly Central Y Position of sector
        float fWidth;       //Width of detector at this iEta value
        
        std::map<int, SectorPhi> map_sectorsPhi;
        
        //One dimensional histograms
        std::shared_ptr<TH1F> hEta_ClustADC;    //ADC spectrum for all clusters in this eta sector
        std::shared_ptr<TH1F> hEta_ClustMulti;  //Multiplicity  "                               "
        std::shared_ptr<TH1F> hEta_ClustPos;    //Position      "                               "
        std::shared_ptr<TH1F> hEta_ClustSize;   //Size          "                               "
        
        //Two dimensional histograms
        std::shared_ptr<TH2F> hEta_ClustADC_v_ClustPos; //ADC vs Position for all clusters in this eta sector
        
        //initialization
        SectorEta(){
            fPos_Y = fWidth = -1;
        } //End initialization
    }; //End SectorEta
    
} //End namespace uniformity

#endif
