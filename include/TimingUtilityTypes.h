#ifndef ____TimingUtilityTypes__
#define ____TimingUtilityTypes__

//C++ Includes
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <stdio.h>
#include <sstream>
#include <string>

//My Includes
#include "TimingUtilityOperators.h"

//ROOT Includes
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TROOT.h"

/*
 * 
 *
 *
 *
 */

using namespace ROOT; //This is application specific code...no one would overwrite these classes...ever...

namespace Timing {
    //Specifies the stages of cuts
    enum kCutClasses{
        kEvt_All = 0,   //All Events
        kEvt_OOT_PMT,   //PMT's out of time
        kEvt_OOT_Det,   //Detector's out of time
        kEvt_OOT_DetTrig,  //Detector & Trigger out of time
        n_cut_classes
    };
    
    //Common Data Types Goes Here
    struct HistoSetup{
        bool bFit;              //Execute fit?
        bool bFit_AutoRanging;  //Fit range determined automatically?
        bool bIsTrig;
        //bool bSubtractBkg;      //Background subtraction
        
        float fHisto_xLower;  //lower x range of histo
        float fHisto_xUpper;  //upper x range of histo
        
        int iTDC_Chan;
        int iHisto_nBins;   //number of bins
        
        std::string strFit_Formula; //Fit formula
        std::string strFit_Name;    //
        std::string strFit_Option;
        
        std::string strHisto_Name;
        std::string strHisto_Title_X;
        std::string strHisto_Title_Y;
        
        std::vector<std::string> vec_strFit_ParamMeaning;
        std::vector<std::string> vec_strFit_ParamIGuess;
        
        HistoSetup(){
            bFit = bFit_AutoRanging = bIsTrig = false;
            
            fHisto_xLower = 0;
            fHisto_xUpper = 1200;
            
            iTDC_Chan = 0;
            iHisto_nBins = 1200;
            
            strFit_Formula = "[0]*[2]*sqrt(TMath::Pi()/2.)*(TMath::Erf( (12.5 + (x-[1])) / ([2] * sqrt(2.) ) ) - TMath::Erf( ((x-[1]) - 12.5) / ([2] * sqrt(2.) ) ) )";
            strFit_Name = "func_Timing";
            strFit_Option = "R";
            
            strHisto_Name = "histo_Timing";
            strHisto_Title_X = "time #left(ns#right)";
            strHisto_Title_Y = "Counts #left(N/ns#right)";
        }
    }; //End HistoSetup
    
    struct AnalysisSetup{
        //bool bCompute_OR;
        //bool bCompute_AND;
        bool bInvertTime;
        bool bMatchArrivalTime;
        
        float fCut_MaxDeltaT_Det;       //Max allowed deltaT between detectors
        float fCut_MaxDeltaT_DetTrig;    //Max allowed deltaT between detector and Trig
        float fCut_MaxDeltaT_PMT;       //Max allowed deltaT between PMT's (who do not determine trigger)
        float fCut_MaxDeltaT_PMTTrig;   //Max allowed deltaT between trigger determining PMT & other PMT's
        
        float fTDCWinSize;
        
        HistoSetup setupOR;
        HistoSetup setupAND;
        
        std::map<std::string, HistoSetup> map_DetSetup;
        std::map<std::string, HistoSetup> map_PMTSetup;
        
        AnalysisSetup(){
            //bCompute_AND = bCompute_OR = bMatchArrivalTime = false;
            bMatchArrivalTime = false;
            bInvertTime = false;
            
            fCut_MaxDeltaT_Det = fCut_MaxDeltaT_DetTrig = fCut_MaxDeltaT_PMT = fCut_MaxDeltaT_PMTTrig = fTDCWinSize = 1200;
        }
    }; //End AnalysisSetup
    
    struct TDCAnalysisResults{
        float fTDC_Fit_Chi2;
        float fTDC_Fit_NDF;

        float fTDC_Eff;
        
        float fTDC_Histo_Mean;
        float fTDC_Histo_RMS;
        float iTDC_Histo_nPks;
        
        std::shared_ptr<TF1> func_TDC_Fit;//(nullptr);
        std::shared_ptr<TH1F> hTDC_Histo; //(nullptr);
        
