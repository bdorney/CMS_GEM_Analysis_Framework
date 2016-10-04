//
//  QualityControlSectionNames.h
//  
//
//  Created by Brian L Dorney on 03/10/16.
//
//

#ifndef _QualityControlSectionNames_h
#define _QualityControlSectionNames_h

//C++ Includes
#include <stdio.h>
#include <string>

//Framework Includes

//ROOT Includes

namespace QualityControl {
    namespace Uniformity{
        struct SecNamesAna{
            std::string m_strSecBegin_Analysis;       //Defines how the file will look
            //std::string m_strSecBegin_Timing;         //Hard coded section headers
            std::string m_strSecBegin_Uniformity;     //Staring point
            std::string m_strSecBegin_Uniformity_Fit;
            std::string m_strSecBegin_Uniformity_Histo;
            
            std::string m_strSecEnd_Analysis;         //Defines how the file will look
            //std::string m_strSecEnd_Timing;           //Hard coded section headers
            std::string m_strSecEnd_Uniformity;       //ending point
            std::string m_strSecEnd_Uniformity_Fit;
            std::string m_strSecEnd_Uniformity_Hiso;
            
            //Constructor
            SecNamesAna(){
                m_strSecBegin_Analysis    = "[BEING_ANALYSIS_INFO]";
                //m_strSecBegin_Timing      = "[BEGIN_TIMING_INFO]";
                m_strSecBegin_Uniformity  = "[BEGIN_UNIFORMITY_INFO]";
                m_strSecBegin_Uniformity_Fit = "[BEGIN_ADC_FIT_INFO]";
                m_strSecBegin_Uniformity_Histo = "[BEGIN_HISTO_INFO]";
                
                m_strSecEnd_Analysis      = "[END_ANALYSIS_INFO]";
                //m_strSecEnd_Timing        = "[END_TIMING_INFO]";
                m_strSecEnd_Uniformity    = "[END_UNIFORMITY_INFO]";
                m_strSecEnd_Uniformity_Fit = "[END_ADC_FIT_INFO]";
                m_strSecEnd_Uniformity_Hiso = "[END_HISTO_INFO]";
            } //End Cosntructor
        }; //End SecNamesAna
        
        struct SecNamesRun{
            std::string m_strSecBegin_CompInfo;
            std::string m_strSecBegin_RunList;
            std::string m_strSecBegin_RunInfo;
            
            std::string m_strSecEnd_CompInfo;
            std::string m_strSecEnd_RunInfo;
            std::string m_strSecEnd_RunList;
            
            //Constructor
            SecNamesRun(){
                m_strSecBegin_CompInfo  = "[BEGIN_COMP_INFO]";
                m_strSecBegin_RunInfo   = "[BEGIN_RUN_INFO]";
                m_strSecBegin_RunList   = "[BEGIN_RUN_LIST]";
                
                m_strSecEnd_CompInfo    = "[END_COMP_INFO]";
                m_strSecEnd_RunInfo     = "[END_RUN_INFO]";
                m_strSecEnd_RunList     = "[END_RUN_LIST]";
            } //End Constructor
        }; //End SecNamesRun
    } //End namespace Uniformity
    
    namespace Plotter{
        struct SecNamesPlotter{
            std::string m_strSecBegin_Canv;
            std::string m_strSecBegin_Data;
            std::string m_strSecBegin_Plot;
            
            std::string m_strSecEnd_Canv;
            std::string m_strSecEnd_Data;
            std::string m_strSecEnd_Plot;
            
            //Constructor
            SecNamesPlotter(){
                m_strSecBegin_Canv = "[BEGIN_CANVAS]";
                m_strSecBegin_Data = "[BEGIN_DATA]";
                m_strSecBegin_Plot = "[BEGIN_PLOT]";
                
                m_strSecEnd_Canv = "[END_CANVAS]";
                m_strSecEnd_Data = "[END_DATA]";
                m_strSecEnd_Plot = "[END_PLOT]";
            } //End Constructor
        }; //End SecNamesPlotter
        
        struct ColNamesPlotter{
            std::string m_strColX;
            std::string m_strColX_Err;
            std::string m_strColY;
            std::string m_strColY_Err;
            
            //Constructor
            ColNamesPlotter(){
                m_strColX = "VAR_INDEP";
                m_strColX_Err "VAR_INDEP_ERR";
                m_strColY = "VAR_DEP";
                m_strColY_Err = "VAR_DEP_ERR";
            } //End Constructor
        }; //End ColNamesPlotter
    } //End namespace Plotter
} //End namespace Quality Control


#endif
