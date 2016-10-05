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
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

//Framework Includes
#include "ParameterLoader.h"
#include "PlotterUtilityFunctions.h"
#include "PlotterUtilityTypes.h"
#include "QualityControlSectionNames.h"
#include "TimingUtilityFunctions.h"
#include "UniformityUtilityFunctions.h"

//ROOT Includes

namespace QualityControl {
    namespace Plotter {
        class ParameterLoaderPlotter : public QualityControl::Uniformity::ParameterLoader {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            ParameterLoaderPlotter();
            
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
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Loads Parameters of the Canvas
            virtual void loadParametersCanvas(std::ifstream & file_Input, InfoCanvas &inputCanvInfo);
            
            //Loads Parameters of the Plot
            virtual void loadParametersPlot(std::ifstream & file_Input, InfoPlot & inputPlotInfo);
            
            //Loads Data (if any)
            virtual std::vector<DataPoint> loadData(std::ifstream & file_Input);

            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual int getColLabelPosition(std::vector<std::string> vec_strInputParam, std::string & strInputLabel);
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Sets the value on a pair of parameters based on input data type
            //We don't use std::pair here in case user gives 3 or more parameters (of which later ones are discarded)
            //Wish I had a good idea on how to template this, but each of them individually call stoi, stof, or Timing::convert2bool based on the input type.
            virtual void setParameters(std::vector<std::string> & vec_strInputParam, bool &bInput1, bool &bInput2);
            virtual void setParameters(std::vector<std::string> & vec_strInputParam, float &fInput1, float &fInput2);
            virtual void setParameters(std::vector<std::string> &vec_strInputParam, int &iInput1, int &iInput2);
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
            QualityControl::Plotter::SecNamesPlotter m_headers_plots;
            
            QualityControl::Plotter::ColNamesPlotter m_col_labels;
        }; //End class ParameterLoaderPlotter
    } //End namespace Plotter
} //End namespace QualityControl


#endif /* defined(____ParameterLoaderPlotter__) */
