//
//  ParameterLoaderRun.h
//  
//
//  Created by Brian L Dorney on 03/05/16.
//
//

#ifndef ____ParameterLoaderRun__
#define ____ParameterLoaderRun__

//C++ Includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes

namespace QualityControl {
    namespace Uniformity {
        class ParameterLoaderRun {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            ParameterLoaderRun();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Opens a text file set by the user and loads the requested parameters
            virtual void loadRunParameters(std::ifstream &file_Input, bool bVerboseMode, RunSetup & inputRunSetup);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Gets the list of input runs from the input config file
            std::vector<std::string> getRunList(std::ifstream &file_Input, bool bVerboseMode);
            
            //Gets the run number from an input run name
            int getRunNumber(std::string & strRunName);
            
            //As loadRunParameters() above but does not need an AnalysisSetupUniformity argument
            virtual RunSetup getRunParameters(std::ifstream &file_Input, bool bVerboseMode){
                Uniformity::RunSetup rSetup;
                loadRunParameters(file_Input, bVerboseMode, rSetup);
                return rSetup;
            };
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Memebers
            
        private:
            std::string strSecBegin_RunInfo;
            std::string strSecBegin_RunList;
            
            std::string strSecEnd_RunInfo;
            std::string strSecEnd_RunList;
        }; //End ParameterLoaderRun
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____ParameterLoaderRun__) */
