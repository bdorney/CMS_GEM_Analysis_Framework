//
//  VisualizeUniformity.h
//  
//
//  Created by Brian L Dorney on 18/02/16.
//
//

#ifndef ____VisualizeUniformity__
#define ____VisualizeUniformity__

//C++ Includes
#include <algorithm>
#include <cmath>
#include <iterator>
#include <map>
//#include <memory>
#include <numeric>
#include <stdio.h>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

//Framework Includes
#include "AnalyzeResponseUniformity.h"
//#include "DetectorMPGD.h" //Done in source file not header file due to inheritance/friendship
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityOperators.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes
#include "TCanvas.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TGraph2D.h"
//#include "TGraph2DErrors.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH1F.h"
//#include "TImage.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMultiGraph.h"
#include "TObject.h"
#include "TPad.h"
#include "TROOT.h"
#include "TStyle.h"
//#include "TView.h"

//using namespace ROOT;

#endif /* defined(____VisualizeUniformity__) */

namespace QualityControl {
    namespace Uniformity {
        
        class VisualizeUniformity : public AnalyzeResponseUniformity {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            VisualizeUniformity();
            
            VisualizeUniformity(Uniformity::AnalysisSetupUniformity inputSetup, Uniformity::DetectorMPGD inputDet);
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Draws the distribution of an observable onto a single pad of canvas
            //Takes a std::string which stores the physical filename as input
            virtual void storeCanvasData(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption);
            
            //Draws the distribution of an observable onto a single pad of canvas
            //Takes a TFile *, which the canvas is written to, as input
            virtual void storeCanvasData(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption);
            
            //Draws a given observable onto a single pad of canvas
            //Takes a std::string which stores the physical filename as input
            virtual void storeCanvasGraph(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);
            
            //Draws a given observable onto a single pad of canvas
            //Takes a TFile *, which the canvas is written to, as input
            virtual void storeCanvasGraph(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);
            
            //Makes a 2D plot of a given observable in the detector's active area
            //Takes a std::string which stores the physical filename as input
            virtual void storeCanvasGraph2D(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bNormalize);
            
            //Makes a 2D plot of a given observable in the detector's active area
            //Takes a TFile *, which the canvas is writtent to, as input
            virtual void storeCanvasGraph2D(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bNormalize);
            
            //Draws a given observable onto a single pad of canvas
            //Takes a std::string which stores the physical filename as input
            virtual void storeCanvasHisto(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);
            
            //Draws a given observable onto a single pad of canvas
            //Takes a TFile * which the histograms are written to as input
            virtual void storeCanvasHisto(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);
            
            //Makes a 2D plot of a given observable in the detector's active area
            //Takes a std::string which stores the physical filename as input
            virtual void storeCanvasHisto2D(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption);
            
            //Makes a 2D plot of a given observable in the detector's active area
            //Takes a TFile *, which the canvas is writtent to, as input
            virtual void storeCanvasHisto2D(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption);
            
            //Partitions a canvas into N TPads where N = number of ReadoutSectorEta for input DetectorMPGD
            //Draws a given observable on each pad
            //Takes a std::string which stores the physical filename as input
            virtual void storeCanvasHistoSegmented(std::string & strOutputROOTFileName, std::string strOption, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);
            
            //Partitions a canvas into N TPads where N = number of ReadoutSectorEta for input DetectorMPGD
            //Draws a given observable on each pad
            //Takes a std::string which stores the physical filename as input
            //Takes a TFile * which the histograms are written to as input
            virtual void storeCanvasHistoSegmented(TFile * file_InputRootFile, std::string strObsName, std::string strDrawOption, bool bShowPhiSegmentation);
            
            //For each member of the input map storeListOfCanvasesGraph is called
            //Takes a std::string which stores the physical filename as input
            //map_strObsNameAndDrawOpt
            //  first   -> Obs Name
            //  second  -> Draw option
            virtual void storeListOfCanvasesGraph(std::string & strOutputROOTFileName, std::string strOption, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);
            
            //For each member of the input map storeListOfCanvasesGraph is called
            //Takes a TFile * which the histograms are written to as input
            //map_strObsNameAndDrawOpt
            //  first   -> Obs Name
            //  second  -> Draw option
            virtual void storeListOfCanvasesGraph(TFile * file_InputRootFile, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);
            
            //For each member of the input map storeListOfCanvasesHisto is called
            //Takes a std::string which stores the physical filename as input
            //map_strObsNameAndDrawOpt
            //  first   -> Obs Name
            //  second  -> Draw option
            virtual void storeListOfCanvasesHisto(std::string & strOutputROOTFileName, std::string strOption, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);
            
            //For each member of the input map storeListOfCanvasesHisto is called
            //Takes a TFile * which the histograms are written to as input
            //map_strObsNameAndDrawOpt
            //  first   -> Obs Name
            //  second  -> Draw option
            virtual void storeListOfCanvasesHisto(TFile * file_InputRootFile, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);
            
            //For each member of the input map storeListOfCanvasesHistoSegmented is called
            //Takes a std::string which stores the physical filename as input
            //map_strObsNameAndDrawOpt
            //  first   -> Obs Name
            //  second  -> Draw option
            virtual void storeListOfCanvasesHistoSegmented(std::string & strOutputROOTFileName, std::string strOption, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);
            
            //For each member of the input map storeListOfCanvasesHistoSegmented is called
            //Takes a TFile * which the histograms are written to as input
            //map_strObsNameAndDrawOpt
            //  first   -> Obs Name
            //  second  -> Draw option
            virtual void storeListOfCanvasesHistoSegmented(TFile * file_InputRootFile, std::map<std::string, std::string> & map_strObsNameAndDrawOpt, bool bShowPhiSegmentation);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Sets the flag for automatically saving canvases
            virtual void setAutoSaveCanvas(bool bInput){ bSaveCanvases = bInput; return; };
            
        private:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Saves inputCanv as a *.png file
            //The file is placed in the working directory
            //The name of the file is the TName of the canvas
            virtual void save2png(TCanvas & inputCanvas);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //std::vector<float> getObsData(std::string strObsName, Uniformity::ReadoutSectorEta &inputEta);
            Uniformity::SummaryStatistics getObsData(std::string strObsName);
            
            std::shared_ptr<TGraphErrors> getObsGraph(std::string strObsName, Uniformity::ReadoutSectorEta &inputEta);
            
            std::shared_ptr<TH1F> getObsHisto(std::string strObsName, Uniformity::ReadoutSector &inputSector);
            
            std::map<int, std::shared_ptr<TH2F> > getMapObsHisto2D(std::string strObsName, Uniformity::ReadoutSector &inputSector);
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Members
            //------------------------------------------------------------------------------------------------------------------------------------------
            bool bSaveCanvases;
            
        }; //End class VisualizeUniformity
    } //End namespace Uniformity
} //End namespace QualityControl

