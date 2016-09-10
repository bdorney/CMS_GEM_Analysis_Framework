//
//  ParameterLoader.h
//  
//
//  Created by Brian L Dorney on 10/09/16.
//
//

#ifndef ____ParameterLoader__
#define ____ParameterLoader__

//C++ Includes
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <utility>
#include <vector>

//Framework Includes
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityOperators.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes

namespace QualityControl {
    namespace Uniformity {
        class ParameterLoader {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            ParameterLoader();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Gets the list of input runs from the input config file
            std::vector<std::pair<int, std::string> > getPairedRunList(std::ifstream &file_Input, bool bVerboseMode);
            std::vector<std::string> getRunList(std::ifstream &file_Input, bool bVerboseMode);
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Memebers
            
        protected:
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Gets the run number from an input run name
            int getRunNumber(std::string strRunName);
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
            std::string strSecBegin_RunList, strSecEnd_RunList;
        }; //End class ParameterLoader
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____ParameterLoader__) */
