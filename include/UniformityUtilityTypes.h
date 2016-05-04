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
        
        Timing::HistoSetup histoSetup_hitADC;
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
    
    //Run Setup
    struct RunSetup{
        bool bAnaStep_Clusters;         //true -> perform the cluster analysis; false -> do not
        bool bAnaStep_Fitting;          //true -> run fitting on output histo's; false -> do not
        bool bAnaStep_Hits;             //true -> perform the hit analysis (NOTE if bAnaStep_Reco is true this must also be true); false -> do not
        bool bAnaStep_Reco;             //true -> reconstruct clusters from input hits; false -> use clusters provided in amoreSRS output file;
        bool bAnaStep_Visualize;        //true -> make summary plots at end of analysis; false -> do not
        
        bool bInputFromFrmwrk;          //true -> input file is a framework output file, not from amoreSRS; false -> input file(s) are from amoreSRS
        
        bool bLoadSuccess;
        
        bool bMultiOutput;              //true -> one output file per input run; false -> one output file representing the "sum" of the input runs
        bool bVisPlots_PhiLines;        //true -> summary plots have phi lines segmenting sectors; false -> they do not
        
        std::string strFile_Config_Ana;      //Name of analysis config file
        std::string strFile_Config_Map;      //Name of mapping file
        
        std::string strFile_Output_Name;     //Name of output TFile to be created
        std::string strFile_Output_Option;   //Option for TFile: CREATE, RECREATE, UPDATE, etc...
        
        //Default constructor
        RunSetup(){
            bAnaStep_Reco = false;
            bAnaStep_Clusters = bAnaStep_Fitting = bAnaStep_Hits = bAnaStep_Visualize = true;
            
            bInputFromFrmwrk = false;
            
            bLoadSuccess = false;   //This is set to true if the struct is loaded successfully
            
            bMultiOutput = false;
            
            bVisPlots_PhiLines = true;
            
            strFile_Config_Ana = "config/configAnalysis.cfg";
            strFile_Config_Map = "config/GE7MappingCMScernData2016.cfg";
            
            strFile_Output_Name = "FrameworkOutput.root";
            strFile_Output_Option = "RECREATE";
        } //End Default constructor
    }; //End RunSetup
    
    struct Hit{
        int iPos_Y; //distance from base of trapezoid (in mm); e.g. planeID from amoreSRS
        
        int iStripNum;  //Strip number; e.g. strip from amoreSRS in range [0,384)?
        
        int iTimeBin;   //Time bin with the maximum ADC value; e.g. hitTimebin from amoreSRS in range [1,30]? Corresponds to tree adcX where X is an integer iTimeBin - 1;
        
        //For now ADC is not used
        //float fADC[30];           //ADC value of hit for time bin i; e.g. fADC[iTimeBine] gives the adc value at the assigned time bin
        std::vector<short> vec_sADC;//ADC value of hit for time bin i; e.g. vec_fADC[iTimeBine] gives the adc value at the assigned time bin
        
        //Set Initial Values
        Hit(){
            iPos_Y = iStripNum = iTimeBin = -1;
            
            vec_sADC.resize(30);
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
        
        std::map<int, Hit> map_hits;    //Hits used to reconstruct this cluster; placeholder, key-val here is the hit's strip number iStripNum
        
        //Set Initial Values
        Cluster(){
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
    
        //Default Constructor
        HistosPhysObj(){
            
        } //End Default Constructor
        
        //Copy Constructor
        HistosPhysObj(const HistosPhysObj& other){
            //Deep copy
            if( other.hADC != NULL )    hADC    = std::make_shared<TH1F>( *other.hADC.get() );
            if( other.hMulti != NULL )  hMulti  = std::make_shared<TH1F>( *other.hMulti.get() );
            if( other.hPos != NULL )    hPos    = std::make_shared<TH1F>( *other.hPos.get() );
            if( other.hSize != NULL )   hSize   = std::make_shared<TH1F>( *other.hSize.get() );
            if( other.hTime != NULL )   hTime   = std::make_shared<TH1F>( *other.hTime.get() );
            
            if( other.hADC_v_Pos != NULL ) hADC_v_Pos = std::make_shared<TH2F>( *other.hADC_v_Pos.get() );
        } //End Copy Constructor
        
        //Assignment operator
        HistosPhysObj & operator=(const HistosPhysObj & other){
            if (this != &other ) { //Protect against invalid self-assignment
                //Deep copy
                if( other.hADC != NULL )    hADC    = std::make_shared<TH1F>( *other.hADC.get() );
                if( other.hMulti != NULL )  hMulti  = std::make_shared<TH1F>( *other.hMulti.get() );
                if( other.hPos != NULL )    hPos    = std::make_shared<TH1F>( *other.hPos.get() );
                if( other.hSize != NULL )   hSize   = std::make_shared<TH1F>( *other.hSize.get() );
                if( other.hTime != NULL )   hTime   = std::make_shared<TH1F>( *other.hTime.get() );
                
                if( other.hADC_v_Pos != NULL ) hADC_v_Pos = std::make_shared<TH2F>( *other.hADC_v_Pos.get() );
            } //Protects against invalid self-assignment
            
            return *this;
        } //End Assignment operator
    }; //End HistosPhysObj
    
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
        
        //Default Constructor
        SummaryStatistics(){
            fIQR = fMax = fMean = fMin = -1;
            fQ1 = fQ2 = fQ3 = -1;
            fStdDev = -1;
        } //End Default Constructor
        
        //Copy Constructor
        SummaryStatistics(const SummaryStatistics& other){
            fIQR    = other.fIQR;
            fMax    = other.fMax;
            fMean   = other.fMean;
            fMin    = other.fMin;
            fQ1     = other.fQ1;
            fQ2     = other.fQ2;
            fQ3     = other.fQ3;
            fStdDev = other.fStdDev;
            
            mset_fOutliers = other.mset_fOutliers;
            
            //Deep Copy
            if( other.hDist != NULL )   hDist   = std::make_shared<TH1F>( *other.hDist.get() );
        } //End Copy Constructor
        
        //Assignment Operator
        SummaryStatistics & operator=(const SummaryStatistics & other){
            if( this != &other ) { //Protects against invalid self-assignment
                fIQR    = other.fIQR;
                fMax    = other.fMax;
                fMean   = other.fMean;
                fMin    = other.fMin;
                fQ1     = other.fQ1;
                fQ2     = other.fQ2;
                fQ3     = other.fQ3;
                fStdDev = other.fStdDev;
                
                mset_fOutliers = other.mset_fOutliers;
                
                //Deep Copy
                if( other.hDist != NULL )   hDist   = std::make_shared<TH1F>( *other.hDist.get() );
            } //Protects against invalid self-assignment
            
            return *this;
        } //End Assignment Operator
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
        
        //Default Constructor
        SectorSlice(){
            fPos_Center = fWidth = -1;
        } //End Default Constructor
        
        //Copy Constructor
        SectorSlice(const SectorSlice& other){
            fPos_Center = other.fPos_Center;
            fWidth      = other.fWidth;
            
            //Deep copy
            if( other.fitSlice_ClustADC != NULL )   fitSlice_ClustADC   = std::make_shared<TF1>( *other.fitSlice_ClustADC.get() );
            if( other.hSlice_ClustADC != NULL )     hSlice_ClustADC     = std::make_shared<TH1F>( *other.hSlice_ClustADC.get() );
        } //End Copy Constructor
        
        //Assignment operator
        SectorSlice & operator=(const SectorSlice & other){
            if (this != &other ) { //Protects against invalid self-assignment
                fPos_Center = other.fPos_Center;
                fWidth      = other.fWidth;
                
                //Deep copy
                if( other.fitSlice_ClustADC != NULL )   fitSlice_ClustADC   = std::make_shared<TF1>( *other.fitSlice_ClustADC.get() );
                if( other.hSlice_ClustADC != NULL )     hSlice_ClustADC     = std::make_shared<TH1F>( *other.hSlice_ClustADC.get() );
            } //Protects against invalid self-assignment
            
            return *this;
        } //End Assignment operator
    }; //End SectorSlice
    
    //Defines the phi sector within the detector
    struct SectorPhi{
        float fPos_Xlow;    //Lower Bound X Position
        float fPos_Xhigh;   //Upper Bound X Position
        
        float fWidth;       //Sector Width (mm)
        
        int iStripNum_Min;  //Minimum Strip number of sector (inclusive)
        int iStripNum_Max;  //Maximum Strip number of sector (exclusive)
        
        std::map<int, SectorSlice> map_slices;  //Slices of this sector
        
        std::vector<Cluster> vec_clusters;
        std::vector<Hit> vec_hits;
        
        //Histograms
        HistosPhysObj clustHistos;
        HistosPhysObj hitHistos;
        
        //Default Constructor
        SectorPhi(){
            fPos_Xlow = fPos_Xhigh = fWidth = -1;
        } //End Default Constructor
        
        //Copy Constructor
        SectorPhi(const SectorPhi& other){
            fPos_Xlow   = other.fPos_Xlow;
            fPos_Xhigh  = other.fPos_Xhigh;
            
            fWidth = other.fWidth;
            
            iStripNum_Min = other.iStripNum_Min;
            iStripNum_Max = other.iStripNum_Max;
            
            map_slices = other.map_slices;
            
            vec_hits    = other.vec_hits;
            vec_clusters= other.vec_clusters;
            
            clustHistos = other.clustHistos;
            hitHistos   = other.hitHistos;
        } //End Copy Constructor
        
        //Assignment operator
        SectorPhi & operator=(const SectorPhi & other){
            if (this != &other ){ //Protects against invalid self-assignment
                fPos_Xlow  = other.fPos_Xlow;
                fPos_Xhigh = other.fPos_Xhigh;
                
                fWidth = other.fWidth;
                
                iStripNum_Min = other.iStripNum_Min;
                iStripNum_Max = other.iStripNum_Max;
                
                map_slices = other.map_slices;
                
                vec_hits    = other.vec_hits;
                vec_clusters= other.vec_clusters;
                
                clustHistos = other.clustHistos;
                hitHistos   = other.hitHistos;
            } //Protects against invalid self-assignment
            
            return *this;
        } //End Assignment Operator
    }; //End SectorPhi
    
    //Defines the pseudorapidity sector within the detector (note this is considered 3 phi sectors)
    struct SectorEta{
        float fPos_Y;       //Roughly Central Y Position of sector
        float fWidth;       //Width of detector at this iEta value
        
        std::map<int, SectorPhi> map_sectorsPhi;
        
        std::multiset<float> mset_fClustADC_Fit_PkPos;
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
        
        //Default Constructor
        SectorEta(){
            fPos_Y = fWidth = -1;
        } //End Default Constructor
        
        //Copy Constructor
        SectorEta(const SectorEta& other){
            fPos_Y = other.fPos_Y;
            fWidth = other.fWidth;
            
            map_sectorsPhi = other.map_sectorsPhi;
            
            mset_fClustADC_Fit_PkPos    = other.mset_fClustADC_Fit_PkPos;
            mset_fClustADC_Spec_PkPos   = other.mset_fClustADC_Spec_PkPos;
            
            //histograms
            clustHistos = other.clustHistos;
            hitHistos = other.hitHistos;
            
            //Summary Statistics
            statClustADC_Fit_PkPos  = other.statClustADC_Fit_PkPos;
            statClustADC_Spec_PkPos = other.statClustADC_Spec_PkPos;
            
            //Deep Copy
            if( other.gEta_ClustADC_Fit_NormChi2 != NULL )  gEta_ClustADC_Fit_NormChi2  = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Fit_NormChi2.get() );
            if( other.gEta_ClustADC_Fit_PkPos != NULL )     gEta_ClustADC_Fit_PkPos     = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Fit_PkPos.get() );
            if( other.gEta_ClustADC_Fit_Failures != NULL )  gEta_ClustADC_Fit_Failures  = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Fit_Failures.get() );
            
            if( other.gEta_ClustADC_Spec_NumPks != NULL )   gEta_ClustADC_Spec_NumPks   = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Spec_NumPks.get() );
            if( other.gEta_ClustADC_Spec_PkPos != NULL )    gEta_ClustADC_Spec_PkPos    = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Spec_PkPos.get() );
        } //End Copy Constructor
        
        //Assignment operator
        SectorEta & operator=(const SectorEta & other){
            if (this != &other ){ //Protects against invalid self-assignment
                fPos_Y = other.fPos_Y;
                fWidth = other.fWidth;
                
                map_sectorsPhi = other.map_sectorsPhi;
                
                mset_fClustADC_Fit_PkPos    = other.mset_fClustADC_Fit_PkPos;
                mset_fClustADC_Spec_PkPos   = other.mset_fClustADC_Spec_PkPos;
                
                //histograms
                clustHistos = other.clustHistos;
                hitHistos = other.hitHistos;
                
                //Summary Statistics
                statClustADC_Fit_PkPos  = other.statClustADC_Fit_PkPos;
                statClustADC_Spec_PkPos = other.statClustADC_Spec_PkPos;
                
                //Deep Copy
                if( other.gEta_ClustADC_Fit_NormChi2 != NULL )  gEta_ClustADC_Fit_NormChi2  = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Fit_NormChi2.get() );
                if( other.gEta_ClustADC_Fit_PkPos != NULL )     gEta_ClustADC_Fit_PkPos     = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Fit_PkPos.get() );
                if( other.gEta_ClustADC_Fit_Failures != NULL )  gEta_ClustADC_Fit_Failures  = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Fit_Failures.get() );
                
                if( other.gEta_ClustADC_Spec_NumPks != NULL )   gEta_ClustADC_Spec_NumPks   = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Spec_NumPks.get() );
                if( other.gEta_ClustADC_Spec_PkPos != NULL )    gEta_ClustADC_Spec_PkPos    = std::make_shared<TGraphErrors>( *other.gEta_ClustADC_Spec_PkPos.get() );
            } //Protects against invalid self-assignment
            
            return *this;
        } //End Assignment Operator
    }; //End SectorEta
    
} //End namespace uniformity

#endif
