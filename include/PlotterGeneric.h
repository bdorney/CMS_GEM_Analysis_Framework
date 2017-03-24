//
//  PlotterGeneric.h
//  
//
//  Created by Brian L Dorney on 05/10/16.
//
//

#ifndef ____PlotterGeneric__
#define ____PlotterGeneric__

//C++ Includes
#include <map>
#include <memory>
#include <stdio.h>
#include <string>
#include <tuple>

//Framework Includes
#include "CMS_lumi.h"
#include "PlotterUtilityTypes.h"
#include "TimingUtilityFunctions.h"

//ROOT Includes
#include "TCanvas.h"
#include "TFrame.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"

namespace QualityControl {
    namespace Plotter {
        class PlotterGeneric {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            PlotterGeneric();
            
            //Destructor
            //------------------------------------------------------------------------------------------------------------------------------------------
            ~PlotterGeneric(){
                m_canv.reset();
                
                m_map_iSameNameCount.clear();
                m_map_fits.clear();
                
                m_tdrStyle.reset();
            }
            
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Creates the canvas
            //Draws all requested items
            //Saves output to a ROOT file
            virtual void plotAndStore();
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Sets the flag for automatically saving canvases
            virtual void setAutoSaveCanvas(bool bInput){ m_bSaveCanvases = bInput; return; };
            
            //Sets the canvas parameters
            virtual void setCanvasParameters(InfoCanvas & inputCanvInfo){
                m_canvInfo = inputCanvInfo;
                setMonoColor(m_canvInfo.m_bMonoColor);
                setPreliminary(m_canvInfo.m_bIsPrelim);
                setLogoPos(m_canvInfo.m_iLogoPos);
                return;
            }

            //Determines how the logo position is oriented
            //  0 : out of frame
            // 11 : top-left, left-aligned
            // 33 : top-right, right-aligned
            // 22 : center, centered
            // mode generally :
            //   10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
            virtual void setLogoPos(int iInput){
                m_iLogoPos = iInput;
                return;
            }
            
            virtual void setMonoColor(bool bInput){
                m_bMonoColor = bInput;
                return;
            }
            
            //True -> "CMS Preliminary"
            //False -> "CMS"
            virtual void setPreliminary(bool bInput){
                m_bIsPrelim = bInput;
                lumi.setPreliminary(bInput);
                return;
            }
            
            //Sets the output TFile name & option
            virtual void setOutputNameNOption(std::string  strInputName, std::string strInputOption){
                m_strOutputFile_Name    = strInputName;
                m_strOutputFile_Option  = strInputOption;
                return;
            }
            
        protected:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Draw the "CMS" or "CMS Preliminary" on the plot
            //How this text is drawn is based on the style given:
            //  https://ghm.web.cern.ch/ghm/plots/MacroExample/CMS_lumi.h
            //  https://ghm.web.cern.ch/ghm/plots/MacroExample/CMS_lumi.C
            virtual void drawCMS(){
                lumi.drawLumi(m_canv, 0, m_iLogoPos);
                return;
            }
            
            //Draws a TLatex object
            //Set additional style based on:
            //  https://ghm.web.cern.ch/ghm/plots/MacroExample/myMacro.C
            virtual void drawLatex(std::tuple<float, float, std::string> tupleTexLine);
            
            //Generates a TCanvas based on the user input
            //Set additional style based on:
            //  https://ghm.web.cern.ch/ghm/plots/MacroExample/myMacro.C
            virtual void initCanv();
            
            //In inherited class developer should:
            //  Immediately initPlot(InfoPlot & plotInfo)
            //  Check to make sure plot init'd successfully
            //  Set the style of the plot
            //  Add it to the legend
            //To be over-ridded by inherited classes
            virtual void addPlot(TLegend & inputLegend, InfoPlot & plotInfo);

            //Draw Fits, right now fits are assumed to always be TF1 objects
            //  Draws all fits defined for each plot
            //Developer can override though
            virtual void drawFits(TLegend & inputLegend);
            
            //In inherited class developer should:
            //  Draw all plots added with addPlot()
            //To be over-ridded by inherited classes
            virtual void drawPlots();

            //In inherited class developer should:
            //  Check if user is supply data or a TFile
            //  Initialize a pointer for the appropriate TObject
            //  TObject's constructor should be used or loaded from the TFile
            //To be over-ridded by inherited classes
            virtual void initPlot(InfoPlot & plotInfo);

            //In inherited class developer should:
            //  Loads a fit from a TFile and draws it for the requested plot
            //To be over-ridded by inherited classes
            virtual void loadAndDrawFit(TLegend & inputLegend, InfoFit & fitInfo);

            //In inherited class developer should:
            //  Perform a fit and draws it for the requested plot
            //To be over-ridded by inherited classes
            virtual void performAndDrawFit(TLegend & inputLegend, InfoFit & fitInfo, InfoPlot & plotInfo);

            //Saves the canvas as a *.png and a *.pdf file
            virtual void save2ImgFile(std::shared_ptr<TCanvas> inputCanvas);
            
            //Saves all TObjects to an output ROOT file
            //To be over-ridded by inherited classes
            virtual void write2RootFile();
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
	    //Returns a TF1 object from a given fitInfo container
            std::shared_ptr<TF1>  getFit(InfoFit & fitInfo);

            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Sets the style
            //Follows the tdrStyle.C macro available from:
            //  https://ghm.web.cern.ch/ghm/plots/
            virtual void setStyle();
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
            bool m_bIsPrelim;
            bool m_bMonoColor;
            bool m_bSaveCanvases;
            
            int m_iLogoPos; //Determines CMS Logo position
            //int m_iSameNamePlotCount; //Tracks How Many Plots have the same TName
            
            std::map<std::string,int> m_map_iSameNameCount; //Tracks How Many Plots have the same TName
            std::map<std::string, std::shared_ptr<TF1> > m_map_fits;
            

            std::string m_strOutputFile_Name;
            std::string m_strOutputFile_Option;
            
            InfoCanvas m_canvInfo;
            
            std::shared_ptr<TStyle> m_tdrStyle;
            std::shared_ptr<TCanvas> m_canv;
            
            Luminosity::CMSLumi lumi;
        }; //End class PlotterGeneric
    } //End namespace Plotter
} //End namespace QualityContrl


#endif /* defined(____PlotterGeneric__) */
