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
            
            float m_fZ;
            float m_fZ_Err;
            
            //Constructor
            DataPoint(){
                m_fX = m_fX_Err = m_fY = m_fY_Err = m_fZ = m_fZ_Err = 0.;
            }
            
            //Copy Constructor
            DataPoint(const DataPoint & other){
                m_fX      = other.m_fX;
                m_fX_Err  = other.m_fX_Err;
                
                m_fY      = other.m_fY;
                m_fY_Err  = other.m_fY_Err;
                
                m_fZ      = other.m_fZ;
                m_fZ_Err  = other.m_fZ_Err;
            } //End Copy Constructor
            
            //Clear container
            void clear(){
                m_fX = m_fX_Err = m_fY = m_fY_Err = m_fZ = m_fZ_Err = 0.;
                return;
            }
        }; //End DataPoint
        
        struct InfoFit{
            bool m_bFit;
            
            //float m_fRange_xLower;
            //float m_fRange_xUpper;
            float m_fSizeLine;          //Line Width
            
            int m_iColor;             //Line and Marker Color
            int m_iStyleLine;         //Line Style
            
            std::pair<int,int> m_pair_iParamRange_Sig;
            std::pair<int,int> m_pair_iParamRange_Bkg;
            
            std::string m_strFile_Name;        //Input ROOT file where TF1 is found (if supplied)
            std::string m_strFile_Path;        //Path in input ROOT file where TF1 is found
            
            std::string m_strFit_Formula;     //Fit Formula (used)
            std::string m_strFit_Formula_Sig; //Fit Formula - Signal (visual aid only)
            std::string m_strFit_Formula_Bkg; //Fit Formula - Bkg (visual aid only)
            std::string m_strFit_Name;
            std::string m_strFit_Option;
            
            std::string m_strLegEntry;
            
            //Allow for complex expressions
            std::vector<std::string> m_vec_strFit_ParamMeaning;
            std::vector<std::string> m_vec_strFit_ParamIGuess;
            std::vector<std::string> m_vec_strFit_ParamLimit_Min;
            std::vector<std::string> m_vec_strFit_ParamLimit_Max;
            std::vector<std::string> m_vec_strFit_Range;
            
            //Constructor
            InfoFit(){
                m_bFit = false;
                
                //m_fRange_xLower=0;
                //m_fRange_xUpper=1e4;
                m_fSizeLine = 3;
                
                m_iColor = kRed;
                m_iStyleLine = 1;
                
                m_strFile_Name = "";
                m_strFile_Path = "";

                m_strFit_Formula = "";
                m_strFit_Formula_Sig = "";
                m_strFit_Formula_Bkg = "";
                m_strFit_Name = "";
                m_strFit_Option = "R";
            } //End Constructor
        };
        
        struct InfoPlot{
            //bool m_bXAxis_UserRange;  //Use Range Specified by User
            //bool m_bYAxis_UserRange;  //Use Range Specified by User
            
            int m_iColor;             //Line and Marker Color
            
            int m_iStyleLine;         //Line Style
            int m_iStyleMarker;       //Marker Style
            
            float m_fSizeLine;          //Line Width
            float m_fSizeMarker;        //Marker Size
            
            //float m_fXAxis_Min;       //Min X-Axis Value
            //float m_fXAxis_Max;       //Max X-Axis Value
            
            //float m_fYAxis_Min;       //Min Y-Axis Value
            //float m_fYAxis_Max;       //Max Y-Axis Value
            
            std::string m_strFileName;      //Input ROOT File where data is found
            std::string m_strFilePath;      //Path in input ROOT File where data is found
            std::string m_strLegEntry;      //Legend Entry
            std::string m_strName;          //TName of TGraph
            //std::string m_strOptionDraw;    //Draw Option
            //std::string m_strTitle_X;       //X-Axis Title
            //std::string m_strTitle_Y;       //Y-Axis Title
            
            std::vector<DataPoint> m_vec_DataPts;
            
            std::map<std::string, InfoFit> m_map_infoFit;
            
            //Constructor
            InfoPlot(){
                //m_bXAxis_UserRange    = false;
                //m_bYAxis_UserRange    = false;
                
                m_iColor = kBlack;
                
                m_iStyleLine = 1;
                m_iStyleMarker = 20;
                
                m_fSizeLine = 1.;
                m_fSizeMarker = 0.7;
                
                m_strFileName = "";
                m_strFilePath = "";
                //m_strOptionDraw = "E1";
                //m_strTitle_Y = "N";
            }
            
            //Destructor
            ~InfoPlot(){
                m_strFileName.clear();
                m_strFilePath.clear();
                m_strLegEntry.clear();
                m_strName.clear();
                //m_strOptionDraw.clear();
                //m_strTitle_X.clear();
                //m_strTitle_Y.clear();
                m_vec_DataPts.clear();
            }
        }; //End InfoPlot
        
        struct InfoCanvas{
            bool m_bDrawLeg;
            bool m_bGrid_X, m_bGrid_Y;//, bGrid_Z;      //Grid
            bool m_bIsPrelim;                   	//Write "Preliminary" after CMS
            bool m_bLog_X, m_bLog_Y, m_bLog_Z;       	//Logarithmic Axis
            bool m_bMonoColor;				//Color Palette is a signal shade?
            bool m_bNormalize;				//Normalize Plot to unit area?
            
            bool m_bXAxis_UserRange;  //Use Range Specified by User
            bool m_bYAxis_UserRange;  //Use Range Specified by User
            bool m_bZAxis_UserRange;  //Use Range Specified by User
            
            int m_iLogoPos;                   		//Position of "CMS" or "CMS Preliminary"
            int m_iSize_X, m_iSize_Y;                   //Canvas Size

            int m_iXAxis_NDiv;      			//N Divisions for X axis
            int m_iYAxis_NDiv;      			//N Divisions for Y axis
            
            float m_fLegNDCPos_X1, m_fLegNDCPos_X2;     //NDC X-Position of Legend
            float m_fLegNDCPos_Y1, m_fLegNDCPos_Y2;     //NDC Y-Position of Legend

            float m_fMargin_Top, m_fMargin_Bot;     	//Pad Margins
            float m_fMargin_Lf, m_fMargin_Rt;     	//
            
            float m_fXAxis_Min;       //Min X-Axis Value
            float m_fXAxis_Max;       //Max X-Axis Value
            float m_fXAxis_Title_Offset;       //X-Axis Title Offset
            
            float m_fYAxis_Min;       //Min Y-Axis Value
            float m_fYAxis_Max;       //Max Y-Axis Value
            float m_fYAxis_Title_Offset;       //Y-Axis Title Offset

            float m_fZAxis_Min;       //Min Z-Axis Value
            float m_fZAxis_Max;       //Max Z-Axis Value
            float m_fZAxis_Title_Offset;    	//Z-Axis Title Offset
            
            std::vector<std::tuple<float, float, std::string> > m_vec_LatexNPos;
            
            std::string m_strName;          //TName
            std::string m_strOptionDraw;    //Draw Option
            std::string m_strPlotType;      //Type of Plot to be drawn
            std::string m_strTitle;         //Canvas Title
            std::string m_strTitle_X;       //X-Axis Title
            std::string m_strTitle_Y;       //Y-Axis Title
            std::string m_strTitle_Z;       //Z-Axis Title
            
            //std::map<std::string, InfoPlot> m_map_infoPlot;
            std::multimap<std::string, InfoPlot> m_map_infoPlot;
            
            //Constructor
            InfoCanvas(){
                m_bDrawLeg=true;
                m_bLog_X=m_bLog_Y=m_bLog_Z=false;
                m_bGrid_X=m_bGrid_Y=false;
                m_bIsPrelim=true;
                m_bMonoColor=false;
                m_bNormalize=false;

                m_bXAxis_UserRange    = false;
                m_bYAxis_UserRange    = false;
                m_bZAxis_UserRange    = false;
                
                m_iLogoPos=11;                
                m_iSize_X=m_iSize_Y=700;

                m_iXAxis_NDiv = 510;
                m_iYAxis_NDiv = 510;              

                m_fLegNDCPos_X1=m_fLegNDCPos_X2=-1;
                m_fLegNDCPos_Y1=m_fLegNDCPos_Y2=-1;
                
                m_fXAxis_Title_Offset=-1;
                m_fYAxis_Title_Offset=-1;
                m_fZAxis_Title_Offset=-1;

                m_fMargin_Top=0.08;
                m_fMargin_Bot=0.12;
                m_fMargin_Lf=0.15;
                m_fMargin_Rt=0.04;

                //strName="";
                m_strOptionDraw = "E1";
                m_strTitle="";
                m_strTitle_Y = "N";
            }
            
            //Destructor
            ~InfoCanvas(){
                m_vec_LatexNPos.clear();
                m_strTitle.clear();
                m_strOptionDraw.clear();
                m_strTitle_X.clear();
                m_strTitle_Y.clear();
                m_map_infoPlot.clear();
            }
        }; //End InfoCanvas

