//
//  AnalyzeResponseUniformity.h
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#ifndef ____AnalyzeResponseUniformity__
#define ____AnalyzeResponseUniformity__

//C++ Includes
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <stdio.h>
#include <string>
#include <vector>

//Framework Includes
//#include "DetectorMPGD.h"
#include "Analyzer.h"
//#include "ParameterLoaderDetector.h"
#include "PlotterUtilityTypes.h"
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
        class AnalyzeResponseUniformity : public QualityControl::Analyzer {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            AnalyzeResponseUniformity();
            
            //Set the detector and anlysis parameters at construction
            AnalyzeResponseUniformity(AnalysisSetupUniformity inputSetup, DetectorMPGD & inputDet);
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Calculates Summary Statistics
            virtual void calcStatistics(Uniformity::SummaryStatistics &inputStatObs, std::multiset<float> &mset_fInputObs, std::string strObsName);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Sets the Analysis Setup
            
        protected:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Returns a fit whose parameters match those defined in the AnalysisSetupUniformity
            virtual TF1 getFit(int iEta, int iPhi, int iSlice, Plotter::InfoFit & setupFit, std::shared_ptr<TH1F> hInput, TSpectrum &specInput );
            
            //Returns a TGraph Errors whose parameters match those defined in the input HistoSetup object
            virtual TGraphErrors getGraph(int iEta, int iPhi, Timing::HistoSetup &setupHisto);
            
            //Returns a histogram whose parmeters match those defined in the input HistoSetup object
            virtual TH1F getHistogram(int iEta, int iPhi, Timing::HistoSetup &setupHisto);
            
            //Returns a histogram whose parmeters match those defined in the input HistoSetup object
            virtual TH2F getHistogram2D(int iEta, int iPhi, Timing::HistoSetup &setupHisto_X, Timing::HistoSetup &setupHisto_Y);
            
            //Formats a given input string such that it follows the iEta, iPhi, iSlice naming convention
            std::string getNameByIndex(int iEta, int iPhi, int iSlice, std::string & strInputPrefix, std::string & strInputName);
            std::string getNameByIndex(int iEta, int iPhi, int iSlice, const std::string & strInputPrefix, const std::string & strInputName);
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
        }; //End class AnalyzeResponseUniformity
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____AnalyzeResponseUniformity__) */
