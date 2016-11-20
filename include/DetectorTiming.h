//
//  DetectorTiming.h
//  
//
//  Created by Brian L Dorney on 26/10/16.
//
//

#ifndef ____DetectorTiming__
#define ____DetectorTiming__

//C++ Includes
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <utility>

//Framework Includes
#include "TimingUtilityTypes.h"

//ROOT Includes

namespace QualityControl {
    namespace Timing {
        
        //Note information from a detector cannot come from 2 different VME boards
        class DetectorTiming {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            DetectorTiming();
            
            //Destructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            ~DetectorTiming(){
                reset();
            };
            
            //Operators
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual void reset();
            virtual void resetChannels();
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Get base address of VME board this detector is plugged into
            virtual std::string getBaseAddress(){
                return m_strBaseAddress;
            };

            //Return the data from this detector for a particular channel
            virtual double getChanData(int iDetChan){
                return m_map_DetChanData[iDetChan];
            };
            
            //Get const iterators for the detector's channel map
            //pair::first is m_map_VMEChan2DetChan.cbegin()
            //pair::second is m_map_VMEChan2DetChan.cend()
            virtual std::pair<imap_iter_t, imap_iter_t> getChannelMapVME2Det();
            
            //Get Detector's name
            virtual std::string getName(){
                return m_strName;
            };
            
            //Get number of channels in the detector
            virtual int getNChan(){
                return m_map_VMEChan2DetChan.size();
            };
            
            virtual bool hasData(){
                return bHasData;
            };
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Set base address of VME board this detector is plugged into
            virtual void setBaseAddress(std::string strInput){
                m_strBaseAddress = strInput;
                return;
            };
            
            //Set the mapping between VME board channel and detector channel
            //Key value is VME board chan #
            virtual void setChannelMapVME2Det(std::map<int,int> & map_iInputChMap){
                m_map_VMEChan2DetChan = map_iInputChMap;
                return;
            };
            
            //Set the mapping between VME board channel and detector channel
            //First value is VME board chan #
            virtual void setChannelMapVME2Det(int iVMEChan, int iDetChan){
                m_map_VMEChan2DetChan[iVMEChan]=iDetChan;
                return;
            };
            
            //Set the data for a particular channel for a particular event
            virtual void setChanData(int iVMEChan, double dData);
            
            //Set Detector's name
            virtual void setName(std::string strInput){
                m_strName = strInput;
                return;
            };
            
        protected:
            //Attributes - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            bool bHasData;
            
            std::string m_strName;  //Name of the detector
            
            std::string m_strBaseAddress;   //Base address of the VME board this detector is connected to
            
            std::map<int,int> m_map_VMEChan2DetChan; //map for VME board chan to detector chan, key value is VME board chan #
            
            //key value is det chan #
            std::map<int, double> m_map_DetChanData;
            
        }; //End class DetectorTiming
    } //End namespace Timing
} //End namespace QualityControl


#endif /* defined(____DetectorTiming__) */