//RGB color palette
	struct PlotColorPaletteRGB{
		static const Int_t m_NRGBs = 5;
    		static const Int_t m_NCont = 255;

    		Double_t m_stops[m_NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    		Double_t m_red[m_NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    		Double_t m_green[m_NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    		Double_t m_blue[m_NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
	}; //End PlotColorPaletteRGB

	//Mono-shade color palette (Cyan lighter)
	//Thanks to Tutanon Sinthuprasith
	struct PlotColorPaletteMono{
		static const Int_t m_NRGBs = 20;
    		static const Int_t m_NCont = 510;

		Double_t m_blue[m_NRGBs]   = { 0.3825, 0.415, 0.4475, 0.48, 0.5125, 0.545, 0.5775, 0.61, 0.6425, 0.675, 0.7075, 0.74, 0.7725, 0.805, 0.8375, 0.87, 0.9025, 0.935, 0.9675, 1.00};
  		Double_t m_green[m_NRGBs]  = { 0.25, 0.30, 0.325, 0.35, 0.375, 0.40, 0.425, 0.45, 0.475, 0.50, 0.525, 0.55, 0.575, 0.60, 0.625, 0.65, 0.675, 0.75, 0.775, 0.80};
 		Double_t m_red[m_NRGBs]    = {  0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50};
  		Double_t m_stops[m_NRGBs] = { 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.95, 0.975, 1.00};
	}; //End PlotColorPaletteMono
    } //End namspace Plotter
} //End namespace QualityControl

#endif
