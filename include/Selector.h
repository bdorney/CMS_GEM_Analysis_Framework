//
//  Selector.h
//  
//
//  Created by Brian L Dorney on 14/03/16.
//
//

#ifndef ____Selector__
#define ____Selector__

//C++ Includes
#include <stdio.h>

//Framework Includes
#include "FrameworkBase.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes

namespace QualityControl {
    namespace Uniformity {
        class Selector : public FrameworkBase {
            
        public:
            
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Default
            Selector();
            
            //With Analysis Setup File Given at time of construction
            Selector(Uniformity::AnalysisSetupUniformity &inputSetup);
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            //Sets the analysis parameters
            /*virtual void setAnalysisParameters(Uniformity::AnalysisSetupUniformity &inputSetup){
             aSetupUniformity = inputSetup;
             return;
             };*/
            
        protected:
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Data Members
            //Uniformity::AnalysisSetupUniformity aSetupUniformity;
            
        }; //End class Selector
        
    } //End namespace Uniformity
} //End namespace QualityControl

#endif /* defined(____Selector__) */