        std::map<std::string, float> map_fTDC_Fit_Param; //Fit parameters
        std::map<std::string, float> map_fTDC_Fit_ParamErr; //Error on fit parameters

        std::vector<std::string> vec_strParamName;
        
        TDCAnalysisResults(){
            fTDC_Histo_Mean = fTDC_Histo_RMS = fTDC_Eff = fTDC_Fit_Chi2 = fTDC_Fit_NDF = -1;
            iTDC_Histo_nPks = -1;
        }
    }; //End TDCAnalysisResults
    
    struct RackParameters{
        float fAmp_Gain_Coarse; //A.U.
        float fAmp_Gain_Fine;   //A.U.
        float fAmp_Time_Diff;   //nanoseconds
        float fAmp_Time_Int;    //nanoseconds
        
        float fDisc_Thresh;     //mV
    };
    
    struct Detector{
        //Detector Info
        //=============================
        bool bDet_Gain_IndepVar_Imon;
        bool bDet_HV_MultiChan;         //true -> 7 electrodes powered independently; false -> Divider
        
        float fDet_Imon;    //Current through the Divider
        float fDet_VDrift;  //Potential on the Drift
        float fDet_VG1_Top;    //Potential on GEM1 Top
        float fDet_VG1_Bot;    //Potential on GEM1 Bot
        float fDet_VG2_Top;    //"             "2 Top
        float fDet_VG2_Bot;
        float fDet_VG3_Top;
        float fDet_VG3_Bot;
        
        float fDet_Gain;            //Gain, given by f(x) = exp(Const + Slope * x)
        float fDet_Gain_Err;        //Gain Error
        float fDet_Gain_Const;      //Gain Curve - Amplitude
        float fDet_Gain_Const_Err;  //Gain Curve - Amplitude, Error
        float fDet_Gain_Slope;      //Gain Curve - Exponential Slope
        float fDet_Gain_Slope_Err;  //Gain Curve - Exponential Slope, Error
        
        float fDet_GasFrac_Ar;  //Argon Fraction in Gas
        float fDet_GasFrac_CO2; //CO2 "                 "
        float fDet_GasFrac_CF4; //CF4 "                 "
        
        int iDet_Pos;       //Position of Detector of Interest in Listing
        int iDet_Eta;       //iEta value of readout sector
        int iDet_Phi;       //iPhi value of readout sector
        
        float fZPos;    //Position along beamline
        
        //std::string strDet_Name; //Detector Name
        
        //std::string getTreeText(){
        //    return
        //}
        
        //Rack Parameters
        //=============================
        RackParameters RackInfo;
        
        //TDC
        //=============================
        TDCAnalysisResults timingResults;
        
        int iTDC_Chan;

        std::vector<int> vec_iTDC_Data; //Event-by-Event Data
        
        //VFAT Info
        //=============================
        //int iTURBO_ID = -1;      //TURBO ID, 0->Master, 1->Slave 1, 2->Slave 2, etc...
        int iVFAT_Pos;      //VFAT Position (on GE1/1)
        
        //std::string strVFAT_ID = ""; //VFAT ID, hex code
        
        int iVFAT_IPreAmpIn;
        int iVFAT_IPreAmpFeed;
        int iVFAT_IPreAmpOut;
        
        int iVFAT_IShaper;
        int iVFAT_IShaperFeed;
        
        int iVFAT_IComp;        //Current to Comparator
        
        int iVFAT_MSPL;         //Monostable Pulse Length
        
        //int iVFAT_Latency;      //VFAT Latency
        
        float fVFAT_Thresh;     //VFAT Threshold in fC
        
