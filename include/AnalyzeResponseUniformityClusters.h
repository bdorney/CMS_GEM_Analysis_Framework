//
//  AnalyzeResponseUniformityClusters.h
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#ifndef ____AnalyzeResponseUniformityClusters__
#define ____AnalyzeResponseUniformityClusters__

//C++ Includes
#include <algorithm>
#include <cmath>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <stdio.h>
#include <string>
#include <vector>

//Framework Includes
#include "AnalyzeResponseUniformity.h"
//#include "DetectorMPGD.h"
#include "ParameterLoaderAmoreSRS.h"
#include "TimingUtilityFunctions.h"
#include "TimingUtilityTypes.h"
#include "UniformityUtilityOperators.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes
#include "TDirectory.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TList.h"
//#include "TPolyMarker.h"
#include "TROOT.h"
#include "TSpectrum.h"

//C++ Mathematical Expression Toolkit Library
//By Arash Partow; http://www.partow.net/programming/exprtk/index.html
#include "exprtk.hpp"

namespace QualityControl {
    namespace Uniformity {
        class AnalyzeResponseUniformityClusters : public AnalyzeResponseUniformity {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            AnalyzeResponseUniformityClusters();
            
            //Set the detector and anlysis parameters at construction
            AnalyzeResponseUniformityClusters(Uniformity::AnalysisSetupUniformity inputSetup);
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Filling*****************
            //Loops over all stored clusters in detMPGD and Book histograms for the full detector
            virtual void fillHistos(DetectorMPGD & inputDet);
            
            //Fitting*****************
            //Loops over all slices in detMPGD and fits Booked histograms for the full detector
            virtual void fitHistos(DetectorMPGD & inputDet);
            
            
            //Initialize**************
            //Loops through the detector and initializes all cluster graphs
            virtual void initGraphsClusters(DetectorMPGD & inputDet);
            
            //Loops through the detector and initalizes all cluster histograms
            virtual void initHistosClusters(DetectorMPGD & inputDet);
            
            //Loops through the detector and creates a new set of histograms specific to the run iInputRunNo
            virtual void initHistosClustersByRun(int iInputRunNo, DetectorMPGD & inputDet);
            
            //Loading*****************
            //Loads a ROOT file previously created by an instance of AnalyzeResponseUniformityClusters
            //Loads all TObjects found in the input ROOT file into detMPGD;
            //Any previously stored information in detMPGD is lost.
            //Takes a std::string which stores the physical filename as input
            virtual void loadHistosFromFile(std::string & strInputMappingFileName, std::string & strInputROOTFileName);
            
            //Loads a ROOT file previously created by an instance of AnalyzeResponseUniformityClusters
            //Loads all TObjects found in the input ROOT file into detMPGD;
            //Any previously stored information in detMPGD is lost.
            //Takes a TFile * which the histograms are written to as input
            virtual void loadHistosFromFile(std::string & strInputMappingFileName, TFile * file_InputRootFile);
            
            //Pass/Fail***************
            //Checks to see if the detector's uniformity is within requested amount
            
            //Storing*****************
            //Stores booked cluster histograms (for those histograms that are non-null)
            //Takes a std::string which stores the physical filename as input
            void storeHistos(std::string & strOutputROOTFileName, std::string strOption, DetectorMPGD & inputDet);
            
            //Stores booked cluster histograms (for those histograms that are non-null)
            //Takes a TFile * which the histograms are written to as input
            void storeHistos(TFile * file_InputRootFile, DetectorMPGD & inputDet);
            
            //Stores booked cluster fits (for those fits that are non-null)
            //Takes a std::string which stores the physical filename as input
            void storeFits(std::string & strOutputROOTFileName, std::string strOption, DetectorMPGD & inputDet);
            
            //Stores booked cluster fits (for those fits that are non-null)
            //Takes a TFile * which the histograms are written to as input
            void storeFits(TFile * file_InputRootFile, DetectorMPGD & inputDet);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
        private:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Members
            //------------------------------------------------------------------------------------------------------------------------------------------
            
        }; //End class AnalyzeResponseUniformityClusters
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____AnalyzeResponseUniformityClusters__) */
