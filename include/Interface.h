//
//  Interface.h
//  
//
//  Created by Brian L Dorney on 05/07/16.
//
//

#ifndef ____Interface__
#define ____Interface__

//C++ Includes
#include <stdio.h>

//Framework Includes
#include "SelectorCluster.h"
#include "SelectorHit.h"
#include "DetectorMPGD.h"   //Needs to be included before AnalyzeResponseUniformity.h
#include "AnalyzeResponseUniformityClusters.h"
#include "AnalyzeResponseUniformityHits.h"
#include "UniformityUtilityTypes.h"
#include "VisualizeUniformity.h"

//ROOT Includes

namespace QualityControl {
    namespace Uniformity{
        class Interface : public FrameworkBase {
            
        public:
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Sets the analysis setup in all classes
            virtual void initialize(QualityControl::Uniformity::AnalysisSetupUniformity inputAnaSetup, QualityControl::Uniformity::RunSetup inputRunSetup);
            
            //Sets the analysis setup in all classes
            //Initializes all histograms
            virtual void initialize(QualityControl::Uniformity::AnalysisSetupUniformity inputAnaSetup, QualityControl::Uniformity::RunSetup inputRunSetup, QualityControl::Uniformity::DetectorMPGD & inputDet);
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            virtual void setRunParameters(Uniformity::RunSetup inputSetup){ rSetup = inputSetup; return; };
            
        protected:
            
            //Constructors
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Actions - Methods that Do Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Getters - Methods that Get (i.e. Return) Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Printers - Methods that Print Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Setters - Methods that Set Something
            //------------------------------------------------------------------------------------------------------------------------------------------
            
            //Attributes
            //------------------------------------------------------------------------------------------------------------------------------------------
            //bool bInitialized;
            
            //Analyzers
            AnalyzeResponseUniformityClusters clustAnalyzer;
            AnalyzeResponseUniformityHits hitAnalyzer;
            
            //Containers
            RunSetup rSetup;
            
            //Selectors
            SelectorCluster clustSelector;
            SelectorHit hitSelector;
            
            //Visualizer
            VisualizeUniformity visualizeUni;
        }; //End class Interface
    } //End namespace Uniformity
} //End namespace QualityControl


#endif /* defined(____Interface__) */
