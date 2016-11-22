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
#include <utility>
#include <vector>

//My Includes
#include "TimingUtilityOperators.h"
#include "PlotterUtilityTypes.h"

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

//using namespace ROOT; //This is application specific code...no one would overwrite these classes...ever...

namespace QualityControl {
    namespace Timing {
        //Types
        typedef unsigned int            uint32_t;

        typedef std::map<int,int>::const_iterator imap_iter_t;
        
        //Specifies the stages of cuts
        enum kCutClasses{
            kEvt_All = 0,   //All Events
            kEvt_OOT_PMT,   //PMT's out of time
            kEvt_OOT_Det,   //Detector's out of time
            kEvt_OOT_DetTrig,  //Detector & Trigger out of time
            n_cut_classes
        };
        
        enum DetType{
            kDUT=0,       //Device under test
            kTrigger,     //Trigger device
            n_det_types
        };
        
        //Common Data Types Goes Here
        struct HistoSetup{
            bool bIsTrig;
            
            float fHisto_BinWidth;//bin width of histo
            float fHisto_xLower;  //lower x range of histo
            float fHisto_xUpper;  //upper x range of histo
            
            int iTDC_Chan;
            int iHisto_nBins;   //number of bins
            
            std::string strHisto_Name;
            std::string strHisto_Title_X;
            std::string strHisto_Title_Y;
            
            HistoSetup(){
                bIsTrig = false;
                
                fHisto_BinWidth = 1;
                fHisto_xLower = 0;
                fHisto_xUpper = 1200;
                
                iTDC_Chan = 0;
                iHisto_nBins = 1200;
                
                strHisto_Name = "";
                strHisto_Title_X = "";
                strHisto_Title_Y = "";
            }
        }; //End HistoSetup
        
        //Histograms for a Detector
        struct HistoDet{
            std::shared_ptr<TH1F> m_hAll; //All hits for this detector
            std::shared_ptr<TH1F> m_hMulti; //Channel Multiplicity for this detector
            
            std::map<int, std::shared_ptr<TH1F> > m_map_hChan; //hits for each channel
            
            //Default Constructor
            HistoDet(){
                //Intentionally Blank
            }
            
            //Copy Constructor
            HistoDet(const HistoDet & other){
                //Deep copy
                if( other.m_hAll != NULL )    m_hAll    = std::make_shared<TH1F>( *other.m_hAll.get() );
                if( other.m_hMulti != NULL )    m_hMulti    = std::make_shared<TH1F>( *other.m_hMulti.get() );
                
                for(auto iterHisto = other.m_map_hChan.begin(); iterHisto != other.m_map_hChan.end(); ++iterHisto){ //Loop Over Histograms
                    m_map_hChan[(*iterHisto).first] = std::make_shared<TH1F>( *(*iterHisto).second.get() );
                } //End Loop Over Histograms
            } //End Copy Constructor
            
            //Assignment operator
            HistoDet & operator=(const HistoDet & other){
                if (this != &other) { //Protects against invalid self-assignment
                    if( other.m_hAll != NULL )    m_hAll    = std::make_shared<TH1F>( *other.m_hAll.get() );
                    if( other.m_hMulti != NULL )    m_hMulti    = std::make_shared<TH1F>( *other.m_hMulti.get() );
                    
                    for(auto iterHisto = other.m_map_hChan.begin(); iterHisto != other.m_map_hChan.end(); ++iterHisto){ //Loop Over Histograms
                        m_map_hChan[(*iterHisto).first] = std::make_shared<TH1F>( *(*iterHisto).second.get() );
                    } //End Loop Over Histograms
                } //Protects against invalid self-assignment
                
                return *this;
            } //End Assignment operator
        }; //End HistoDet
        
        //Histograms for a Superchamber
        struct HistoSC{
            std::map<std::string, Timing::HistoDet> m_map_HistosDet;
            
            std::shared_ptr<TH1F> m_hAll;   //All Hits, from all det, from all chan
            std::shared_ptr<TH1F> m_hAND;   //AND from all det, from one chan per det (first chan in time)
            std::shared_ptr<TH1F> m_hDelta; //Difference in time between detectors
            std::shared_ptr<TH1F> m_hMulti; //Number of Detectors on per superchamber
            std::shared_ptr<TH1F> m_hOR;    //OR from all det, from one chan per det (first chan in time)
            
            std::shared_ptr<TH2F> m_hCorr;  //Correlation between detectors
            
            std::string m_strName;
            
            //Default constructor
            HistoSC(){
                m_strName = "";
            } //End Default constructor
            
            //Copy Constructor
            HistoSC(const HistoSC & other){
                m_map_HistosDet = other.m_map_HistosDet;
                
                //Deep copy - TH1F Pointers
                if( other.m_hAll != NULL )    m_hAll    = std::make_shared<TH1F>( *other.m_hAll.get() );
                if( other.m_hAND != NULL )    m_hAND    = std::make_shared<TH1F>( *other.m_hAND.get() );
                if( other.m_hDelta != NULL )    m_hDelta    = std::make_shared<TH1F>( *other.m_hDelta.get() );
                if( other.m_hMulti != NULL )    m_hMulti    = std::make_shared<TH1F>( *other.m_hMulti.get() );
                if( other.m_hOR != NULL )    m_hOR    = std::make_shared<TH1F>( *other.m_hOR.get() );
                
                //Deep copy - TH2F Pointers
                if( other.m_hCorr != NULL )    m_hCorr    = std::make_shared<TH2F>( *other.m_hCorr.get() );
                
                m_strName = other.m_strName;
            } //End Copy Constructor
            
