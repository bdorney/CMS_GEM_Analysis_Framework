//
//  FrameworkBase.h
//  
//
//  Created by Brian L Dorney on 05/07/16.
//
//

#ifndef ____FrameworkBase__
#define ____FrameworkBase__

//C++ Includes
#include <stdio.h>

//Framework Includes
#include "DetectorMPGD.h"
#include "UniformityUtilityTypes.h"

//ROOT Includes

namespace QualityControl {
    class FrameworkBase {
        
    public:
        //Constructors
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Default
        FrameworkBase();
        
        //Actions - Methods that Do Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Getters - Methods that Get (i.e. Return) Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        virtual Uniformity::DetectorMPGD getDetector(){ return detMPGD; };
        
        //Sets the run number
        virtual int getRunNum(){ return iNum_Run; };
        
        //Printers - Methods that Print Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        
        //Setters - Methods that Set Something
        //------------------------------------------------------------------------------------------------------------------------------------------
        //Sets the Analysis Setup
        virtual void setAnalysisParameters(Uniformity::AnalysisSetupUniformity inputSetup){ aSetup = inputSetup; return; };
        
        //Sets the Detector
        virtual void setDetector(Uniformity::DetectorMPGD & inputDet){ detMPGD = inputDet; return; };

        //Sets the run number
        virtual void setRunNum(int iInput){ iNum_Run = iInput; return; };
        
        //Sets the Verbose Output Mode
        virtual void setVerboseMode(bool bInput){ bVerboseMode = bInput; return; };
        
    protected:
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
        bool bVerboseMode; //flag for level of printing to the user
        
        int iNum_Run;   //Run number
        
        QualityControl::Uniformity::AnalysisSetupUniformity aSetup; //Container to define the analysis setup
        
        QualityControl::Uniformity::DetectorMPGD detMPGD; //Link to header file in AnalyzeResponseUniformity.cpp; Detector object
    }; //End class FrameworkBase
} //End namespace QualityControl


#endif /* defined(____FrameworkBase__) */
