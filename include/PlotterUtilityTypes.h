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
#include <stdio.h>
#include <string>
#include <vector>

//Framework Includes

//ROOT Includes
#include "TROOT.h"

using namespace ROOT;

namespace QualityControl {
    namespace Plotter {
        struct DataPoint{
            float fX;
            float fX_Err;
            
            float fY;
            float fY_Err;
            
            //Constructor
            DataPoint(){
                fX = fX_Err = fY = fY_Err = 0.;
            }
            
            //Copy Constructor
            DataPoint(const DataPoint & other){
                fX      = other.fX;
                fX_Err  = other.fX_Err;
                
                fY      = other.fY;
                fY_Err  = other.fY_Err;
            } //End Copy Constructor
            
            //Clear container
            clear(){
                fX = fX_Err = fY = fY_Err = 0.;
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
            float m_fYAxis_TitleOffset;
            
            string m_strLegEntry;     //Legend Entry
            
            //string m_strName;         //TName of TGraph
            
            string m_strOptionDraw;   //Draw Option
            
            string m_strTitle_X;  //X-Axis Title
            string m_strTitle_Y;  //Y-Axis Title
            
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
                
                m_strOptionDraw = "E1";
                
                m_strTitle_Y = "N";
            }
            
            //Destructor
            InfoPlot(){
                m_strLegEntry.clear();
                //m_strName.clear();
                m_strOptionDraw.clear();
                m_strTitle_X.clear();
                m_strTitle_Y.clear();
                m_vec_DataPts.clear();
            }
        }; //End InfoPlot
        
        struct InfoCanvas{
            bool m_bLog_X, m_bLog_Y, m_bLog_Z;         //Logarithmic Axis
            bool m_bGrid_X, m_bGrid_Y;//, bGrid_Z;      //Grid
            
            int m_iSize_X, m_iSize_Y;                   //Canvas Size
            
            float m_fLegNDCPos_X1, m_fLegNDCPos_X2;     //NDC X-Position of Legend
            float m_fLegNDCPos_Y1, m_fLegNDCPos_Y2;     //NDC Y-Position of Legend
            
            //std::vector<float> vec_fLatexNDCPos_X;  //NDC X-Position of TLatex
            //std::vector<float> vec_fLatexNDCPos_Y;  //NDC Y-Position of TLatex
            std::vector<std::tuple<float, float, std::string> > m_vec_LatexNPos;
            
            //string m_strName;                         //TName
            
            string m_strTitle;                        //Canvas Title
            
            //std::vector<string> vec_strLatexLine;   //String Container
            
            std::map<string, InfoPlot> m_map_infoPlot;
            
            //Constructor
            InfoCanvas(){
                bLog_X=bLog_Y=false;
                bGrid_X=bGrid_Y=false;
                
                //iSize_X=iSize_Y=700;
                
                //fLegNDCPos_X1=fLegNDCPos_X2=0;
                //fLegNDCPos_Y1=fLegNDCPos_Y2=0;
                
                //strName="";
                //strTitle="";
            }
            
            //Destructor
            InfoCanvas(){
                m_vec_LatexNPos.clear();
                m_strTitle.clear();
                m_map_infoPlot.clear();
            }
        }; //End InfoCanvas
    } //End namspace Plotter
} //End namespace QualityControl

#endif