            //Assignment Operator
            HistoSC & operator=(const HistoSC & other){
                if (this != &other) { //Protects against invalid self-assignment
                    m_map_HistosDet = other.m_map_HistosDet;
                    
                    //Deep copy - TH1F Pointers
                    if( other.m_hAll != NULL )    m_hAll    = std::make_shared<TH1F>( *other.m_hAll.get() );
                    if( other.m_hAND != NULL )    m_hAND    = std::make_shared<TH1F>( *other.m_hAND.get() );
                    if( other.m_hDelta != NULL )    m_hDelta    = std::make_shared<TH1F>( *other.m_hDelta.get() );
                    if( other.m_hMulti != NULL )    m_hMulti    = std::make_shared<TH1F>( *other.m_hMulti.get() );
                    if( other.m_hOR != NULL )    m_hOR    = std::make_shared<TH1F>( *other.m_hOR.get() );
                    
                    //Deep copy - TH2F Pointers
                    if( other.m_hCorr != NULL )    m_hCorr    = std::make_shared<TH2F>( *other.m_hCorr.get() );
                    
                    m_strName = other.m_strName;
                } //Protects against invalid self-assignment
                
                return *this;
            } //End Assignment operator
        }; //End HistoSC
        
        struct HistoMatrix{
            std::shared_ptr<TH1F> m_hMultiDUT;  //Number of DUT's on for each event
            std::shared_ptr<TH1F> m_hMultiTrig; //Number of Trig's on for each event
            
            std::map<std::pair<int,int>, Timing::HistoSC> m_map_HistosSC_All;
            
            //Default Constructor
            HistoMatrix(){
                //Left Intentionally blank
            } //End Default Constructor
            
            //Copy Constructor
            HistoMatrix(const HistoMatrix & other){
                //Deep copy
                if( other.m_hMultiDUT != NULL )    m_hMultiDUT    = std::make_shared<TH1F>( *other.m_hMultiDUT.get() );
                if( other.m_hMultiTrig != NULL )    m_hMultiTrig    = std::make_shared<TH1F>( *other.m_hMultiTrig.get() );
                
                m_map_HistosSC_All = other.m_map_HistosSC_All;
            } //End Copy Constructor
            
            void clear(){
                m_hMultiDUT.reset();
                m_hMultiTrig.reset();
                m_map_HistosSC_All.clear();
            }
            
            //Assignment Operator
            HistoMatrix & operator=(const HistoMatrix & other){
                if (this != &other) { //Protects against invalid self-assignment
                    if( other.m_hMultiDUT != NULL )    m_hMultiDUT    = std::make_shared<TH1F>( *other.m_hMultiDUT.get() );
                    if( other.m_hMultiTrig != NULL )    m_hMultiTrig    = std::make_shared<TH1F>( *other.m_hMultiTrig.get() );
                    
                    m_map_HistosSC_All = other.m_map_HistosSC_All;
                } //Protects against invalid self-assignment
                
                return *this;
            } //End Assignment operator
            
            //Subscript Operator
            Timing::HistoSC & operator[](std::pair<int,int> pair_Pos){
                return m_map_HistosSC_All[pair_Pos];
            } //End Subscript Operator
        }; //End HistoMatrix
        
        struct SelParamTiming{
            int m_iCut_NTrig_Max;   //Max # of Trig Det required per layer per event
            int m_iCut_NTrig_Min;   //Min # of Trig Det required per layer per event
            int m_iCut_NDUT;        //Min # of DUT required per event
            
            //Default Constructor
            SelParamTiming(){
                m_iCut_NTrig_Max = m_iCut_NTrig_Min = 1;
                m_iCut_NDUT = 1;
            } //End Default Constructor
            
            //Copy Constructor
            SelParamTiming(const SelParamTiming & other){
                m_iCut_NTrig_Max    = other.m_iCut_NTrig_Max;
                m_iCut_NTrig_Min    = other.m_iCut_NTrig_Min;
                m_iCut_NDUT         = other.m_iCut_NDUT;
            } //End Copy Constructor
            
            //Assignment Operator
            SelParamTiming & operator=(const SelParamTiming & other){
                if (this != &other){ //Protects against invalid self-assignment
                    m_iCut_NTrig_Max    = other.m_iCut_NTrig_Max;
                    m_iCut_NTrig_Min    = other.m_iCut_NTrig_Min;
                    m_iCut_NDUT         = other.m_iCut_NDUT;
                } //Protects against invalid self-assignment
                
                return *this;
            } //End Assignment Operator
        }; //End SelParamTiming
        
        struct AnalysisSetupTiming{
            int m_iEvt_First;  //Starting Event For Analysis
            int m_iEvt_Total;    //Total number of Events to Analyze
            
            HistoSetup m_hSetupTime;
            HistoSetup m_hSetupMulti;
            
            Plotter::InfoFit m_fitSetup_timeRes;
            
            SelParamTiming m_selTime;
            
            //Default Constructor
            AnalysisSetupTiming(){
                m_iEvt_First = 0;
                m_iEvt_Total = -1;
            } //End Default Constructor
            
            //Copy Constructor
            AnalysisSetupTiming(const AnalysisSetupTiming & other){
                m_iEvt_First  = other.m_iEvt_First;
                m_iEvt_Total  = other.m_iEvt_Total;
                m_selTime     = other.m_selTime;
            } //End Copy Constructor
            
            //Assignment Operator
            AnalysisSetupTiming & operator=(const AnalysisSetupTiming & other){
                if (this != &other){ //Protects against invalid self-assignment
                    m_iEvt_First  = other.m_iEvt_First;
                    m_iEvt_Total  = other.m_iEvt_Total;
                    m_selTime     = other.m_selTime;
                } //Protects against invalid self-assignment
                
                return *this;
            } //End Assignment Operator
        }; //End AnalysisSetupTiming
    } //End namespace Timing
} //End namespace QualityControl

#endif
