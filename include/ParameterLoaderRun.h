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
#include <map>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
#include "ParameterLoader.h"
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityOperators.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes

namespace QualityControl {
    namespace Uniformity {
        class ParameterLoaderRun : public ParameterLoader {
            
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
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Memebers
            
        private:
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            std::string m_strSecBegin_RunInfo, m_strSecEnd_RunInfo;
        }; //End ParameterLoaderRun
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____ParameterLoaderRun__) */
