//
//  Analyzer.h
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#ifndef ____Analyzer__
#define ____Analyzer__

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
#include "FrameworkBase.h"
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
    //Used for evaluating fit limits given by user
    typedef exprtk::symbol_table<float> symbol_table_t;
    typedef exprtk::expression<float> expression_t;
    typedef exprtk::parser<float> parser_t;
    
    class Analyzer : public FrameworkBase {
        
    public:
        //Constructors
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Default
        Analyzer();
        
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
    protected:
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Determines if a fit is "good"
        //Good fits have:
        //		fit parameter != 0 (hold that thought...better implementation needed?)
        //		fit parameter != parameter limit
        //		Percent Error on fit parameter (sigma/value) <= 0.1
        virtual bool isQualityFit(std::shared_ptr<TF1> fitInput);
        
        //As above but for a specific parameter iPar
        virtual bool isQualityFit(std::shared_ptr<TF1> fitInput, int iPar);
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Returns a fit whose parameters match those defined in the AnalysisSetupUniformity
        virtual TF1 getFit(Plotter::InfoFit & setupFit, std::shared_ptr<TH1F> hInput, TSpectrum &specInput );
        
        //strInput is understood to be a number or an algebraic expression
        //If strInput contains a substr matching to one or more elements of vec_strSupportedKeywords it is treated as an algebraic expression
        //If so it parses the expression and returns a numeric value
        //If not it is treated as a number and attempts to convert strInput directly
        virtual float getParsedInput(std::string &strInputExp, std::shared_ptr<TH1F> hInput, TSpectrum &specInput);
        
        //Returns a TGraph Errors whose parameters match those defined in the input HistoSetup object
        virtual TGraphErrors getGraph(Timing::HistoSetup &setupHisto);
        
        //Returns a histogram whose parmeters match those defined in the input HistoSetup object
        virtual TH1F getHistogram(Timing::HistoSetup &setupHisto);
        
        //Returns a histogram whose parmeters match those defined in the input HistoSetup object
        virtual TH2F getHistogram2D(Timing::HistoSetup &setupHisto_X, Timing::HistoSetup &setupHisto_Y);
        
        //Formats a given input string such that it follows the iEta, iPhi, iSlice naming convention
        
        //Searchs the input fit for the given variable (strParam); returns it
        virtual float getParam( std::shared_ptr<TF1> fitInput, Plotter::InfoFit & setupFit, std::string strParam );
        
        //Searchs the input fit for the error on the given variable (strParam); returns it
        virtual float getParamError( std::shared_ptr<TF1> fitInput, Plotter::InfoFit & setupFit, std::string strParam );
        
        //Given an input histogram and TSpectrum returns a numeric value based on the input keyword; supported keywords are "AMPLITUDE,MEAN,PEAK,SIGMA"
        virtual float getValByKeyword(std::string strInputKeyword, std::shared_ptr<TH1F> hInput, TSpectrum &specInput);
        
        //Data Members
        //------------------------------------------------------------------------------------------------------------------------------------------
        std::string strAnalysisName;
        
        const std::vector<std::string> vec_strSupportedKeywords = {"AMPLITUDE", "FWHM", "HWHM", "MEAN","PEAK","SIGMA"}; //Supported Keywords for fit setup
    }; //End class Analyzer
} //End namespace QualityControl

#endif /* defined(____Analyzer__) */
