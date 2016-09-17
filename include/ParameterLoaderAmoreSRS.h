//
//  ParameterLoaderAmoreSRS.h
//  
//
//  Created by Brian L Dorney on 26/01/16.
//
//

#ifndef ____ParameterLoaderAmoreSRS__
#define ____ParameterLoaderAmoreSRS__

//C++ Includes
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>

//My Includes
#include "ParameterLoader.h"
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityFunctions.h"

//ROOT Includes

namespace QualityControl {
    namespace Uniformity {
        class ParameterLoaderAmoreSRS : public ParameterLoader{
            
        private:
            DetectorMPGD det_GE11;  //Link to header file in ParameterLoaderAmoreSRS.cpp file
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            ParameterLoaderAmoreSRS();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Load the Amore mapping file and initializes the detector
            void loadAmoreMapping(std::string & strInputMappingFileName);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Returns the detector; this allows for some more flexibility; maybe we want to manipulate it after initializing in the future
            DetectorMPGD getDetector(){ return det_GE11; };
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
        private:
            
            //Placeholder
            
        }; //End class ParameterLoaderAmore
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____ParameterLoaderAmore__) */
