    //
//  TimingEvent.h
//  
//
//  Created by Brian L Dorney on 19/09/16.
//
//

#ifndef _TimingEvent_h
#define _TimingEvent_h

//C++ Includes
//#include <algorithm>
#include <map>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
//#include "DetectorTiming.h"
#include "DetectorSuperchamber.h"
//#include "IDaqVmeTypes.h"
#include "TimingUtilityTypes.h"

//ROOT Includes

namespace QualityControl {
    namespace Timing {
        struct TDCDataRaw {
            uint32_t m_uiEvtCount;
            //unsigned int m_uiEvtCount;
            
            std::vector<uint32_t> m_vec_DataWord;
            std::string m_strBaseAddress;   	//Base Address of originating module
            
            //Default Constructor
            TDCDataRaw(){
                m_uiEvtCount = 0;
            }
            
            //Copy Constructor
            TDCDataRaw(const TDCDataRaw &other){
                m_uiEvtCount = other.m_uiEvtCount;
                m_vec_DataWord = other.m_vec_DataWord;
                m_strBaseAddress = other.m_strBaseAddress;
            }

            //Clear Stored Digi
            void clear(){
                m_uiEvtCount = 0;
                m_vec_DataWord.clear();
                m_strBaseAddress.clear();
            }
        }; //End TDCDataRaw
            
        struct TDCDataDigi {
            uint32_t m_uiEvtCount;
            //unsigned int m_uiEvtCount;
            
            std::map<int, double> m_map_fTime;  //Key -> Chan #; Val -> Time
            std::string m_strBaseAddress;   	//Base Address of originating module
            
            //Default Constructor
            TDCDataDigi(){
                m_uiEvtCount = 0;
            }
            
            //Copy Constructor
            TDCDataDigi(const TDCDataDigi &other){
                m_uiEvtCount = other.m_uiEvtCount;
                m_map_fTime = other.m_map_fTime;
                m_strBaseAddress = other.m_strBaseAddress;
            }
            
            //Default Destructor
            /*~TDCDataDigi(){
             clear();
             }*/
            
            //Clear Stored Digi
            void clear(){
                m_uiEvtCount = 0;
                m_map_fTime.clear();
                m_strBaseAddress.clear();
            }
        }; //End TDCDataDigi;
            
        struct EventRaw{
            uint32_t m_uiEvtCount;
            //unsigned int m_uiEvtCount;
            
            std::map<std::string, Timing::TDCDataRaw> m_map_TDCData; //Key -> Hex Board Address; Val -> Data
            
            //Default Constructor
            EventRaw(){
                m_uiEvtCount = 0;
            }; //End Constructor
            
            //Default Destructor
            /*~EventDigi(){
             clear();
             };*/
            
            //Clear Stored Raw Data
            void clear(){
                m_uiEvtCount = 0;
                m_map_TDCData.clear();
            };
        }; //End EventRaw
        
        struct EventDigi{
            uint32_t m_uiEvtCount;
            //unsigned int m_uiEvtCount;
            
            std::map<std::string, Timing::TDCDataDigi> m_map_TDCData; //Key -> Hex Board Address; Val -> Data
            
            //Default Constructor
            EventDigi(){
                m_uiEvtCount = 0;
            }; //End Constructor
            
            //Default Destructor
            /*~EventDigi(){
             clear();
             };*/
            
            //Clear Stored Digi
            void clear(){
                m_uiEvtCount = 0;
                m_map_TDCData.clear();
            };
        }; //End EventDigi
        
        struct EventReco{
            uint32_t m_uiEvtCount;
            
            //the key is a pair, assigns an (i,j) indices to the PMTs
            Timing::DetectorMatrix m_detMatrix_Trigger;
            
            //the key is a pair, assigns an (i,k) indices to the PMTs
            //Note "i" here covers the same range
            //Note "j" does not necessarily cover teh same range as "k"
            Timing::DetectorMatrix m_detMatrix_DUTs;
            
            //Default Constructor
            EventReco(){
                m_uiEvtCount = 0;
            };
            
            //Constructor - Specifies Event number
            EventReco(uint32_t & uiEvtCount){
                m_uiEvtCount = uiEvtCount;
            };
            
            //Destructor
            ~EventReco(){
                m_detMatrix_Trigger.clear();
                m_detMatrix_DUTs.clear();
            };
        }; //End EventReco
    } //End namespace Timing
} //End namespace QualityControl


#endif
