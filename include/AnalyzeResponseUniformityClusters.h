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

namespace Uniformity {
    //Used for evaluating fit limits given by user
    //typedef exprtk::symbol_table<float> symbol_table_t;
    //typedef exprtk::expression<float> expression_t;
    //typedef exprtk::parser<float> parser_t;
    
    class AnalyzeResponseUniformityClusters : public AnalyzeResponseUniformity {
        
    public:
        //Constructors
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Default
        AnalyzeResponseUniformityClusters();
        
        //Set the detector and anlysis parameters at construction
        AnalyzeResponseUniformityClusters(Uniformity::AnalysisSetupUniformity inputSetup, Uniformity::DetectorMPGD inputDet);
        
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Filling*****************
        //Loops over all stored clusters in detMPGD and Book histograms for the full detector
        virtual void fillHistos();
        
        //Fitting*****************
        //Loops over all slices in detMPGD and fits Booked histograms for the full detector
        virtual void fitHistos();
        
        //Loading*****************
        //Loads a ROOT file previously created by an instance of AnalyzeResponseUniformityClusters
        //Loads all TObjects found in the input ROOT file into detMPGD;
        //Any previously stored information in detMPGD is lost.
        virtual void loadHistosFromFile(std::string & strInputMappingFileName, std::string & strInputROOTFileName);
        
        //Pass/Fail***************
        //Checks to see if the detector's uniformity is within requested amount
        //virtual void checkUniformity();
        
        //Storing*****************
        //Stores booked cluster histograms (for those histograms that are non-null)
        void storeHistos(std::string & strOutputROOTFileName, std::string strOption);
        
        //Stores booked cluster fits (for those fits that are non-null)
        void storeFits(std::string & strOutputROOTFileName, std::string strOption);
        
        //Stores TObjects created in createResponseMap() in the requested output file
        //Placeholder
        //void storeResponseMap(std::string strOutputROOTFileName, std::string strOption);
        
        //Visualizing*************
        //Displays the results of the analysis in a user friendly manner
        //Placeholder
        //void visualizeResponseMap(std::string strOutputROOTFileName, std::string strOption);
        //Actually it's better to put this in a separate file
        
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


#endif /* defined(____AnalyzeResponseUniformityClusters__) */
