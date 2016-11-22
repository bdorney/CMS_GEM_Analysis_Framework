//
//  TimingRunInfo.h
//  
//
//  Created by Brian L Dorney on 19/09/16.
//
//

#ifndef _TimingRunSetup_h
#define _TimingRunSetup_h

//C++ Includes
#include <map>
#include <stdio.h>
#include <string>

//Framework Includes

//ROOT Includes

namespace QualityControl {
    namespace Timing {
        enum VMETypes{
            kVMEUnrecognized = -1,
            //kVMEBridge_V2718 = 0,   //Optical Bridge
            //kVMEIO_V513,            //IO
            //kVMETDC_V775,           //TDC
            kVMEBridge = 0,         //Optical Bridge
            kVMEIO,                 //IO
            kVMETDC,                //TDC
            n_vme_boards
        };
        
        struct HwVMEBoard{
            int m_iBitADC;      //X-bit ADC if present
            
            int m_iNumBoard;    //Bridge - Board number (if relevant)
            int m_iNumChan;     //Number of channels (if relevant)
            int m_iNumLink;     //Bridge - Link number (if relevant)
            
            Timing::VMETypes m_vme_type;        //All - Type of Board
            
            std::string m_strBaseAddress;       //All - Base Address
            std::string m_strFullScaleRange;    //TDC - Full Scale Range (e.g. resolution)
            
            //Default Constructor
            HwVMEBoard(){
                m_iBitADC = 12;
                
                m_iNumChan = 32;
                m_iNumLink = m_iNumBoard = 0;
                
                m_vme_type = kVMEUnrecognized;
                
                m_strBaseAddress = "0x0";
                m_strFullScaleRange = "0x5A"; //400ns window
            }; //End Default Constructor
            
            //Copy Constructor
            HwVMEBoard(const HwVMEBoard & other){
                m_iBitADC   = other.m_iBitADC;
                
                m_iNumChan  = other.m_iNumChan;
                m_iNumBoard = other.m_iNumBoard;
                m_iNumLink  = other.m_iNumLink;
                
                m_vme_type  = other.m_vme_type;
                
                m_strBaseAddress = other.m_strBaseAddress;
                m_strFullScaleRange = other.m_strFullScaleRange;
            }; //End Copy Constructor
            
            //Get Least Sensitive Bit
            float getLSB(){
                switch(m_vme_type){
                    case Timing::kVMETDC:   return 8.9 / std::stoi(m_strFullScaleRange, NULL, 16);
                    default:        return -1.;
                }
            }
            
            //Destructor
            /*~HwVMEBoard(){
             m_strBaseAddress.clear();
             m_strFullScaleRange.clear();
             };*/ //End Copy Constructor
        };
        
        struct RunSetup{
            bool m_bLoadSuccess;
            
            unsigned int m_uiEvtNum;    //Acquire this many events
            
            std::string m_strFile_Output_Name;      //Name of output file
            std::string m_strFile_Output_Option;    //Option for TFile: CREATE, RECREATE, UPDATE, etc...
            
            std::map<std::string, Timing::HwVMEBoard> m_map_vmeBoards;
            
            //Default Constructor
            RunSetup(){
                m_bLoadSuccess = false;
            };
            
            //Destructor
            /*~RunSetup(){
             m_strFile_Output_Name.clear();
             m_strFile_Output_Option.clear();
             
             m_map_vmeBoards.clear();
             };*/
        };
    } //End namespace Timing
} //End namespace QualityControl

#endif
