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
    } //End namespace Timing
} //End namespace QualityControl

#endif