        Detector(){
            //Detector Info
            //=============================
            bDet_Gain_IndepVar_Imon = true;
            
            fDet_Imon=fDet_VDrift = -1;
            fDet_VG1_Top=fDet_VG1_Bot=fDet_VG2_Top=fDet_VG2_Bot=fDet_VG3_Top=fDet_VG3_Bot=-1;
            
            fDet_Gain = fDet_Gain_Err = -1;            //Gain, given by f(x) = exp(Const + Slope * x)
            fDet_Gain_Const = fDet_Gain_Const_Err = 0;
            fDet_Gain_Slope = fDet_Gain_Slope_Err = 0;
            
            fDet_GasFrac_Ar = fDet_GasFrac_CO2 = fDet_GasFrac_CF4 = -1;
            
            iDet_Pos = 0;
            iDet_Eta = iDet_Phi = -1;
            
            std::string strDet_Name = ""; //Detector Name
            
            //VFAT Info
            //=============================
            fVFAT_Thresh = -1;     //VFAT Threshold in fC
            
            iVFAT_Pos = -1;
            
            iVFAT_IPreAmpIn = iVFAT_IPreAmpFeed = iVFAT_IPreAmpOut = -1;
            
            iVFAT_IShaper = iVFAT_IShaperFeed = -1;
            
            iVFAT_IComp = -1;        //Current to Comparator
            
            iVFAT_MSPL = -1;         //Monostable Pulse Length
            
            //TDC
            //=============================
            iTDC_Chan = 0;
        }
    };
    
    struct PMT{
        bool bIsTrig;
        
        float fPMT_HV;      //HV Setpoint
        
        float fZPos;    //Position along beamline
        
        //Rack Parameters
        //=============================
        RackParameters RackInfo;
        
        //TDC
        //=============================
        TDCAnalysisResults timingResults;
        
        int iTDC_Chan;
        
        std::vector<int> vec_iTDC_Data; //Event-by-Event Data
        
        PMT(){
            bIsTrig = false;
            
            fPMT_HV = fZPos = -1;
            
            iTDC_Chan = 0;
        }
    };
    
    //Run Info
    //Most values initialized to -1 (iBeam_Type to 0)
    //Pointers initialized to nullptr
    struct Run{
        //Run info
        //=============================
        bool bTakeHVFromRunParamTree;           //Take HV parameters from the run and not the filename
        
        //float fMaxDiffArrivalTime;              //Depreciated
        float fSupermoduleHVSetpoint;           //Setpoint of Supermodule
        float fTrig_Delay;                      //Trigger Delay in ns (Only used in Sync mode);
        
        int iBeam_Type;                         //11->Electron, 13->Muon, 211->Pion
        int iEvtPassing[n_cut_classes];         //Number of events passing each selection cut per run
        int iRun;                               //Run Number
        int iTrig_Mode;                         //0->Async, 1->Sync
        
        std::string strRunName;                 //Name of the ROOT file (e.g. the Run)
        std::string strTreeName_Run;            //Name of TTree that has the data
        std::string strTreeName_RunParam_DUT;   //Name of TTree that has the parameters of the DUT
        
        //Detectors
        //=============================
        std::map<std::string, Detector> map_det;//Map of detectors to be analyzed in this run
        
        //Photomultipliers
        //=============================
        std::map<std::string, PMT> map_PMT;     //Map of PMT's to be analyzed in this run
        
        //TDC Info
        //=============================
        //int iTDC_Chan_Trig;                     //Depreciated
        
        TDCAnalysisResults timingResultsAND;    //TDC results for AND of detectors (2 detectors only)
        TDCAnalysisResults timingResultsOR;     //TDC results for OR of detectors (2 detectors only)
        
        std::shared_ptr<TH1F> hTDC_DeltaT;      //Difference in Time of detectors (2 detectors only)
        std::shared_ptr<TH2F> hTDC_Correlation; //Correlation of Times of detectors (2 detectors only)
        
        Run(){
            //Run info
            //=============================
            bTakeHVFromRunParamTree = false;
            
            fSupermoduleHVSetpoint = fTrig_Delay = -1;    //Trigger Delay in ns;
            
            iBeam_Type = 0;     //11->Electron, 13->Muon, 211->Pion
            
            for(int i=0; i < n_cut_classes; ++i){iEvtPassing[i] = 0;}
            
            iRun = iTrig_Mode = -1;           //Run Number
            
            strRunName = "";
            strTreeName_Run = "";
            strTreeName_RunParam_DUT = "";
            
            //TDC Info
            //=============================
            //iTDC_Chan_Trig = 0;         //Channel of the Trigger in the TDC
        }
    };
    
} //End namespace

#endif
