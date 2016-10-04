//
//  ParameterLoaderPlotter.h
//  
//
//  Created by Brian L Dorney on 03/10/16.
//
//

#ifndef ____ParameterLoaderPlotter__
#define ____ParameterLoaderPlotter__

//C++ Includes
#include <stdio.h>

//Framework Includes
#include "ParameterLoader.h"
#include "PlotterUtilityTypes.h"
#include "QualityControlSectionNames.h"


//ROOT Includes

namespace QualityControl {
    namespace Plotter {
        class ParameterLoaderPlotter : public QualityControl::Uniformity::ParameterLoader {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            ParameterLoader();
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Opens a text file set by the user and loads the requested parameters
            virtual void loadParameters(std::ifstream &file_Input, bool bVerboseMode, InfoCanvas &inputCanvInfo);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
        private:
            //Loads Parameters of the Canvas
            virtual void loadParametersCanvas(std::ifstream & file_Input, InfoCanvas &inputCanvInfo);
            
            //Loads Parameters of the Plot
            virtual void loadParametersPlot()
            
            //Loads Data (if any)
            virtual void loadData()
            
        }; //End class ParameterLoaderPlotter
    } //End namespace Plotter
} //End namespace QualityControl


#endif /* defined(____ParameterLoaderPlotter__) */
