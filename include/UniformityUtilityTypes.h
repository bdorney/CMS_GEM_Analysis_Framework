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

//using namespace ROOT;

namespace QualityControl {
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
            int iUniformityGranularity; //Each iPhi sector is partitioned into this many slices
            
            Timing::HistoSetup histoSetup_clustADC;
            Timing::HistoSetup histoSetup_clustMulti;
            Timing::HistoSetup histoSetup_clustPos;
            Timing::HistoSetup histoSetup_clustSize;
            Timing::HistoSetup histoSetup_clustTime;
            
            Timing::HistoSetup histoSetup_hitADC;
            Timing::HistoSetup histoSetup_hitMulti;
            Timing::HistoSetup histoSetup_hitPos;
            Timing::HistoSetup histoSetup_hitTime;
            
            SelParam selClust;  //Selection Criteria for Clusters
            SelParam selHit;    //Selection Criteria for Hits
            
            //Initialization
            AnalysisSetupUniformity(){
                iEvt_First =  0;
                iEvt_Total = -1;
                iUniformityGranularity = 4;
            } //End Initialization
        }; //End AnalysisSetupUniformity
        
        //Run Setup
        struct RunSetup{
            //Setup - Master Mode
            std::string strRunMode;
            
            //Setup - Analyzer
            bool bAnaStep_Clusters;             //true -> perform the cluster analysis; false -> do not
            bool bAnaStep_Fitting;              //true -> run fitting on output histo's; false -> do not
            bool bAnaStep_Hits;                 //true -> perform the hit analysis (NOTE if bAnaStep_Reco is true this must also be true); false -> do not
            bool bAnaStep_Reco;                 //true -> reconstruct clusters from input hits; false -> use clusters provided in amoreSRS output file;
            bool bAnaStep_Visualize;            //true -> make summary plots at end of analysis; false -> do not
            
            //Setup - Comparison
            std::string strIdent;               //Unique identifier in input runs
            std::string strObsName;             //Name of Observable to be compared across inputs
            
            //Setup - Detector
            int iEta, iPhi, iSlice;             //Position within the detector
            
            std::string strDetName;             //Name of the detector

            //Setup - I/O
            bool bInputFromFrmwrk;              //true -> input file is a framework output file, not from amoreSRS; false -> input file(s) are from amoreSRS
            bool bLoadSuccess;
            bool bMultiOutput;                  //true -> one output file per input run; false -> one output file representing the "sum" of the input runs
            
            std::string strFile_Config_Ana;     //Name of analysis config file
            std::string strFile_Config_Map;     //Name of mapping file
            
            std::string strFile_Output_Name;    //Name of output TFile to be created
            std::string strFile_Output_Option;  //Option for TFile: CREATE, RECREATE, UPDATE, etc...
            
            //Setup - Visualizer
            bool bDrawNormalized;               //Comparison plots drawn normalized
            bool bVisPlots_PhiLines;            //true -> summary plots have phi lines segmenting sectors; false -> they do not
            bool bVisPlots_AutoSaving;          //true -> automatically save canvases during visualize step; false -> do not
            
            std::string strDrawOption;

            //Default constructor
            RunSetup(){
                //Setup - Master Mode
                strRunMode = "ANALYSIS";
                
                //Setup - Analyzer
                bAnaStep_Reco = false;
                bAnaStep_Clusters = bAnaStep_Fitting = bAnaStep_Hits = bAnaStep_Visualize = true;
                
                //Setup - Comparison
                strIdent = "RUN";
                strObsName = "CLUSTADC";
                
                //Setup - Detector
                iEta = 4;
                iPhi = 2;
                iSlice = -1;
                
                strDetName = "Detector";

                //Setup - I/O
                bInputFromFrmwrk = bMultiOutput = false;
                bLoadSuccess = false;   //This is set to true if the struct is loaded successfully

                strFile_Config_Ana = "config/configAnalysis.cfg";
                strFile_Config_Map = "config/Mapping_GE11-VII-L.cfg";
                
                strFile_Output_Name = "FrameworkOutput.root";
                strFile_Output_Option = "RECREATE";
                
                //Setup - Visualizer
                bDrawNormalized = false;
                bVisPlots_PhiLines = true;
                bVisPlots_AutoSaving = false;

                strDrawOption = "E1";
            } //End Default constructor
        }; //End RunSetup
        
        struct Hit{
	    float fPos_Y; //distance from detector origin; e.g. etaSector from amoreSRS

            //int iPos_Y; //distance from base of trapezoid (in mm); e.g. planeID from amoreSRS            

            int iStripNum;  //Strip number; e.g. strip from amoreSRS in range [0,384)?
            
            int iTimeBin;   //Time bin with the maximum ADC value; e.g. hitTimebin from amoreSRS in range [1,30]? Corresponds to tree adcX where X is an integer iTimeBin - 1;
            
            short sADCIntegral;         //Integral of vec_sADC
            
            //For now ADC is not used
            std::vector<short> vec_sADC;//ADC value of hit for time bin i; e.g. vec_fADC[iTimeBine] gives the adc value at the assigned time bin
            
            //Set Initial Values
            Hit(){
		fPos_Y = -1.;

                //iPos_Y = iStripNum = iTimeBin = -1;
		iStripNum = iTimeBin = -1;                

                sADCIntegral = 0;
                
                vec_sADC.resize(30);
            } //End Initialization
        }; //End Hit
        
        //Cluster of strips
        struct Cluster{
            //int iPos_Y;  //distance from base of trapezoid (in mm); e.g. planeID from amoreSRS
	    float fPos_Y;  //distance from detector origin; e.g. etaSector from amoreSRS
            float fPos_X;  //position within eta sector (in mm); e.g. clustPos from amoreSRS
            
            float fADC;       //ADC value of cluster; e.g. clustADCs from amoreSRS
            
            int iSize;      //Number of strips in cluster; e.g. clustSize from amoreSRS
            
            int iTimeBin;   //Time bin of cluster (not sure what that physically means...); e.g. clustTimeBin from amoreSRS
            
            std::map<int, Hit> map_hits;    //Hits used to reconstruct this cluster; placeholder, key-val here is the hit's strip number iStripNum
            
            //Set Initial Values
            Cluster(){
                //iPos_Y = iSize = iTimeBin = -1;
		iSize = iTimeBin = -1;                

                fPos_X = fPos_Y = fADC = -1.;
            } //End initialization
        }; //End Cluster
        
        struct Event{
            //bool bIsValid;
            
            int iNum_Evt, iNum_Run;
            
            std::vector<Cluster> vec_clusters;
            std::vector<Hit> vec_hits;
            
            //Default Constructor
            Event(){
                //bIsValid = false;
                iNum_Evt = iNum_Run = 1;
            }
            
            //Copy Constructor
            Event(const Event& other){
                iNum_Evt    = other.iNum_Evt;
                iNum_Run    = other.iNum_Run;
                
                vec_clusters= other.vec_clusters;
                vec_hits    = other.vec_hits;
            }
            
            //Reset
            void clear(){
                //bIsValid = false;
                iNum_Evt = iNum_Run = 1;
                vec_clusters.clear();
                vec_hits.clear();
            }
        };
        
        //Cluster Histograms
        struct HistosPhysObj{
            //One dimensional histograms
            std::shared_ptr<TH1F> hADC;    //ADC spectrum for some physics object
            std::shared_ptr<TH1F> hMulti;  //Multiplicity  "                    "
            std::shared_ptr<TH1F> hPos;    //Position      "                    "
            std::shared_ptr<TH1F> hSize;   //Size          "                    "
            std::shared_ptr<TH1F> hTime;   //Time          "                    "
            
            //Two dimensional histograms
            std::shared_ptr<TH2F> hADC_v_Pos;       //ADC vs Position for all physics objects
            std::shared_ptr<TH2F> hADC_v_Size;      //ADC vs Size for all physics objects
            std::shared_ptr<TH2F> hADC_v_Time;      //ADC vs Time for all physics objects
            std::shared_ptr<TH2F> hADCMax_v_ADCInt;	//Max ADC of an object (from all time bins) vs. Integral of object's ADC (summing all time bins)
            
            std::shared_ptr<TH2F> hSize_v_Pos;      //Size vs. Position for all physics objects
            
            //Run Histograms dimensional histograms
            std::map<int, std::shared_ptr<TH2F> > map_hADC_v_EvtNum_by_Run; //ADC vs Event Number for all physics objects (time series) for a given run (map_hADC_v_EvtNum_by_Run.first)
            std::map<int, std::shared_ptr<TH2F> > map_hTime_v_EvtNum_by_Run; //Time vs Event Number for all physics objects (time series) for a given run (map_hTime_v_EvtNum_by_Run.first)
            
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
                if( other.hADC_v_Size != NULL ) hADC_v_Size = std::make_shared<TH2F>( *other.hADC_v_Size.get() );
                if( other.hADC_v_Time != NULL ) hADC_v_Time = std::make_shared<TH2F>( *other.hADC_v_Time.get() );
                if( other.hADCMax_v_ADCInt != NULL ) hADCMax_v_ADCInt = std::make_shared<TH2F>( *other.hADCMax_v_ADCInt.get() );
                
                if( other.hSize_v_Pos != NULL ) hSize_v_Pos = std::make_shared<TH2F>( *other.hSize_v_Pos.get() );
                
                map_hADC_v_EvtNum_by_Run    = other.map_hADC_v_EvtNum_by_Run;
                map_hTime_v_EvtNum_by_Run   = other.map_hTime_v_EvtNum_by_Run;
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
                    if( other.hADC_v_Size != NULL ) hADC_v_Size = std::make_shared<TH2F>( *other.hADC_v_Size.get() );
                    if( other.hADC_v_Time != NULL ) hADC_v_Time = std::make_shared<TH2F>( *other.hADC_v_Time.get() );
                    if( other.hADCMax_v_ADCInt != NULL ) hADCMax_v_ADCInt = std::make_shared<TH2F>( *other.hADCMax_v_ADCInt.get() );
                    
                    if( other.hSize_v_Pos != NULL ) hSize_v_Pos = std::make_shared<TH2F>( *other.hSize_v_Pos.get() );
                    
                    map_hADC_v_EvtNum_by_Run    = other.map_hADC_v_EvtNum_by_Run;
                    map_hTime_v_EvtNum_by_Run   = other.map_hTime_v_EvtNum_by_Run;
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
            
            std::multiset<float> mset_fOutliers;
            
            std::shared_ptr<TH1F> hDist;    //Distribution of Dataset
            std::shared_ptr<TF1> fitDist;  //Fit of dataset
            
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
                if( other.fitDist != NULL ) fitDist = std::make_shared<TF1>( *other.fitDist.get() );
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
                    if( other.fitDist != NULL ) fitDist = std::make_shared<TF1>( *other.fitDist.get() );
                } //Protects against invalid self-assignment
                
                return *this;
            } //End Assignment Operator
            
            //Clear stored information
            void clear(){
                fIQR = fMax = fMean = fMin = -1;
                fQ1 = fQ2 = fQ3 = -1;
                fStdDev = -1;
                
                hDist.reset();
                fitDist.reset();
                
                mset_fOutliers.clear();
            }
        }; //End SummaryStatistics
        
        //Defines a slice of a phi sector within the detector
        struct SectorSlice{
            bool bFitAccepted;

            float fPos_Center;    //Center of the slice;
            float fWidth;       //Width of the slice;
            
            int iMinuitStatus;	//Minuit Status Code

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
                bFitAccepted = false;
                fPos_Center = fWidth = -1;
                iMinuitStatus  = 0;
            } //End Default Constructor
            
            //Copy Constructor
            SectorSlice(const SectorSlice& other){
                bFitAccepted = other.bFitAccepted;

                fPos_Center = other.fPos_Center;
                fWidth      = other.fWidth;
                
                iMinuitStatus  = other.iMinuitStatus;

                //Deep copy
                if( other.fitSlice_ClustADC != NULL )   fitSlice_ClustADC   = std::make_shared<TF1>( *other.fitSlice_ClustADC.get() );
                if( other.hSlice_ClustADC != NULL )     hSlice_ClustADC     = std::make_shared<TH1F>( *other.hSlice_ClustADC.get() );
            } //End Copy Constructor
            
            //Assignment operator
            SectorSlice & operator=(const SectorSlice & other){
                if (this != &other ) { //Protects against invalid self-assignment
                    bFitAccepted= other.bFitAccepted;

                    fPos_Center = other.fPos_Center;
                    fWidth      = other.fWidth;
                    
                    iMinuitStatus  = other.iMinuitStatus;

                    //Deep copy
                    if( other.fitSlice_ClustADC != NULL )   fitSlice_ClustADC   = std::make_shared<TF1>( *other.fitSlice_ClustADC.get() );
                    if( other.hSlice_ClustADC != NULL )     hSlice_ClustADC     = std::make_shared<TH1F>( *other.hSlice_ClustADC.get() );
                } //Protects against invalid self-assignment
                
                return *this;
            } //End Assignment operator
        }; //End SectorSlice
    } //End namespace Uniformity
} //End namespace QualityControl

#endif
