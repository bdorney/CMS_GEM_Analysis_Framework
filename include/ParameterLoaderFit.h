//
//  ParameterLoaderFit.h
//  
//
//  Created by Brian L Dorney on 10/10/16.
//
//

#ifndef ____ParameterLoaderFit__
#define ____ParameterLoaderFit__

//C++ Includes
#include <algorithm>
#include <stdio.h>
#include <string>
#include <utility>

//Framework Includes
#include "ParameterLoader.h"
#include "PlotterUtilityFunctions.h"
#include "PlotterUtilityTypes.h"
#include "QualityControlSectionNames.h"

//ROOT Includes

namespace QualityControl {
    namespace Uniformity {
        class ParameterLoaderFit : public ParameterLoader {
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            //ParameterLoaderFit();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Opens a text file set by the user and loads the requested parameters
            virtual void loadAnalysisParametersFits(std::ifstream &file_Input, QualityControl::Plotter::InfoFit & inputFitInfo);
            
            
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
            //QualityControl::Uniformity::SecNamesAna m_headers_Ana;
            
            QualityControl::Plotter::SecNamesPlotter m_headers_plots;
        }; //End class ParameterLoaderFit
    } //End namespace Uniformity
} //End namespace QualityControl


#endif /* defined(____ParameterLoaderFit__) */
