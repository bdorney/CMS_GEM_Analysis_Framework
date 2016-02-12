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

//Framework Includes

//ROOT Includes
//#include "FitResult.h"
#include "TFile.h"
//#include "TFitResultPtr.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TH2F.h"
//#include "TPolyMarker.h"
#include "TROOT.h"
//#include "TSpectrum.h"
//#include "TTree.h"

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
        int iEvt_First;  //Starting Event For Analysis
        int iEvt_Total;    //Total number of Events to Analyze
        int iUniformityGranularity; //Each iEta sector is partitioned into this many slices
        
        float fUniformityTolerance; //Detector should be uniform within this percent
        
        Timing::HistoSetup histoSetup_clustADC;
        Timing::HistoSetup histoSetup_clustMulti;
        Timing::HistoSetup histoSetup_clustPos;
        Timing::HistoSetup histoSetup_clustSize;
        Timing::HistoSetup histoSetup_clustTime;
        
        SelParamClusters selClust; //Selection Criteria for Clusters
        
        //Soon to be depreciated
        //std::string strFit_Eqn;     //Fit equation, e.g. "[0]*x+[1]"
        //std::string strFit_Option;  //Fit Option, e.g. "R"
        
        //Initialization
        AnalysisSetupUniformity(){
            iEvt_First =  0;
            iEvt_Total = -1;
            iUniformityGranularity = 4;
            
            fUniformityTolerance = 0.15;
            
            //strFit_Eqn = "[0]*exp(-0.5*(x-[1])^2/[2]^2)";
            //strFit_Eqn = "gaus";
            //strFit_Option = "QMR";
        } //End Initialization
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
        float fPos_Center;    //Center of the slice;
        float fWidth;       //Width of the slice;
        
        //One dimensional fits
        std::shared_ptr<TF1> fitSlice_ClustADC;
        
        //One dimensional histograms
        std::shared_ptr<TH1F> hSlice_ClustADC;
        
        //Spectral analysis
        //std::shared_ptr<TSpectrum> specSlice_ClustADC;
        //std::shared_ptr<TPolyMarker> pmrkSlice_ClustADC;
        //TPolyMarker *pmrkSlice_ClustADC;
        
        //Initialization
        SectorSlice(){
            fPos_Center = fWidth = -1;
        } //End Initialization
    };
    
    //Defines the phi sector within the detector
    struct SectorPhi{
        float fPos_Xlow;    //Lower Bound X Position
        float fPos_Xhigh;   //Upper Bound X Position
        
        float fWidth;       //Sector Width (mm)
        
        std::map<int, SectorSlice> map_slices;  //Slices of this sector
        
        std::vector<Cluster> vec_clusters;
        
        //One dimensional histograms
        std::shared_ptr<TH1F> hPhi_ClustADC;
        std::shared_ptr<TH1F> hPhi_ClustMulti;
        std::shared_ptr<TH1F> hPhi_ClustSize;
        std::shared_ptr<TH1F> hPhi_ClustTime;
        
        //Two dimensional histograms
        std::shared_ptr<TH2F> hPhi_ClustADC_v_ClustPos; //ADC vs Position for all clusters in this eta sector
        
        //initialization
        SectorPhi(){
            fPos_Xlow = fPos_Xhigh = fWidth = -1;
        } //End initialization
    };
    
    //Defines the pseudorapidity sector within the detector (note this is considered 3 phi sectors)
    struct SectorEta{
        float fPos_Y;       //Roughly Central Y Position of sector
        float fWidth;       //Width of detector at this iEta value
        
        std::map<int, SectorPhi> map_sectorsPhi;
        
        //One dimensional graphs
        std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_NormChi2;
        std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_PkPos;
        
        std::shared_ptr<TGraphErrors> gEta_ClustADC_Spec_NumPks;
        std::shared_ptr<TGraphErrors> gEta_ClustADC_Spec_PkPos;
        
        //One dimensional histograms
        std::shared_ptr<TH1F> hEta_ClustADC;    //ADC spectrum for all clusters in this eta sector
        std::shared_ptr<TH1F> hEta_ClustMulti;  //Multiplicity  "                               "
        std::shared_ptr<TH1F> hEta_ClustPos;    //Position      "                               "
        std::shared_ptr<TH1F> hEta_ClustSize;   //Size          "                               "
        std::shared_ptr<TH1F> hEta_ClustTime;   //Time          "                                   "
        
        //Two dimensional histograms
        std::shared_ptr<TH2F> hEta_ClustADC_v_ClustPos; //ADC vs Position for all clusters in this eta sector
        
        //initialization
        SectorEta(){
            fPos_Y = fWidth = -1;
        } //End initialization
    }; //End SectorEta
    
} //End namespace uniformity

#endif
