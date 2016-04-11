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
#include <set>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
#include "TimingUtilityTypes.h"

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
    struct SelParam{
        //Cut on cluster/hit adc value
        int iCut_ADCNoise;  //Cluster::fADC or Hit::fADC[i] greater than this number
        int iCut_ADCSat;    //Hit::fADC[i] lower than this number (not applied to clusters)
        
        //Cut on cluster/hit multiplicity per event
        int iCut_MultiMin;
        int iCut_MultiMax;
        
        //Cut on cluster size (not used for hits)
        int iCut_SizeMin;
        int iCut_SizeMax;
        
        //Cut on cluster/hit time bin
        int iCut_TimeMin;
        int iCut_TimeMax;
        
        //Default Values
        SelParam(){ //SelParamClusters Inital Values
            iCut_ADCNoise = -1;
            iCut_ADCSat = 1600;
            
            iCut_MultiMin = 0;
            
            iCut_SizeMin = iCut_TimeMin = -1;
            iCut_SizeMax = iCut_TimeMax = iCut_MultiMax = 3072;
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
        
        Timing::HistoSetup histoSetup_hitPos;
        Timing::HistoSetup histoSetup_hitTime;
        
        SelParam selClust;  //Selection Criteria for Clusters
        SelParam selHit;    //Selection Criteria for Hits
        
        //Initialization
        AnalysisSetupUniformity(){
            iEvt_First =  0;
            iEvt_Total = -1;
            iUniformityGranularity = 4;
            
            fUniformityTolerance = 0.15;
        } //End Initialization
    }; //End AnalysisSetupUniformity
    
    struct Hit{
        int iPos_Y; //distance from base of trapezoid (in mm); e.g. planeID from amoreSRS
        
        int iStripNum;  //Strip number; e.g. strip from amoreSRS in range [0,384)?
        
        int iTimeBin;   //Time bin with the maximum ADC value; e.g. hitTimebin from amoreSRS in range [1,30]? Corresponds to tree adcX where X is an integer iTimeBin - 1;
        
        //For now ADC is not used
        //float fADC[30];           //ADC value of hit for time bin i; e.g. fADC[iTimeBine] gives the adc value at the assigned time bin
        std::vector<float> vec_fADC;//ADC value of hit for time bin i; e.g. vec_fADC[iTimeBine] gives the adc value at the assigned time bin
        
        //Set Initial Values
        Hit(){
            iPos_Y = iStripNum = iTimeBin = -1;
        } //End Initialization
    }; //End Hit
    
    //Cluster of strips
    struct Cluster{
        //Int_t *fPos_Y;  //distance from base of trapezoid (in mm); e.g. planeID from amoreSRS
        int iPos_Y;  //distance from base of trapezoid (in mm); e.g. planeID from amoreSRS
        //Float_t *fPos_X;  //position within eta sector (in mm); e.g. clustPos from amoreSRS
        float fPos_X;  //position within eta sector (in mm); e.g. clustPos from amoreSRS
        
        //Float_t *fADC;       //ADC value of cluster; e.g. clustADCs from amoreSRS
        float fADC;       //ADC value of cluster; e.g. clustADCs from amoreSRS
        
        //Int_t *fSize;      //Number of strips in cluster; e.g. clustSize from amoreSRS
        int iSize;      //Number of strips in cluster; e.g. clustSize from amoreSRS
        
        //Int_t *fTimeBin;   //Time bin of cluster (not sure what that physically means...); e.g. clustTimeBin from amoreSRS
        int iTimeBin;   //Time bin of cluster (not sure what that physically means...); e.g. clustTimeBin from amoreSRS
        
        //Set Initial Values
        Cluster(){
            //fPos_Y = fPos_X = -1;
            
            //fADC = fSize = fTimeBin = -1;
            
            iPos_Y = iSize = iTimeBin = -1;
            
            fPos_X = fADC = -1.;
        } //End initialization
    }; //End Cluster
    
    //Cluster Histograms
    struct HistosPhysObj{
        //One dimensional histograms
        std::shared_ptr<TH1F> hADC;    //ADC spectrum for some physics object
        std::shared_ptr<TH1F> hMulti;  //Multiplicity  "                    "
        std::shared_ptr<TH1F> hPos;    //Position      "                    "
        std::shared_ptr<TH1F> hSize;   //Size          "                    "
        std::shared_ptr<TH1F> hTime;   //Time          "                    "
        
        //Two dimensional histograms
        std::shared_ptr<TH2F> hADC_v_Pos; //ADC vs Position for all physics objects
        //std::shared_ptr<TH2F> hPos_v_Sector; //Position vs. Sector No. for all physics objects
    };
    
    //Summary Statistics
    struct SummaryStatistics{
        //Statistics from Fit
        float fIQR;      //Inter-quantile range (IQR = Q3 - Q1);
        float fMax;      //Max response;
        float fMean;     //Mean response;
        float fMin;      //Min response;
        float fQ1;       //First Quantile (Q1)
        float fQ2;       //Second Quantile (Q2, e.g. median)
        float fQ3;       //Third Quantile (Q3)
        float fStdDev;   //Standard Deviation
        
        //std::vector<float> vec_fOutliers;
        std::multiset<float> mset_fOutliers;
        
        std::shared_ptr<TH1F> hDist;  //Distribution of Dataset
        
        //Initialize
        SummaryStatistics(){
            fIQR = fMax = fMean = fMin = -1;
            fQ1 = fQ2 = fQ3 = -1;
            fStdDev = -1;
        }
    }; //End SummaryStatistics
    
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
        
        int iStripNum_Min;  //Minimum Strip number of sector (inclusive)
        int iStripNum_Max;  //Maximum Strip number of sector (exclusive)
        
        std::map<int, SectorSlice> map_slices;  //Slices of this sector
        
        //std::pair<int,int> pair_iStripRange;  //Strip number range covered by sector [first,second)
        
        std::vector<Hit> vec_hits;
        std::vector<Cluster> vec_clusters;
        
        //Histograms
        HistosPhysObj clustHistos;
        HistosPhysObj hitHistos;
        
        //initialization
        SectorPhi(){
            fPos_Xlow = fPos_Xhigh = fWidth = -1;
            
            //pair_iStripRange = std::make_pair(-2,-1);
        } //End initialization
    };
    
    //Defines the pseudorapidity sector within the detector (note this is considered 3 phi sectors)
    struct SectorEta{
        float fPos_Y;       //Roughly Central Y Position of sector
        float fWidth;       //Width of detector at this iEta value
        
        std::map<int, SectorPhi> map_sectorsPhi;
        
        std::multiset<float> mset_fClustADC_Fit_PkPos;
        //std::vector<float> vec_fClustADC_Fit_PkWidth;   //Energy resolution info, from Fit
        
        std::multiset<float> mset_fClustADC_Spec_PkPos;
        
        //One dimensional graphs
        std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_NormChi2;
        std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_PkPos;
        std::shared_ptr<TGraphErrors> gEta_ClustADC_Fit_Failures;
        
        std::shared_ptr<TGraphErrors> gEta_ClustADC_Spec_NumPks;
        std::shared_ptr<TGraphErrors> gEta_ClustADC_Spec_PkPos;
        
        //histograms
        HistosPhysObj clustHistos;
        HistosPhysObj hitHistos;
        
        //Summary Statistics
        SummaryStatistics statClustADC_Fit_PkPos;
        SummaryStatistics statClustADC_Spec_PkPos;
        
        //initialization
        SectorEta(){
            fPos_Y = fWidth = -1;
        } //End initialization
    }; //End SectorEta
    
} //End namespace uniformity

#endif
