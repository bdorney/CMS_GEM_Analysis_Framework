//
//  PlotterUtilityTypes.h
//  
//
//  Created by Brian L Dorney on 03/10/16.
//
//

#ifndef _PlotterUtilityTypes_h
#define _PlotterUtilityTypes_h


//C++ Includes
#include <map>
#include <stdio.h>
#include <string>
#include <vector>
#include <utility>

//Framework Includes

//ROOT Includes
#include "TROOT.h"

using namespace ROOT;

namespace QualityControl {
    namespace Plotter {
        struct DataPoint{
            float m_fX;
            float m_fX_Err;
            
            float m_fY;
            float m_fY_Err;
            
            //Constructor
            DataPoint(){
                m_fX = m_fX_Err = m_fY = m_fY_Err = 0.;
            }
            
            //Copy Constructor
            DataPoint(const DataPoint & other){
                m_fX      = other.m_fX;
                m_fX_Err  = other.m_fX_Err;
                
                m_fY      = other.m_fY;
                m_fY_Err  = other.m_fY_Err;
            } //End Copy Constructor
            
            //Clear container
            void clear(){
                m_fX = m_fX_Err = m_fY = m_fY_Err = 0.;
                return;
            }
        }; //End DataPoint
        
        struct InfoPlot{
            bool m_bXAxis_UserRange;  //Use Range Specified by User
            bool m_bYAxis_UserRange;  //Use Range Specified by User
            
            int m_iColor;             //Line and Marker Color
            
            int m_iStyleLine;         //Line Style
            int m_iStyleMarker;       //Marker Style
            
            float m_fSizeLine;          //Line Width
            float m_fSizeMarker;        //Marker Size
            
            float m_fXAxis_Min;       //Min X-Axis Value
            float m_fXAxis_Max;       //Max X-Axis Value
            
            float m_fYAxis_Min;       //Min Y-Axis Value
            float m_fYAxis_Max;       //Max Y-Axis Value
            //float m_fYAxis_TitleOffset;
            
            std::string m_strFileName;      //Input ROOT File where data is found
            std::string m_strLegEntry;      //Legend Entry
            std::string m_strName;          //TName of TGraph
            std::string m_strOptionDraw;    //Draw Option
            std::string m_strTitle_X;       //X-Axis Title
            std::string m_strTitle_Y;       //Y-Axis Title
            
            std::vector<DataPoint> m_vec_DataPts;
            
            //Constructor
            InfoPlot(){
                m_bXAxis_UserRange    = false;
                m_bYAxis_UserRange    = false;
                
                m_iColor = kBlack;
                
                m_iStyleLine = 1;
                m_iStyleMarker = 20;
                
                m_fSizeLine = 1.;
                m_fSizeMarker = 0.7;
                
                m_strFileName = "";
                m_strOptionDraw = "E1";
                m_strTitle_Y = "N";
            }
            
            //Destructor
            ~InfoPlot(){
                m_strLegEntry.clear();
                //m_strName.clear();
                m_strOptionDraw.clear();
                m_strTitle_X.clear();
                m_strTitle_Y.clear();
                m_vec_DataPts.clear();
            }
        }; //End InfoPlot
        
        struct InfoCanvas{
            bool m_bLog_X, m_bLog_Y;//, m_bLog_Z;         //Logarithmic Axis
            bool m_bGrid_X, m_bGrid_Y;//, bGrid_Z;      //Grid
            
            int m_iSize_X, m_iSize_Y;                   //Canvas Size
            
            float m_fLegNDCPos_X1, m_fLegNDCPos_X2;     //NDC X-Position of Legend
            float m_fLegNDCPos_Y1, m_fLegNDCPos_Y2;     //NDC Y-Position of Legend
            
            std::vector<std::tuple<float, float, std::string> > m_vec_LatexNPos;
            
            std::string m_strName;                      //TName
            std::string m_strTitle;                     //Canvas Title
            
            std::map<std::string, InfoPlot> m_map_infoPlot;
            
            //Constructor
            InfoCanvas(){
                m_bLog_X=m_bLog_Y=false;
                m_bGrid_X=m_bGrid_Y=false;
                
                m_iSize_X=m_iSize_Y=700;
                
                m_fLegNDCPos_X1=m_fLegNDCPos_X2=-1;
                m_fLegNDCPos_Y1=m_fLegNDCPos_Y2=-1;
                
                //strName="";
                //strTitle="";
            }
            
            //Destructor
            ~InfoCanvas(){
                m_vec_LatexNPos.clear();
                m_strTitle.clear();
                m_map_infoPlot.clear();
            }
        }; //End InfoCanvas
    } //End namspace Plotter
} //End namespace QualityControl

#endif
