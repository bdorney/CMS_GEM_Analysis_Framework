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
#include <iterator>
#include <stdio.h>
#include <string>
#include <map>
#include <memory>
#include <vector>

//Framework Includes
//#include "DetectorMPGD.h"
#include "TimingUtilityFunctions.h"
#include "TimingUtilityTypes.h"
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
    typedef exprtk::symbol_table<float> symbol_table_t;
    typedef exprtk::expression<float> expression_t;
    typedef exprtk::parser<float> parser_t;
    
    class AnalyzeResponseUniformity {
        
    public:
        //Constructors
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Default
        AnalyzeResponseUniformity();
        
        //Declare the detector and the requested granularity
        //AnalyzeResponseUniformity(int iNSlices, Uniformity::DetectorMPGD inputDet);
        
        //Set the detector and anlysis parameters at construction
        AnalyzeResponseUniformity(Uniformity::AnalysisSetupUniformity inputSetup, Uniformity::DetectorMPGD inputDet);
        
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Filling*****************
        //Loops over all stored clusters in detMPGD and Book histograms for the full detector
        void fillHistos();
        
        //Fitting*****************
        //Loops over all slices in detMPGD and fits Booked histograms for the full detector
        void fitHistos();
        
        //Loading*****************
        //Loads a ROOT file previously created by an instance of AnalyzeResponseUniformity
        //Loads all TObjects found in the input ROOT file into detMPGD;
        //Any previously stored information in detMPGD is lost.
        void loadHistosFromFile(std::string strInputROOTFileName);
        
        //Pass/Fail***************
        //Checks to see if the detector's uniformity is within requested amount
        void checkUniformity();
        //bool checkUniformity(bool &bInput);
        
        //Storing*****************
        //Stores booked histograms (for those histograms that are non-null)
        void storeHistos(std::string strOutputROOTFileName, std::string strOption);
        
        //Stores booked fits (for those fits that are non-null)
        void storeFits(std::string strOutputROOTFileName, std::string strOption);
        
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
        Uniformity::DetectorMPGD getDetector(){ return detMPGD; };
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Sets the Analysis Setup
        void setAnalysisParameters(Uniformity::AnalysisSetupUniformity inputSetup){ aSetup = inputSetup; return; };
        
        //Sets the Detector
        void setDetector(Uniformity::DetectorMPGD inputDet){ detMPGD = inputDet; return; };
        
    private:
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //template<class TFileStructure>
        //TDirectory * getDirectory(int iIdentifier, std::string strName, TFileStructure *inputStructure);
        
        //Returns a fit whose parameters match those defined in the AnalysisSetupUniformity
        TF1 getFit(int iEta, int iPhi, int iSlice, Timing::HistoSetup & setupHisto, std::shared_ptr<TH1F> hInput, TSpectrum &specInput );
        
        //Based on an input expression (strInputExp) returns the boundary of a fit parameter
        float getFitBoundary(std::string &strInputExp, std::shared_ptr<TH1F> hInput, TSpectrum &specInput);
        
        //Returns a TGraph Errors whose parameters match those defined in the input HistoSetup object
        TGraphErrors getGraph(int iEta, int iPhi, Timing::HistoSetup &setupHisto);
        
        //Returns a histogram whose parmeters match those defined in the input HistoSetup object
        TH1F getHistogram(int iEta, int iPhi, Timing::HistoSetup &setupHisto);
        //TH2F getHistogram2D( Timing::HistoSetup &setupHisto );
        
        //Formats a given input string such that it follows the iEta, iPhi, iSlice naming convention
        std::string getNameByIndex(int iEta, int iPhi, int iSlice, std::string & strInputPrefix, std::string & strInputName);
        std::string getNameByIndex(int iEta, int iPhi, int iSlice, const std::string & strInputPrefix, const std::string & strInputName);
        
        //Searchs the input fit for the given variable; returns it
        float getPeakPos( std::shared_ptr<TF1> fitInput, Timing::HistoSetup & setupHisto );
        
        //Searchs the input fit for the given variable; returns it
        float getPeakPosError( std::shared_ptr<TF1> fitInput, Timing::HistoSetup & setupHisto );
        
        //Given an input histogram and TSpectrum returns a numeric value based on the input keyword; supported keywords are "AMPLITUDE,MEAN,PEAK,SIGMA"
        float getValByKeyword(std::string strInputKeyword, std::shared_ptr<TH1F> hInput, TSpectrum &specInput);
        
        //Data Members
        //------------------------------------------------------------------------------------------------------------------------------------------
        std::string strAnalysisName;
        
        const std::vector<std::string> vec_strSupportedKeywords = {"AMPLITUDE", "MEAN","PEAK","SIGMA"}; //Supported Keywords for fit setup
        
        Uniformity::AnalysisSetupUniformity aSetup; //Container to define the analysis setup
        
        Uniformity::DetectorMPGD detMPGD; //Link to header file in AnalyzeResponseUniformity.cpp; Detector object
        
        //std::shared_ptr<TH2F> hEta_v_Pos_Occupancy;
        //std::shared_ptr<TH2F> hEta_v_Pos_ClustADC_Fit_PkPos;
        //std::shared_ptr<TH2F> hEta_v_Pos_ClustADC_Fit_NormChi2;
        //std::shared_ptr<TH2F> hEta_v_Pos_ClustADC_Spec_PkPos;
        //std::shared_ptr<TH2F> hEta_v_Pos_ClustADC_Spec_NumPks;
        
        //std::shared_ptr<TH2F> hEta_v_SliceNum_Occupancy;
        //std::shared_ptr<TH2F> hEta_v_SliceNum_ClustADC_Fit_PkPos;
        //std::shared_ptr<TH2F> hEta_v_SliceNum_ClustADC_Fit_NormChi2;
        //std::shared_ptr<TH2F> hEta_v_SliceNum_ClustADC_Spec_PkPos;
        //std::shared_ptr<TH2F> hEta_v_SliceNum_ClustADC_Spec_NumPks;
    }; //End class AnalyzeResponseUniformity
} //End namespace Uniformity


#endif /* defined(____AnalyzeResponseUniformity__) */
