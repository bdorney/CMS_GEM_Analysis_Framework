//
//  ParameterLoaderAnaysis.h
//  
//
//  Created by Brian L Dorney on 28/01/16.
//
//

#ifndef ____ParameterLoaderAnaysis__
#define ____ParameterLoaderAnaysis__

//C++ Includes
#include <fstream>
#include <stdio.h>
#include <string>
#include <utility>

//Framework Includes
#include "ParameterLoader.h"
#include "TimingUtilityFunctions.h"
#include "TimingUtilityTypes.h"
#include "UniformityUtilityTypes.h"
#include "UniformityUtilityFunctions.h"

//ROOT Includes

namespace QualityControl {
    namespace Uniformity {
        class ParameterLoaderAnaysis : public ParameterLoader {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            ParameterLoaderAnaysis();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Opens a text file set by the user and loads the requested parameters
            virtual void loadAnalysisParameters(std::string & strInputSetupFile, AnalysisSetupUniformity &aSetupUniformity);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //As loadAnalysisParameters() above but does not need an AnalysisSetupUniformity argument
            virtual AnalysisSetupUniformity getAnalysisParameters(std::string & strInputSetupFile);
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Memebers
            
        private:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual void loadAnalysisParametersFits(std::ifstream & inputFileStream, Timing::HistoSetup &hSetup);
            
            //Called when loading analysis parameters; relative to histograms
            //This is the top level method; this method calls loadAnalysisParametersHistograms(ifstream, Timing::HistoSetup) depending on which histogram is requested by the user
            virtual void loadAnalysisParametersHistograms(std::ifstream &inputFileStream, AnalysisSetupUniformity &aSetupUniformity );
            
            //Loads parameters for a specific histogram setup
            virtual void loadAnalysisParametersHistograms(std::ifstream &inputFileStream, Timing::HistoSetup &hSetup);
            
            //Called by loadAnalysisParameters(), loading parameters relative to the timing analysis (not implemented yet...maybe in the future we would want to compare time resolution with gain uniformity...we can dream)
            virtual void loadAnalysisParametersTiming(std::ifstream &inputFileStream, AnalysisSetupUniformity &aSetupUniformity);
            
            //Called by loadAnalysisParameters(), loading parameters relative to the uniformity analysis
            virtual void loadAnalysisParametersUniformity(std::ifstream &inputFileStream, AnalysisSetupUniformity &aSetupUniformity);
            
            //Data Members
            bool bVerboseMode_IO;
            
            std::string strSecBegin_Analysis;       //Defines how the file will look
            std::string strSecBegin_Timing;         //Hard coded section headers
            std::string strSecBegin_Uniformity;     //Staring point
            std::string strSecBegin_Uniformity_Fit;
            std::string strSecBegin_Uniformity_Histo;
            
            std::string strSecEnd_Analysis;         //Defines how the file will look
            std::string strSecEnd_Timing;           //Hard coded section headers
            std::string strSecEnd_Uniformity;       //ending point
            std::string strSecEnd_Uniformity_Fit;
            std::string strSecEnd_Uniformity_Hiso;
            
        }; //End Class ParameterLoaderAnalysis
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____ParameterLoaderAnaysis__) */
